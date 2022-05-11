#include "Model.h"
#include "Sim_object.h"
#include "Island.h"
#include "Ship.h"
#include "View.h"
#include "Geometry.h"
#include "Ship_component_factory.h"
#include "Utility.h"
#include <algorithm>
#include <iostream>

using namespace std;

// create the initial objects
Model::Model()
    : time(0)
{
    // first insert Islands into island_map and insert the returned iterator
    // from .insert() back into sim_object_map
    sim_object_map.insert(
        *island_map.insert(make_pair("Exxon", make_shared<Island>("Exxon", Point(10, 10), 1000, 200))).first);
    sim_object_map.insert(
        *island_map.insert(make_pair("Shell", make_shared<Island>("Shell", Point(0, 30), 1000, 200))).first);
    sim_object_map.insert(
        *island_map.insert(make_pair("Bermuda", make_shared<Island>("Bermuda", Point(20, 20)))).first);
    sim_object_map.insert(
        *island_map.insert(make_pair("Treasure_Island", make_shared<Island>("Treasure_Island", Point(50, 5), 100, 5)))
             .first);

    // first insert Ship into ship_map and insert the returned iterator
    // from .insert() back into sim_object_map
    sim_object_map.insert(*ship_map.insert(make_pair("Ajax", create_ship("Ajax", "Cruiser", Point(15, 15)))).first);
    sim_object_map.insert(*ship_map.insert(make_pair("Xerxes", create_ship("Xerxes", "Cruiser", Point(25, 25)))).first);
    sim_object_map.insert(*ship_map.insert(make_pair("Valdez", create_ship("Valdez", "Tanker", Point(30, 30)))).first);
}

// Will throw Error("Island not found!") if no island of that name
shared_ptr<Island> Model::get_island_ptr(const string& name) const
{
    auto iter_found = island_map.find(name);

    if (iter_found == island_map.cend())
        throw Error("Island not found!");

    return iter_found->second;
}

// Returns nullptr if there is no ship of that name
shared_ptr<Ship> Model::get_ship_ptr(const string& name) const
{
    auto iter_found = ship_map.find(name);

    if (iter_found == ship_map.cend())
        return nullptr;

    return iter_found->second;
}

// Find a Ship_composite with the given name from ship_component_map
shared_ptr<Ship_component> Model::get_ship_composite_ptr(const string& name) const
{
    shared_ptr<Ship_component> ship_composite;
    for (const auto& pair : ship_component_map) {
        ship_composite = pair.second->find_ship_composite(name);
        if (ship_composite)
            return ship_composite;
    }
    return ship_composite;
}

// tell all objects to describe themselves
void Model::describe() const
{
    for_each(sim_object_map.cbegin(), sim_object_map.cend(), [](const auto& map_pair) { map_pair.second->describe(); });
}

// increment the time, and tell all objects to update themselves
void Model::update()
{
    for_each(sim_object_map.cbegin(), sim_object_map.cend(), [](const auto& map_pair) { map_pair.second->update(); });

    ++time;
}

// Add a new ship to the containers, and update the view
// Throws Error if there is already a Ship or Island with that name.
// If insertion fails for any exception, the pointed-to Ship is deleted
// and the exception rethrown.
void Model::add_ship(shared_ptr<Ship> new_ship)
{
    check_if_name_duplicate(new_ship->get_name());

    sim_object_map.insert(make_pair(new_ship->get_name(), new_ship));
    ship_map.insert(make_pair(new_ship->get_name(), new_ship));

    // Notify View about the new Ship.
    new_ship->broadcast_current_state();
    new_ship->broadcast_ship_fuel();
    new_ship->broadcast_ship_course();
    new_ship->broadcast_ship_speed();
}

// Add a Ship_composite to ship_component_map
// Throw an Error when the Ship_composite's name is a duplicate name
void Model::add_composite(shared_ptr<Ship_component> new_group)
{
    check_if_name_duplicate(new_group->get_name());

    ship_component_map.insert(make_pair(new_group->get_name(), new_group));
    ship_composite_names.insert(new_group->get_name());

    cout << "Group " << new_group->get_name() << " added" << endl;
}

// Add a Ship to an existing Ship_composite
// Throw an Error when composite or ship does not exist, or when ship is
// being added to a Ship_composite whose top Ship_composite already
// has the ship.
void Model::add_ship_to_composite(const string& composite, shared_ptr<Ship_component> ship)
{
    // top_ship_composite is a Ship_composite who is at the very top
    // of a Ship_composite hierarchy.
    shared_ptr<Ship_component> top_ship_composite;

    // leaf_ship_composite is a Ship_composite which is under
    // a top Ship_ composite
    shared_ptr<Ship_component> leaf_ship_composite;

    // find the top Ship_composite which contains a Ship_composite with
    // the given string, composite, and also find a Ship_composite
    // with the given string
    for (const auto& pair : ship_component_map) {
        leaf_ship_composite = pair.second->find_ship_composite(composite);
        if (leaf_ship_composite) {
            top_ship_composite = pair.second;
            break;
        }
    }

    if (!top_ship_composite)
        throw Error("Group does not exist!");

    if (!ship)
        throw Error("Ship not found!");

    if (top_ship_composite->check_if_ship_exists(ship->get_name()))
        throw Error("Cannot add the same Ship under the same group!");

    leaf_ship_composite->add_component(ship);
    cout << ship->get_name() << " added to " << leaf_ship_composite->get_name() << endl;
}

// Create a Ship_composite and add it to an existing Ship_composite.
void Model::add_composite_to_composite(const string& composite_name, shared_ptr<Ship_component> composite_in)
{
    shared_ptr<Ship_component> composite_ptr = find_composite_ptr(composite_name);

    if (!composite_ptr)
        throw Error("Group does not exist!");

    check_if_name_duplicate(composite_in->get_name());

    composite_ptr->add_component(composite_in);
    ship_composite_names.insert(composite_in->get_name());
    cout << composite_in->get_name() << " added to " << composite_ptr->get_name() << endl;
}

// Remove a Ship_composite
void Model::remove_composite(const string& group_to_remove)
{
    auto it = ship_component_map.cbegin();
    while (it != ship_component_map.cend()) {
        // If the iterator, it, matches group_to_remove,
        // remove it.
        if (it->second->get_name() == group_to_remove) {
            ship_component_map.erase(it);
            cout << group_to_remove << " removed" << endl;
            ship_composite_names.erase(group_to_remove);
            return;
        } else {
            // If the iterator, it, does not match group_to_remove,
            // search the iterator's container to find group_to_remove
            // and erase it.
            if (it->second->find_and_remove_ship_composite(group_to_remove)) {
                cout << group_to_remove << " removed" << endl;
                ship_composite_names.erase(group_to_remove);
                return;
            }
            ++it;
        }
    }
    throw Error("Group does not exist!");
}

// Find a Ship_composite with the given string
// and remove the given Ship from the Ship_composite
void Model::remove_ship_from_composite(const string& composite, const string& ship_name)
{
    shared_ptr<Ship_component> composite_ptr = find_composite_ptr(composite);

    if (!composite_ptr)
        throw Error("Group not found!");

    auto it = ship_map.find(ship_name);

    if (it == ship_map.cend())
        throw Error("Ship not found!");

    composite_ptr->remove_component(it->second);
    cout << it->second->get_name() << " removed from " << composite_ptr->get_name() << endl;
}

// Describe Ship_components in ship_component_map.
// Indexing indicates the hierarchy
// e.g.
// Favorites
//   Warships
//      Cruisers
//          Ajax
//      Torpedoboats
void Model::describe_composite() const
{
    if (ship_component_map.empty())
        cout << "There is currently no group" << endl;

    for (const auto& pair : ship_component_map)
        pair.second->describe_component();
}

/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(shared_ptr<View> view_ptr)
{
    view_vec.push_back(view_ptr);

    // Notify View about every Sim_object in sim_object_map.
    for_each(sim_object_map.cbegin(), sim_object_map.cend(), [](const auto& map_pair) {
        map_pair.second->broadcast_current_state();
    });

    // Notify View about Ships' information
    for_each(ship_map.cbegin(), ship_map.cend(), [](const auto& map_pair) {
        map_pair.second->broadcast_ship_fuel();
        map_pair.second->broadcast_ship_course();
        map_pair.second->broadcast_ship_speed();
    });
}
// Detach the View by discarding the supplied pointer from the container
// of Views - no updates sent to it thereafter.
void Model::detach(shared_ptr<View> view_ptr)
{
    // Find and erase view_ptr from view_vec
    auto it = find(view_vec.cbegin(), view_vec.cend(), view_ptr);

    if (it == view_vec.cend())
        return;

    view_vec.erase(it);
}

// notify the views about an object's location
void Model::notify_location(const string& name, Point location)
{
    for_each(view_vec.cbegin(), view_vec.cend(), [&](shared_ptr<View> ptr) { ptr->update_location(name, location); });
}
// notify the views that an object is now gone
void Model::notify_gone(const string& name)
{
    for_each(view_vec.cbegin(), view_vec.cend(), [&](shared_ptr<View> ptr) { ptr->update_remove(name); });
}

// notify the Views about a Ship's fuel
void Model::notify_view_about_ship_fuel(const string& name, double fuel) const
{
    for (auto ptr : view_vec)
        ptr->ship_fuel_update(name, fuel);
}

// notify the Views about a Ship's course
void Model::notify_view_about_ship_course(const std::string& name, double course) const
{
    for (auto ptr : view_vec)
        ptr->ship_course_update(name, course);
}

// notify the Views about a Ship's speed
void Model::notify_view_about_ship_speed(const std::string& name, double speed) const
{
    for (auto ptr : view_vec)
        ptr->ship_speed_update(name, speed);
}

// Remove a Ship from sim_object_map and ship_map
void Model::remove_ship(shared_ptr<Ship> ship_ptr)
{
    sim_object_map.erase(ship_ptr->get_name());
    ship_map.erase(ship_ptr->get_name());
}

/*** Helper Functions ***/

// Find a Ship_composite with the given name
shared_ptr<Ship_component> Model::find_composite_ptr(const string& name)
{
    shared_ptr<Ship_component> composite_ptr;
    for (const auto& pair : ship_component_map) {
        composite_ptr = pair.second->find_ship_composite(name);
        if (composite_ptr)
            break;
    }
    return composite_ptr;
}

// Check if name conflicts with another Island, Ship, or Ship_composite's name.
// Throw an Error if it does.
void Model::check_if_name_duplicate(const string& name)
{
    if (island_map.find(name) != island_map.cend())
        throw Error("New object has duplicate name!");

    if (ship_map.find(name) != ship_map.cend())
        throw Error("New object has duplicate name!");

    if (ship_composite_names.find(name) != ship_composite_names.cend())
        throw Error("New object has duplicate name!");
}

// for Singleton
Model& Model::get_instance()
{
    static Model the_model;
    return the_model;
}