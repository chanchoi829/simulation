#include "View.h"
#include "Utility.h"
#include <string>

using namespace std;

// Save the supplied name and location for future use in a draw() call
// If the name is already present,the new location replaces the previous one.
void View::update_location(const string& name, Point location)
{
    auto pair = object_info_map.insert(make_pair(name, location));
    if (!pair.second)
        pair.first->second = location;
}

// Remove the name and its location; no error if the name is not present.
void View::update_remove(const string& name)
{
    if (object_info_map.erase(name) == 0)
        return;
    ship_info_map.erase(name);
}

// Update information about a Ship's fuel
void View::ship_fuel_update(const std::string& name, double fuel)
{
    auto iter_bool = ship_info_map.insert(make_pair(name, Ship_info(fuel, 0, 0)));

    // If the Ship already exists, simply update
    // its information in the map.
    if (!iter_bool.second)
        iter_bool.first->second.fuel = fuel;
}

// Update information about a Ship's course
void View::ship_course_update(const std::string& name, double course)
{
    auto iter_bool = ship_info_map.insert(make_pair(name, Ship_info(0, course, 0)));

    // If the Ship already exists, simply update
    // its information in the map.
    if (!iter_bool.second)
        iter_bool.first->second.course = course;
}

// Update information about a Ship's speed
void View::ship_speed_update(const std::string& name, double speed)
{
    auto iter_bool = ship_info_map.insert(make_pair(name, Ship_info(0, 0, speed)));

    // If the Ship already exists, simply update
    // its information in the map.
    if (!iter_bool.second)
        iter_bool.first->second.speed = speed;
}

// Throw an Error because you cannot perform these functions
void View::set_size(int size_)
{
    throw Error("Cannot set size for this View!");
}

void View::set_scale(double scale_)
{
    throw Error("Cannot set scale for this View!");
}

void View::set_origin(Point origin_)
{
    throw Error("Cannot set origin for this View!");
}

void View::set_defaults()
{
    throw Error("Cannot set default for this View!");
}