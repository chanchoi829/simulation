#include "Ship_composite.h"
#include "Model.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>
#include <map>

using namespace std;

Ship_composite::Ship_composite(string composite_name_) 
	: composite_name(composite_name_){}

// Add a new Ship_component to ship_components map
void Ship_composite::add_component
(shared_ptr<Ship_component> new_ship_component) {
	if (!ship_components.insert
	(make_pair(new_ship_component->get_name(), new_ship_component)).second)
		throw Error("This Ship_composite already contains the Ship_component!");
}

// Remove a Ship_component from ship_components
void Ship_composite::remove_component
(shared_ptr<Ship_component> ship_component) {
	if (ship_components.erase(ship_component->get_name()) == 0)
		throw Error("This Ship_composite does not contain the Ship_component!");
}

// Find Ship_composite from ship_components
shared_ptr<Ship_component> Ship_composite::
find_ship_composite(const string& name) {
	if (name == composite_name)
		return shared_from_this();

	for (const auto& pair : ship_components) {
		shared_ptr<Ship_component> ptr 
			= pair.second->find_ship_composite(name);
		if (ptr)
			return ptr;
	}
	return nullptr;
}

// Find Ship_composite and remove it
bool Ship_composite::find_and_remove_ship_composite(const string& name) {
	if (ship_components.erase(name) == 1)
		return true;
	for (const auto& pair : ship_components)
		if (pair.second->find_and_remove_ship_composite(name))
			return true;
	return false;
}

// Return a bool indicating if a Ship exists in ship_components
bool Ship_composite::check_if_ship_exists(const string& ship) const{
	for (const auto& pair : ship_components) {
		bool exists = pair.second->check_if_ship_exists(ship);
		if (exists)
			return true;
	}
	return false;
}

// Describe ship_components 
void Ship_composite::describe_component() const {
	for (int i = 0; i < Ship_component::index_counter(); ++i)
		cout << "   ";
	cout << "Group " << composite_name;

	if (ship_components.empty()) {
		cout << " is empty" << endl;
		return;
	}
	
	cout << " contains:" << endl;
	// Increment to indicate we are going 1 level down
	// in group hierarchy
	++Ship_component::index_counter();
	for (const auto& pair : ship_components)
		pair.second->describe_component();

	// Decrement to be a level up.
	--Ship_component::index_counter();
}

string Ship_composite::get_name() const{
	return composite_name;
}

/*** Command functions ***/
// Start moving to a destination position at a speed
void Ship_composite::set_destination_position_and_speed
(Point destination_position, double speed) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->set_destination_position_and_speed
			(destination_position, speed);
		}
		catch (Error&) {}
	}
}
// Start moving to a destination Island at a speed
void Ship_composite::set_destination_island_and_speed
(shared_ptr<Island> destination_island, double speed) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->set_destination_island_and_speed
			(destination_island, speed);
		}
		catch (Error&) {}
	}
}
// Start moving on a course and speed
void Ship_composite::set_course_and_speed(double course, double speed) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->set_course_and_speed(course, speed);
		}
		catch (Error&) {}
	}
}
// Stop moving
void Ship_composite::stop() {
	for (const auto& pair : ship_components) {
		try {
			pair.second->stop();
		}
		catch (Error&) {}
	}
}
// dock at an Island - set our position = Island's position, 
void Ship_composite::dock(shared_ptr<Island> island_ptr) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->dock(island_ptr);
		}
		catch (Error&) {}
	}
}
// Refuel - must already be docked at an island; fill takes as much as possible
void Ship_composite::refuel() {
	for (const auto& pair : ship_components) {
		try {
			pair.second->refuel();
		}
		catch (Error&) {}
	}
}

// Tell a Chain_ships to chain other Ships in the simulation world
void Ship_composite::chain_all_ship() {
	for (const auto& pair : ship_components) {
		try {
			pair.second->chain_all_ship();
		}
		catch (Error&){}
	}
}

// Tell Chain_ships to chain a Ship
void Ship_composite::chain_ship(shared_ptr<Ship> ship_to_chain) {
	for (const auto& pair : ship_components){
		try {
			pair.second->chain_ship(ship_to_chain);
		}
		catch (Error&) {}
	}
}

// Tell Chain_ships to unchain a Ship
void Ship_composite::unchain_ship(shared_ptr<Ship> ship_to_unchain) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->unchain_ship(ship_to_unchain);
		}
		catch (Error&) {}
	}
}

// Tell Tankers to set their load destination
void Ship_composite::set_load_destination(shared_ptr<Island> load_destination) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->set_load_destination(load_destination);
		}
		catch (Error&) {}
	}
}

// Tell Tankers to set their unload destination
void Ship_composite::set_unload_destination(shared_ptr<Island> unload_destination) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->set_unload_destination(unload_destination);
		}
		catch (Error&) {}
	}
}

// Tell Warships to attack a target
void Ship_composite::attack(shared_ptr<Ship> target) {
	for (const auto& pair : ship_components) {
		try {
			pair.second->attack(target);
		}
		catch (Error&) {}
	}
}

// Tell Warships to stop attacking
void Ship_composite::stop_attack() {
	for (const auto& pair : ship_components) {
		try {
			pair.second->stop_attack();
		}
		catch (Error&) {}
	}
}