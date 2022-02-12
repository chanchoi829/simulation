#include "Ship_component_factory.h"
#include "Ship_composite.h"
#include "Chain_ship.h"
#include "Cruiser.h"
#include "Cruise_ship.h"
#include "Tanker.h"
#include "Torpedo_boat.h"
#include "Utility.h"
#include <memory>

using namespace std;

// Create a new Ship with the given name, type, and initial_possition
// may throw Error("Trying to create ship of unknown type!")
std::shared_ptr<Ship> create_ship(const string& name, 
	const string& type, Point initial_position) {
	if (type == "Tanker")
		return make_shared<Tanker>(name, initial_position);
	else if (type == "Cruiser")
		return make_shared<Cruiser>(name, initial_position);
	else if (type == "Cruise_ship")
		return make_shared<Cruise_ship>(name, initial_position);
	// There cannot be any other type of Ship, so throw an Error.
	else if (type == "Torpedo_boat")
		return make_shared<Torpedo_boat>(name, initial_position);
	else if (type == "Chain_ship")
		return make_shared<Chain_ship>(name, initial_position);
	else
		throw Error("Trying to create ship of unknown type!");
}

shared_ptr<Ship_component> create_composite(const string& name) {
	return make_shared<Ship_composite>(name);
}