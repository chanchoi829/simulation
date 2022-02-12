#include "Torpedo_boat.h"
#include "Island.h"
#include "Model.h"
#include <iostream>
#include <limits>
#include <map>

using namespace std;


Torpedo_boat::Torpedo_boat(const string& name_, Point position_)
	: Warship(name_, position_, 3, 5.0, 800, 12, 5, 9) {}

// When target is out of range this Torpedo_boat can move,
// set the target's loation as the destination.
void Torpedo_boat::update() {
	Warship::update();

	if (target_out_of_range() && can_move())
		set_destination_position_and_speed
		(get_target().lock()->get_location(), get_maximum_speed());
}

// Describe this Torpedo_boat's state.
void Torpedo_boat::describe() const {
	cout << "\nTorpedo_boat ";
	Warship::describe();
}

// Perform Torpedo_boat specific behavior for receive_hit
void Torpedo_boat::receive_hit
(int hit_force, std::shared_ptr<Ship> attacker_ptr) {
	Ship::receive_hit(hit_force, attacker_ptr);

	if (!can_move())
		return;

	cout << get_name() << " taking evasive action" << endl;

	if (is_attacking())
		stop_attack();

	map<string, shared_ptr<Island>>island_map =
		Model::get_instance().get_island_map();

	shared_ptr<Island> closest_island;
	double lowest_distance = numeric_limits<double>::max();

	// Find an Island closest to the attacker. The distance
	// has to be greater than equal to 15nm.
	for (const auto& pair : island_map) {
		double distance = cartesian_distance
		(attacker_ptr->get_location(), pair.second->get_location());
		if (distance < lowest_distance && distance >= 15) {
			closest_island = pair.second;
			lowest_distance = distance;
		}
	}

	// If such an Island was found, set the destination to that
	// Island.
	if (closest_island) {
		set_destination_island_and_speed
		(closest_island, get_maximum_speed());
	}
	else {
		double longest_distance = 0;
		double distance;
		shared_ptr<Island> farthest_island;

		// If such an Island was not found, set destination to the an Island
		// that is farthest from the attacker.
		for (const auto& pair : island_map) {
			distance = cartesian_distance
			(attacker_ptr->get_location(), pair.second->get_location());
			if (distance > longest_distance) {
				farthest_island = pair.second;
				longest_distance = distance;
			}
		}
		set_destination_island_and_speed(farthest_island, get_maximum_speed());
	}
}