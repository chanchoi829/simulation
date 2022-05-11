/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <memory>
#include <set>
#include <functional>
#include <vector>

class Ship_component;
class View;

class Controller
{
public:
    // Initialize command maps
    Controller();

    // Run the program by acccepting user commands
    void run();

private:
    std::shared_ptr<View> map_view_ptr;
    std::shared_ptr<View> sailing_view_ptr;
    std::map<std::string, std::shared_ptr<View>> local_view_ptr_map;
    std::vector<std::shared_ptr<View>> view_vec;

    // Command maps
    std::map<std::string, std::function<void(Controller*, std::shared_ptr<Ship_component>)>> ship_command_map;
    std::map<std::string, std::function<void(Controller*)>> model_command_map;
    std::map<std::string, std::function<void(Controller*)>> view_command_map;

    // Set of commands to prevent creations of
    // ships with commands as their names.
    std::set<std::string> command_set;

    // View commands

    // create the View and attach to the Model.
    // If the View is already open, throw an Error.
    void open_map_view();

    // detach the View from the Model and destroy
    // it. If the View was not open, throw an Error.
    void close_map_view();

    // create and open the sailing data view. Error: sailing
    // data view is already open.
    void open_sailing_view();

    // close and destroy the sailing data view view.
    // Error: no sailing data view is open.
    void close_sailing_view();

    // create and open a local view centered on the ship
    // with name <name>.Errors in order of checks : no ship
    // with that name; local view is already open for that name.
    void open_local_view();

    // close and destroy the view with that name.
    // Error: local view is not open for that name.
    void close_local_view();

    // restore the default settings of the map.
    // If the View is not open, throw an Error.
    void view_default() const;

    // read a single integer for the size of
    // the map (number of both rows and columns).
    // If the View is not open, throw an Error.
    void view_size() const;

    // read a double value for the scale of the map
    // (number of nm per cell). If the View is not open, throw an Error.
    void view_zoom() const;

    // read a pair of double values for the (x, y) origin of the map.
    // If the View is not open, throw an Error.
    void view_pan() const;

    // tell the View to draw the map ? note that the Model and the objects
    // should have kept the View up to date by calling the relevant update
    // functions.If the View is not open, throw an Error.
    void view_show() const;

    // Model commands

    // have all the objects describe themselves; the output should be in
    // alphabetical order by name of all of the objects.
    void model_status() const;

    // call the Model::update() function
    void model_go() const;

    // create a new Ship using the supplied name, type name, and initial position.
    void model_create() const;

    // Create a Ship_composite and add it to the Model
    void model_create_composite() const;

    // Remove a Ship_composite from the Model
    void model_remove_composite() const;

    // Remove a Ship from a Ship_composite
    void model_remove_ship_from_composite() const;

    // Add a Ship to a Ship_composite
    void model_add_ship_to_composite() const;

    // Add a Ship_composite to a Ship_composite
    void model_add_composite_to_composite() const;

    // Describe a set of Ship_composites
    void model_describe_groups() const;

    // Ship commands

    // read a compass heading and a speed (both doubles) for the
    // Ship to set course and speed. basic validity check:
    // 0.0 <= compass heading < 360.0, speed >= 0.0
    void ship_course(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read an (x, y) position and then a speed (all doubles) for
    // the Ship to set destination position and speed to go to.
    // basic validity check : x, y can have any value, speed >= 0.0
    void ship_position(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read an Island name and a speed (a double) for the ship
    // to set destination Island and speed. basic validity check
    // : Island must exist, speed >= 0.0
    void ship_destination(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read an Island name to load at.
    // basic validity check : Island must exist
    void ship_load_at(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read an Island name to unload at.
    // basic validity check : Island must exist
    void ship_unload_at(std::shared_ptr<Ship_component> const ship_ptr) const;

    // Chain other Ships in the simulation world
    void ship_chain_all_ship(std::shared_ptr<Ship_component> const ship_ptr) const;

    // Chain a Ship to a Chain_ship so that when Chain_ship sets its destination or course,
    // its chained Ships do the same.
    // check : ship_ptr exists
    void ship_chain_ship(std::shared_ptr<Ship_component> const ship_ptr) const;

    // Unchain a Ship from a Chain_ship.
    // check: ship_ptr exists
    void ship_unchain_ship(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read an Island name to dock at.
    // basic validity check : Island must exist
    void ship_dock_at(std::shared_ptr<Ship_component> const ship_ptr) const;

    // read a Ship name to attack.
    // basic validity check : Ship must exist
    void ship_attack(std::shared_ptr<Ship_component> const ship_ptr) const;

    // refuel a Ship
    void ship_refuel(std::shared_ptr<Ship_component> const ship_ptr) const;

    // stop a Ship
    void ship_stop(std::shared_ptr<Ship_component> const ship_ptr) const;

    // stop a Ship from attacking
    void ship_stop_attack(std::shared_ptr<Ship_component> const ship_ptr) const;

    /*** Helper Functions ***/

    void check_if_name_valid(const std::string& name) const;

    void process_ship_command(std::shared_ptr<Ship_component> const ptr);
};

#endif