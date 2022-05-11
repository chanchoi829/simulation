#include "Controller.h"
#include "Model.h"
#include "Ship.h"
#include "Ship_component.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_component_factory.h"
#include "Utility.h"
#include "Local_view.h"
#include "Map_view.h"
#include "Sailing_view.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Initialize command maps
Controller::Controller()
{
    view_command_map = {{"open_map_view", &Controller::open_map_view},
        {"close_map_view", &Controller::close_map_view},
        {"open_sailing_view", &Controller::open_sailing_view},
        {"close_sailing_view", &Controller::close_sailing_view},
        {"open_local_view", &Controller::open_local_view},
        {"close_local_view", &Controller::close_local_view},
        {"default", &Controller::view_default},
        {"size", &Controller::view_size},
        {"zoom", &Controller::view_zoom},
        {"pan", &Controller::view_pan},
        {"show", &Controller::view_show}};

    ship_command_map = {{"course", &Controller::ship_course},
        {"position", &Controller::ship_position},
        {"destination", &Controller::ship_destination},
        {"load_at", &Controller::ship_load_at},
        {"unload_at", &Controller::ship_unload_at},
        {"chain_all", &Controller::ship_chain_all_ship},
        {"chain", &Controller::ship_chain_ship},
        {"unchain", &Controller::ship_unchain_ship},
        {"dock_at", &Controller::ship_dock_at},
        {"attack", &Controller::ship_attack},
        {"refuel", &Controller::ship_refuel},
        {"stop", &Controller::ship_stop},
        {"stop_attack", &Controller::ship_stop_attack}};

    model_command_map = {{"status", &Controller::model_status},
        {"go", &Controller::model_go},
        {"create", &Controller::model_create},
        {"create_group", &Controller::model_create_composite},
        {"remove_group", &Controller::model_remove_composite},
        {"remove_ship_from_group", &Controller::model_remove_ship_from_composite},
        {"add_ship_to_group", &Controller::model_add_ship_to_composite},
        {"add_group_to_group", &Controller::model_add_composite_to_composite},
        {"describe_groups", &Controller::model_describe_groups}};

    command_set = {"open_map_view",
        "close_map_view",
        "open_sailing_view",
        "close_sailing_view",
        "open_local_view",
        "close_local_view",
        "default",
        "size",
        "zoom",
        "pan",
        "show",
        "quit",
        "status",
        "go",
        "create",
        "course",
        "position",
        "destination",
        "load_at",
        "unload_at",
        "chain_all",
        "chain",
        "unchain",
        "dock_at",
        "attack",
        "refuel",
        "stop",
        "stop_attack",
        "chain",
        "unchain",
        "chain_all"
        "create_group",
        "add_ship_to_group",
        "remove_ship_from_group",
        "remove_group",
        "add_group_to_group",
        "describe_groups"};
}

// Run the program by acccepting user commands
void Controller::run()
{
    string first_input;
    while (true) {
        try {
            cout << "\nTime " << Model::get_instance().get_time() << ": Enter command: ";
            cin >> first_input;

            // If the first word is "quit", check if View
            // is still open, and detach and delete view_ptr.
            // Then exit the while loop.
            if (first_input == "quit") {
                cout << "Done";
                return;
            }

            // Check and see if first_input is a Ship's name.
            shared_ptr<Ship_component> ship_ptr = Model::get_instance().get_ship_ptr(first_input);

            // When there is a Ship that matches first_input
            if (ship_ptr) {
                process_ship_command(ship_ptr);
            } else {
                shared_ptr<Ship_component> composite_ptr = Model::get_instance().get_ship_composite_ptr(first_input);

                // When there is a Ship_composite which matches first_input
                if (composite_ptr) {
                    process_ship_command(composite_ptr);
                } else {
                    // Check if first_input is a Model command
                    auto model_command_iter = model_command_map.find(first_input);

                    // If it is a Model command, run the Model command.
                    if (model_command_iter != model_command_map.cend()) {
                        model_command_iter->second(this);
                    } else {
                        // Check if first_input is a View command
                        auto view_command_iter = view_command_map.find(first_input);

                        // If it is a View commnad, run it.
                        // If it is not a View command, throw an Error
                        // because it cannot be any other command.
                        if (view_command_iter != view_command_map.cend())
                            view_command_iter->second(this);
                        else
                            throw Error("Unrecognized command!");
                    }
                }
            }
        }
        // If an Error is thrown, skip rest of the line.
        catch (Error& e) {
            cout << e.what() << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// View commands

// create the View and attach to the Model.
// If the View is already open, throw an Error.
void Controller::open_map_view()
{
    if (map_view_ptr)
        throw Error("Map view is already open!");

    map_view_ptr = make_shared<Map_view>();
    Model::get_instance().attach(map_view_ptr);
    view_vec.push_back(map_view_ptr);
}

// detach the View from the Model and destroy
// it. If the View was not open, throw an Error.
void Controller::close_map_view()
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");

    Model::get_instance().detach(map_view_ptr);
    view_vec.erase(find(view_vec.cbegin(), view_vec.cend(), map_view_ptr));
    map_view_ptr.reset();
}

// create and open the sailing data view. Error: sailing
// data view is already open.
void Controller::open_sailing_view()
{
    if (sailing_view_ptr)
        throw Error("Sailing data view is already open!");

    sailing_view_ptr = make_shared<Sailing_view>();
    view_vec.push_back(sailing_view_ptr);
    Model::get_instance().attach(sailing_view_ptr);
}

// close and destroy the sailing data view view.
// Error: no sailing data view is open.
void Controller::close_sailing_view()
{
    if (!sailing_view_ptr)
        throw Error("Sailing data view is not open!");

    Model::get_instance().detach(sailing_view_ptr);
    view_vec.erase(find(view_vec.cbegin(), view_vec.cend(), sailing_view_ptr));
    sailing_view_ptr.reset();
}

// create and open a local view centered on the ship
// with name <name>.Errors in order of checks : no ship
// with that name; local view is already open for that name.
void Controller::open_local_view()
{
    string ship_name;
    cin >> ship_name;
    shared_ptr<Ship> ship_ptr = Model::get_instance().get_ship_ptr(ship_name);

    if (!ship_ptr)
        throw Error("Ship not found!");

    shared_ptr<Local_view> new_view = make_shared<Local_view>(Local_view(ship_ptr->get_name()));

    if (!local_view_ptr_map.insert(make_pair(ship_ptr->get_name(), new_view)).second)
        throw Error("Local view is already open for that ship!");

    view_vec.push_back(new_view);
    Model::get_instance().attach(new_view);
}

// close and destroy the view with that name.
// Error: local view is not open for that name.
void Controller::close_local_view()
{
    string ship_name;
    cin >> ship_name;

    auto view_ptr = local_view_ptr_map.find(ship_name);
    if (view_ptr == local_view_ptr_map.cend())
        throw Error("Local view for that ship is not open!");

    Model::get_instance().detach(view_ptr->second);
    view_vec.erase(find(view_vec.cbegin(), view_vec.cend(), view_ptr->second));
    local_view_ptr_map.erase(view_ptr);
}

// restore the default settings of the map.
// If the View is not open, throw an Error.
void Controller::view_default() const
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");

    map_view_ptr->set_defaults();
}

// read a single integer for the size of
// the map (number of both rows and columns).
// If the View is not open, throw an Error.
void Controller::view_size() const
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");

    int size;
    cin >> size;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected an integer!");
    }

    map_view_ptr->set_size(size);
}

// read a double value for the scale of the map
// (number of nm per cell). If the View is not open, throw an Error.
void Controller::view_zoom() const
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");

    double scale;
    cin >> scale;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    map_view_ptr->set_scale(scale);
}

// read a pair of double values for the (x, y) origin of the map.
// If the View is not open, throw an Error.
void Controller::view_pan() const
{
    if (!map_view_ptr)
        throw Error("Map view is not open!");

    double x, y;
    cin >> x >> y;
    map_view_ptr->set_origin(Point(x, y));
}

// tell the View to draw the map note that the Model and the objects
// should have kept the View up to date by calling the relevant update
// functions.If the View is not open, throw an Error.
void Controller::view_show() const
{
    if (view_vec.empty())
        throw Error("Map view is not open!");

    for_each(view_vec.cbegin(), view_vec.cend(), [](const shared_ptr<View> ptr) { ptr->draw(); });
}

// Model commands

// have all the objects describe themselves; the output should be in
// alphabetical order by name of all of the objects.
void Controller::model_status() const
{
    Model::get_instance().describe();
}

// call the Model::update() function
void Controller::model_go() const
{
    Model::get_instance().update();
}

// create a new Ship using the supplied name,
// type name, and initial position.
void Controller::model_create() const
{
    string ship_name;
    cin >> ship_name;

    check_if_name_valid(ship_name);

    // Read in object_type which can be
    // Tanker or Cruiser. add_ship() will check
    // if it is one of these two.
    string object_type;
    cin >> object_type;

    double x, y;
    cin >> x;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    cin >> y;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    Model::get_instance().add_ship(create_ship(ship_name, object_type, Point(x, y)));
}

// Create a Ship_composite and add it to the Model
void Controller::model_create_composite() const
{
    string composite_name;
    cin >> composite_name;

    check_if_name_valid(composite_name);

    Model::get_instance().add_composite(create_composite(composite_name));
}

// Remove a Ship_composite from the Model
void Controller::model_remove_composite() const
{
    string composite_name;
    cin >> composite_name;

    Model::get_instance().remove_composite(composite_name);
}

// Remove a Ship from a Ship_composite
void Controller::model_remove_ship_from_composite() const
{
    string composite_name, ship_name;
    cin >> composite_name >> ship_name;

    Model::get_instance().remove_ship_from_composite(composite_name, ship_name);
}

// Add a Ship to a Ship_composite
void Controller::model_add_ship_to_composite() const
{
    string composite_name, ship_name;
    cin >> composite_name >> ship_name;

    Model::get_instance().add_ship_to_composite(composite_name, Model::get_instance().get_ship_ptr(ship_name));
}

// Add a Ship_composite to a Ship_composite
void Controller::model_add_composite_to_composite() const
{
    string composite_name, new_composite_name;
    cin >> composite_name >> new_composite_name;

    Model::get_instance().add_composite_to_composite(composite_name, create_composite(new_composite_name));
}

// Describe a set of Ship_composites
void Controller::model_describe_groups() const
{
    Model::get_instance().describe_composite();
}

// Ship commands

// read a compass heading and a speed (both doubles) for the
// Ship to set course and speed. basic validity check:
// 0.0 <= compass heading < 360.0, speed >= 0.0
void Controller::ship_course(shared_ptr<Ship_component> const ship_ptr) const
{
    double compass_heading;
    cin >> compass_heading;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    if (compass_heading < 0.0 || compass_heading >= 360.0)
        throw Error("Invalid heading entered!");

    double speed;
    cin >> speed;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    if (speed < 0.0)
        throw Error("Negative speed entered!");

    ship_ptr->set_course_and_speed(compass_heading, speed);
}

// read an (x, y) position and then a speed (all doubles) for
// the Ship to set destination position and speed to go to.
// basic validity check : x, y can have any value, speed >= 0.0
void Controller::ship_position(shared_ptr<Ship_component> const ship_ptr) const
{
    double x, y, speed;
    cin >> x;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    cin >> y;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    cin >> speed;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    if (speed < 0.0)
        throw Error("Negative speed entered!");

    ship_ptr->set_destination_position_and_speed(Point(x, y), speed);
}

// read an Island name and a speed (a double) for the ship
// to set destination Island and speed. basic validity check
// : Island must exist, speed >= 0.0
void Controller::ship_destination(shared_ptr<Ship_component> const ship_ptr) const
{
    string island_name;
    cin >> island_name;

    shared_ptr<Island> island_ptr = Model::get_instance().get_island_ptr(island_name);

    double speed;
    cin >> speed;

    if (!cin.good()) {
        cin.clear();
        throw Error("Expected a double!");
    }

    if (speed < 0.0)
        throw Error("Negative speed entered!");

    ship_ptr->set_destination_island_and_speed(island_ptr, speed);
}

// read an Island name to load at.
// basic validity check : Island must exist
void Controller::ship_load_at(shared_ptr<Ship_component> const ship_ptr) const
{
    string island_name;
    cin >> island_name;

    // Get island_ptr from island_name and set load destination
    // to the found Island.
    shared_ptr<Island> island_ptr = Model::get_instance().get_island_ptr(island_name);
    ship_ptr->set_load_destination(island_ptr);
}

// read an Island name to unload at.
// basic validity check : Island must exist
void Controller::ship_unload_at(shared_ptr<Ship_component> const ship_ptr) const
{
    string island_name;
    cin >> island_name;

    shared_ptr<Island> island_ptr = Model::get_instance().get_island_ptr(island_name);
    ship_ptr->set_unload_destination(island_ptr);
}

// Chain other Ships in the simulation world
void Controller::ship_chain_all_ship(shared_ptr<Ship_component> const ship_ptr) const
{
    ship_ptr->chain_all_ship();
}

// Chain a Ship to a Chain_ship so that when
// Chain_ship sets its destination or course,
// its chained Ships do the same.
// check : ship_ptr exists
void Controller::ship_chain_ship(shared_ptr<Ship_component> const ship_ptr) const
{
    string ship_name;
    cin >> ship_name;

    shared_ptr<Ship> ship_to_chain = Model::get_instance().get_ship_ptr(ship_name);

    if (!ship_to_chain)
        throw Error("Ship not found!");

    ship_ptr->chain_ship(ship_to_chain);
}

// Unchain a Ship from a Chain_ship.
// check: ship_ptr exists
void Controller::ship_unchain_ship(shared_ptr<Ship_component> const ship_ptr) const
{
    string ship_name;
    cin >> ship_name;

    shared_ptr<Ship> ship_to_unchain = Model::get_instance().get_ship_ptr(ship_name);

    if (!ship_to_unchain)
        throw Error("Ship not found!");

    ship_ptr->unchain_ship(ship_to_unchain);
}

// read an Island name to dock at.
// basic validity check : Island must exist
void Controller::ship_dock_at(shared_ptr<Ship_component> const ship_ptr) const
{
    string island_name;
    cin >> island_name;

    shared_ptr<Island> island_ptr = Model::get_instance().get_island_ptr(island_name);
    ship_ptr->dock(island_ptr);
}

// read a Ship name to attack.
// basic validity check : Ship must exist
void Controller::ship_attack(shared_ptr<Ship_component> const ship_ptr) const
{
    string ship_name;
    cin >> ship_name;

    shared_ptr<Ship> ship_target_ptr = Model::get_instance().get_ship_ptr(ship_name);

    if (!ship_target_ptr)
        throw Error("Ship not found!");

    ship_ptr->attack(ship_target_ptr);
}

// refuel a Ship
void Controller::ship_refuel(shared_ptr<Ship_component> const ship_ptr) const
{
    ship_ptr->refuel();
}

// stop a Ship
void Controller::ship_stop(shared_ptr<Ship_component> const ship_ptr) const
{
    ship_ptr->stop();
}

// stop a Ship from attacking
void Controller::ship_stop_attack(shared_ptr<Ship_component> const ship_ptr) const
{
    ship_ptr->stop_attack();
}

/*** Helper Functions ***/

void Controller::check_if_name_valid(const std::string& name) const
{
    if (name.length() < 2)
        throw Error("Invalid name for new object!");

    // If ship_name contains any character that is not
    // a digit or an alphabet, throw an Error.
    if (any_of(name.cbegin(), name.cend(), [](char c) { return !isdigit(c) && !isalpha(c); }))
        throw Error("Invalid name for new object!");

    // If ship_name is the same as one of the commands,
    // throw an Error.
    if (command_set.find(name) != command_set.cend())
        throw Error("Invalid name for new object!");
}

void Controller::process_ship_command(std::shared_ptr<Ship_component> const ptr)
{
    // If first_input is a Ship's name, read in a Ship command.
    string ship_command;
    cin >> ship_command;

    // If the second input is not a Ship command, throw an Error.
    auto ship_command_iter = ship_command_map.find(ship_command);

    if (ship_command_iter == ship_command_map.end())
        throw Error("Unrecognized command!");

    ship_command_iter->second(this, ptr);
}