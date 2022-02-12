/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Islands and Ships there are, but it does not
know about any of their derived classes, nor which Ships are of what kind of Ship. 
It has facilities for looking up objects by name, and removing Ships.  When
created, it creates an initial group of Islands and Ships using the Ship_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.
*/

#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

struct Point;
class Sim_object;
class Island;
class Ship_component;
class Ship;
class View;

class Model {
public:
	// Getters
	int get_time() const
		{return time;}

	std::map<std::string, std::shared_ptr<Island>> get_island_map() 
		{ return island_map;}

	std::map<std::string, std::shared_ptr<Ship>> get_ship_map()
		{return ship_map;}

	// Will throw Error("Island not found!") if no island of that name
	std::shared_ptr<Island> get_island_ptr(const std::string& name) const;

	// Returns nullptr if there is no ship of that name
	std::shared_ptr<Ship> get_ship_ptr(const std::string& name) const;

	std::shared_ptr<Ship_component> get_ship_composite_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();	
	
 	// Add a new ship to the containers, and update the view
	// Throws Error if there is already a Ship or Island with that name.
	// If insertion fails for any exception, the pointed-to Ship is deleted
	// and the exception rethrown.
	void add_ship(std::shared_ptr<Ship> new_ship);
    
	// Add a Ship_composite to ship_component_map
	// Throw an Error when the Ship_composite's name is a duplicate name
	void add_composite(std::shared_ptr<Ship_component> new_group);

	// Add a Ship to an existing Ship_composite
	// Throw an Error when composite or ship does not exist, or when ship is
	// being added to a Ship_composite whose top Ship_composite already
	// has the ship.
	void add_ship_to_composite(const std::string& composite, std::shared_ptr<Ship_component>ship);

	// Create a Ship_composite and add it to an existing Ship_composite.
	void add_composite_to_composite(const std::string& composite, std::shared_ptr<Ship_component>composite_in);

	// Remove a Ship_composite
	void remove_composite(const std::string& group_to_remove);

	// Find a Ship_composite with the given string
	// and remove the given Ship from the Ship_composite
	void remove_ship_from_composite(const std::string& composite, const std::string& ship_name);

	// Describe Ship_components in ship_component_map.
	// Indexing indicates the hierarchy
	// e.g.
	// Favorites
	//   Warships
	//      Cruisers
	//          Ajax
	//      Torpedoboats
	void describe_composite() const;

	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::shared_ptr<View>);
	
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);

	// notify the Views about a Ship's fuel
	void notify_view_about_ship_fuel(const std::string& name, double fuel) const;

	// notify the Views about a Ship's course
	void notify_view_about_ship_course(const std::string& name, double course) const;

	// notify the Views about a Ship's speed
	void notify_view_about_ship_speed(const std::string& name, double speed) const;

	// Remove a Ship from sim_object_map and ship_map
	void remove_ship(std::shared_ptr<Ship> ship_ptr);

	// Find a Ship_composite with the given name
	std::shared_ptr<Ship_component> find_composite_ptr(const std::string& name);

	// Check if name conflicts with another Island, Ship, or Ship_composite's name.
	// Throw an Error if it does.
	void check_if_name_duplicate(const std::string& name);

	// For Singleton
	static Model& get_instance();

	// disallow copy/move construction or assignment
	Model(Model& obj) = delete;
	Model(Model&& obj) = delete;
	Model& operator= (Model& obj) = delete;
	Model& operator= (Model&& obj) = delete;

private:
	// create the initial objects
	Model();
	~Model(){}

	int time;		// the simulated time

	std::map<std::string, std::shared_ptr<Sim_object>> sim_object_map;
	std::map<std::string, std::shared_ptr<Ship>>ship_map;
	std::map<std::string, std::shared_ptr<Island>>island_map;
	std::map<std::string, std::shared_ptr<Ship_component>>ship_component_map;
	std::set<std::string> ship_composite_names;

	std::vector<std::shared_ptr<View>>view_vec;
};

#endif