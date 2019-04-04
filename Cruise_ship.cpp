#include "Cruise_ship.h"
#include "Island.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>
#include <limits>

using namespace std;

Cruise_ship::Cruise_ship(const string& name_, Point position_)
	: Ship(name_, position_, 500, 15.0, 2.0, 0),
	visited_vec(Model::get_instance().get_island_map().size(), false),
	state(Cruise_ship_state::not_cruising), island_visited(0) {}

// Update the state of Cruise_ship
void Cruise_ship::update() {
	Ship::update();

	switch (state) {
		// Do not do anything when it is not cruising.
	case Cruise_ship_state::not_cruising:
		break;

		// If Cruise_ship has just set a destination island for cruising and
		// can dock there, dock there. 
	case Cruise_ship_state::set_cruise:
		if (can_dock(island_to_visit)) {
			dock(island_to_visit);
			++island_visited;
			state = Cruise_ship_state::just_docked;
		}
		break;

		// If Cruise_ship has just docked, refuel.
	case Cruise_ship_state::just_docked:
		refuel();
		state = Cruise_ship_state::refuel;
		break;

		// If Cruise_ship has just refueled, make it wait
	case Cruise_ship_state::refuel:
		state = Cruise_ship_state::waiting;
		break;

	case Cruise_ship_state::waiting: {
		// When Cruise_ship has visited all of the islands,
		// go back to the starting Island.
		if (island_visited == (int)Model::get_instance()
			.get_island_map().size()) {
			Ship::set_destination_island_and_speed
			(starting_island, starting_speed);

			cout << get_name() << " will visit "
				<< starting_island->get_name() << endl;

			island_to_visit = starting_island;
			state = Cruise_ship_state::going_back;
			return;
		}

		map<string, shared_ptr<Island>> island_temp_map
			= Model::get_instance().get_island_map();
		double lowest_distance = numeric_limits<double>::max();

		// island_index indicates which index in visited_vec
		// corresponds to the Island.
		int island_index;
		int i = 0;

		// Loop for finding an island closest to the current location.
		for (const auto& island : island_temp_map) {
			// If the Island has been visited already, skip rest of the for loop.
			if (visited_vec[i]) {
				++i;
				continue;
			}

			double distance_from_island = cartesian_distance(get_location(),
				island.second->get_location());

			// If the new distance is lower than the current lowest distance,
			// set the new distance as the lowest distance and remember
			// the index of the island in the visited_vec
			if (distance_from_island < lowest_distance) {
				island_to_visit = island.second;
				lowest_distance = distance_from_island;
				island_index = i;
			}
			++i;
		}

		// Mark the Island as visited and set destination
		// to the Island.
		visited_vec[island_index] = true;
		Ship::set_destination_island_and_speed
		(island_to_visit, starting_speed);
		state = Cruise_ship_state::set_cruise;

		cout << get_name() << " will visit "
			<< island_to_visit->get_name() << endl;
		break;
	}

	// When Cruise_ship is going back to the starting Island
	// and can dock there, dock there and set its state to
	// not cruising.
	case Cruise_ship_state::going_back:
		if (can_dock(starting_island)) {
			dock(starting_island);
			cout << get_name() << " cruise is over at "
				<< starting_island->get_name() << endl;
			state = Cruise_ship_state::not_cruising;
		}
		break;
	}
}

// Output a description of current state to cout
void Cruise_ship::describe() const {
	cout << "\nCruise_ship ";
	Ship::describe();

	switch (state) {
	case Cruise_ship_state::not_cruising:
		break;
	case Cruise_ship_state::set_cruise:
	case Cruise_ship_state::going_back:
		cout << "On cruise to "
			<< island_to_visit->get_name() << endl;
		break;
	case Cruise_ship_state::just_docked:
	case Cruise_ship_state::refuel:
	case Cruise_ship_state::waiting:
		cout << "Waiting during cruise at " <<
			island_to_visit->get_name() << endl;
		break;
	default:
		throw Error("Unrecognized state!");
	}
}

// When Cruise_ship is cruising, cancel it.
void Cruise_ship::set_destination_position_and_speed
(Point destination_point, double speed) {
	if (state != Cruise_ship_state::not_cruising)
		cancel_cruise_and_print();
	Ship::set_destination_position_and_speed(destination_point, speed);
}

// When Cruise_ship was initially cruising, cancel the cruise and a new
// Island destination. Otherwise, set a new destination and set up a 
// cruise to the Islands.
void Cruise_ship::set_destination_island_and_speed
(shared_ptr<Island> destination_island, double speed) {
	bool initially_cruising = false;
	if (state != Cruise_ship_state::not_cruising) {
		cancel_cruise_and_print();
		initially_cruising = true;
	}

	Ship::set_destination_island_and_speed(destination_island, speed);

	cout << get_name() << " will visit "
		<< destination_island->get_name() << endl;

	cout << get_name() << " cruise will start and end at "
		<< destination_island->get_name() << endl;

	// If Cruise_ship was initially cruising, do nothing further.
	if (initially_cruising)
		return;

	state = Cruise_ship_state::set_cruise;
	island_to_visit = destination_island;
	starting_island = destination_island;
	starting_speed = speed;

	int counter = 0;
	map<string, shared_ptr<Island>> island_temp_map
		= Model::get_instance().get_island_map();

	// Find and mark Island as visited
	for (const auto& island : island_temp_map) {
		if (island.second == destination_island) {
			visited_vec[counter] = true;
			break;
		}
		++counter;
	}
}

// Cancel cruise if Cruise_ship was cruising
void Cruise_ship::set_course_and_speed(double course, double speed) {
	if (state != Cruise_ship_state::not_cruising)
		cancel_cruise_and_print();
	Ship::set_course_and_speed(course, speed);
}

// Cancel cruise if Cruise_ship was cruising
void Cruise_ship::stop() {
	if (state != Cruise_ship_state::not_cruising)
		cancel_cruise_and_print();
	Ship::stop();
}

// Helper function for canceling cruise, resetting variables
// used for cruising, and printing a message.
void Cruise_ship::cancel_cruise_and_print() {
	island_visited = 0;
	state = Cruise_ship_state::not_cruising;
	visited_vec = vector<bool>
		(Model::get_instance().get_island_map().size(), false);
	cout << get_name() << " canceling current cruise" << endl;
}