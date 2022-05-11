
/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract,
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/

#include "Ship_component.h"
#include "Model.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>

using namespace std;

// Return an index counter which is used to indicate
// in which level a Ship_component exists in a group hierarchy.
// e.g. when index_count is 0, Ship_component is a top group, and when
// index_count is 1, it is right below a top group.
int& Ship_component::index_counter()
{
    static int index_count = 0;
    return index_count;
}

// Always return a nullptr
shared_ptr<Ship_component> Ship_component::find_ship_composite(const string& name)
{
    return nullptr;
}

// Always return false
bool Ship_component::find_and_remove_ship_composite(const string& name)
{
    return false;
}

// Always return false
bool Ship_component::check_if_ship_exists(const string& ship) const
{
    return false;
}

/*** Fat Interface Functioins ***/

// Every function below always throws an Error.
// These functions are not meant to be called.
void Ship_component::add_component(shared_ptr<Ship_component>)
{
    throw Error("Cannot process this command!");
}

void Ship_component::remove_component(shared_ptr<Ship_component>)
{
    throw Error("Cannot process this command!");
}

void Ship_component::describe_component() const
{
    throw Error("Cannot process this command!");
}

string Ship_component::get_name() const
{
    throw Error("Cannot process this command!");
}

void Ship_component::set_destination_position_and_speed(Point destination_position, double speed)
{
    throw Error("Cannot process this command!");
}

void Ship_component::set_destination_island_and_speed(shared_ptr<Island> destination_island, double speed)
{
    throw Error("Cannot process this command!");
}

void Ship_component::set_course_and_speed(double course, double speed)
{
    throw Error("Cannot process this command!");
}

void Ship_component::stop()
{
    throw Error("Cannot process this command!");
}

void Ship_component::dock(shared_ptr<Island> island_ptr)
{
    throw Error("Cannot process this command!");
}

void Ship_component::refuel()
{
    throw Error("Cannot process this command!");
}

void Ship_component::chain_all_ship()
{
    throw Error("Cannot chain Ship!");
}

void Ship_component::chain_ship(shared_ptr<Ship>)
{
    throw Error("Cannot chain Ship!");
}

void Ship_component::unchain_ship(shared_ptr<Ship>)
{
    throw Error("Cannot drop Ship!");
}

void Ship_component::set_load_destination(shared_ptr<Island>)
{
    throw Error("Cannot load at a destination!");
}

void Ship_component::set_unload_destination(shared_ptr<Island>)
{
    throw Error("Cannot unload at a destination!");
}

void Ship_component::attack(shared_ptr<Ship>)
{
    throw Error("Cannot attack!");
}

void Ship_component::stop_attack()
{
    throw Error("Cannot attack!");
}