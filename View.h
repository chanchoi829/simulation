/*
View class ***
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

Usage:
1. Call the update_location function with the name and position of each object
to be plotted. If the object name is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location.

2. Call the update_remove function with the name of any object that should
no longer be plotted. This must be done *after* any call to update_location that
has the same object name since update_location will add any object name supplied.

3. Call the draw function to print out the map.

4. As needed, change the origin, scale, or displayed size of the map
with the appropriate functions. Since the view "remembers" the previously updated
information, immediately calling the draw function will print out a map showing
the previous objects using the new settings.
*/

#ifndef VIEW_H
#define VIEW_H

#include "Geometry.h"
#include <map>
#include <string>
#include <vector>

class View {
public:
	virtual ~View(){}

	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	virtual void update_location(const std::string& name, Point location);

	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name);

	// Update information about a Ship's fuel
	virtual void ship_fuel_update(const std::string& name, double fuel);
	
	// Update information about a Ship's course
	virtual void ship_course_update(const std::string& name, double course);

	// Update information about a Ship's speed
	virtual void ship_speed_update(const std::string& name, double speed);

	// prints out the current map
	virtual void draw() = 0;

	// Throw an Error because you cannot perform these functions.
	virtual void set_size(int size_);

	virtual void set_scale(double scale_);

	virtual void set_origin(Point origin_);

	virtual void set_defaults();

protected:
	struct Ship_info {
		Ship_info(double fuel_, double course_, double speed_)
			: fuel(fuel_), course(course_), speed(speed_) {}
		double fuel;
		double course;
		double speed;
	};

	std::map<std::string, Point> get_object_info_map() const
		{return object_info_map;}

	std::map<std::string, Ship_info> get_ship_info_map() const
		{return ship_info_map;}

private:
	std::map<std::string, Point> object_info_map;
	std::map<std::string, Ship_info>ship_info_map;
};

#endif
