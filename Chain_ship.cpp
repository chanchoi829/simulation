#include "Chain_ship.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>
#include <limits>

using namespace std;

Chain_ship::Chain_ship(const string& name_, Point position_)
	: Ship(name_, position_, 1500, 10.0, 4.0, 1) , 
	state(State::not_moving_to_chain_ship){}

// Chain every other Ship in the simulation world.
void Chain_ship::chain_all_ship() {
	if (state != State::not_moving_to_chain_ship)
		cout << get_name() << " canceling its current plan to chain " 
		<< ship_to_chain->get_name() << endl;
	
	map_of_ship_to_chain = Model::get_instance().get_ship_map();

	// While loop for erasing Ships which are already chained
	// to this Chain_ship. Also erases this Chain_ship from
	// the map because it doesn't need to chain itself.
	auto it = map_of_ship_to_chain.cbegin();
	while (it != map_of_ship_to_chain.cend()) {
		if (it->second->get_name() == get_name()) {
			map_of_ship_to_chain.erase(it++);
		}
		else {
			// Bool for indicating if we need to
			// increment the iterator, it.
			bool erased = false;
			for (const auto& pair : chained_ship) {
				if (it->second->get_name() == 
					pair.second->get_name()) {
					map_of_ship_to_chain.erase(it++);
					erased = true;
					break;
				}
			}
			if (!erased)
				++it;
		}
	}

	if (map_of_ship_to_chain.empty()) {
		state = State::not_moving_to_chain_ship;
		throw Error("There is no Ship to chain!");
	}

	state = State::moving_to_chain_all_ship;

	num_of_ship_needed_to_chain = map_of_ship_to_chain.size();
	num_of_ship_chained = 0;
	find_closest_ship_to_chain();
}

// Makes the target Ship stop and sets this Chain_ship's destination to the
// target's location.
// Throws an Error when target_to_chain is already chained to this Ship.
void Chain_ship::chain_ship(shared_ptr<Ship>target_to_chain) {
	if (chained_ship.find(target_to_chain->get_name()) != chained_ship.cend())
		throw Error("Chain_ship already contains this Ship!");

	if (state != State::not_moving_to_chain_ship) {
		cout << get_name() << " canceling its current plan to chain "
			<< ship_to_chain->get_name() << endl;
		map_of_ship_to_chain.clear();
	}

	target_to_chain->stop();
	ship_to_chain = target_to_chain;
	state = State::moving_to_chain_ship;

	// Remember the location of Ship to chain so that when it changes,
	// this Chain_ship can set_destination_position_and_speed to the
	// new location.
	location_of_ship_to_chain = ship_to_chain->get_location();
	set_destination_position_and_speed
	(location_of_ship_to_chain, get_maximum_speed());
}

// Unchain a Ship.
// Throws an Error ship_to_unchain is not chained to this Ship.
void Chain_ship::unchain_ship(shared_ptr<Ship>ship_to_drop) {
	if (chained_ship.erase(ship_to_drop->get_name()) == 0)
		throw Error("Chain_ship does not contain this Ship!");
	cout << ship_to_drop->get_name() << 
		" unchained from " << get_name() << endl;
}

// Update the state of Chain_ship
void Chain_ship::update() {
	Ship::update();

	// While loop for automatically unchaining Ships
	// which cannot move
	auto it = chained_ship.cbegin();
	while (it != chained_ship.cend()) {
		if (!it->second->can_move()) {
			cout << it->second->get_name() 
				<< " unchained from " << get_name() << endl;

			if (it->second == ship_to_chain)
				ship_to_chain.reset();
			chained_ship.erase(it++);
		}
		else
			++it;
	}

	// While loop for automatically erasing Ships from
	// the map of Ships to chain if they are no longer
	// afloat.
	it = map_of_ship_to_chain.cbegin();
	while (it != map_of_ship_to_chain.cend()) {
		if (!it->second->is_afloat()) {
			map_of_ship_to_chain.erase(it++);

			// decrement number of Ships needed to chain
			// to indicate we need one fewer Ship to chain
			// to chain all Ships.
			--num_of_ship_needed_to_chain;
		}
		else
			++it;
	}

	switch (state) {
		case State::not_moving_to_chain_ship:
			break;

		case State::moving_to_chain_ship:
			if (!ship_to_chain->is_afloat()) {
				cout << get_name() << "'s Ship to chain is not afloat anymore" << endl;
				stop();
				state = State::not_moving_to_chain_ship;
			}
			// When ship_to_chain is in range of this Chain_ship, chain ship_to_chain
			else if (cartesian_distance(get_location(), ship_to_chain->get_location()) < 0.1) {
				cout << ship_to_chain->get_name() << " chained to " << get_name() << endl;
				chained_ship.insert(make_pair(ship_to_chain->get_name(), ship_to_chain));
				state = State::not_moving_to_chain_ship;
			}
			// When the location of the Ship to chain has changed, set_destination
			// _position and speed to the Ship's new location.
			else if (location_of_ship_to_chain != ship_to_chain->get_location()) {
				set_destination_position_and_speed
				(ship_to_chain->get_location(), get_maximum_speed());
				location_of_ship_to_chain = ship_to_chain->get_location();
			}
			break;
		case State::moving_to_chain_all_ship:
			// Case when this Chain_ship has chained all Ships
			if (num_of_ship_chained == num_of_ship_needed_to_chain) {
				cout << get_name() << " has chained all Ships" << endl;
				state = State::not_moving_to_chain_ship;
			}

			// When ship_to_chain is in range of this Chained_ship, chain the ship and find
			// next closest Ship to chain if this Chained_ship must find more Ships to chain.
			else if (cartesian_distance(get_location(), ship_to_chain->get_location()) < 0.1) {
				cout << ship_to_chain->get_name() << " chained to " << get_name() << endl;
				chained_ship.insert(make_pair(ship_to_chain->get_name(), ship_to_chain));
				++num_of_ship_chained;
				ship_to_chain.reset();

				if (num_of_ship_chained != num_of_ship_needed_to_chain)
					find_closest_ship_to_chain();
			}
			// Update the destination when location of ship_to_chain changes
			else if (location_of_ship_to_chain != ship_to_chain->get_location()) {
				set_destination_position_and_speed
				(ship_to_chain->get_location(), get_maximum_speed());
				location_of_ship_to_chain = ship_to_chain->get_location();
			}
			break;

		default:
			throw Error("Chain_ship unrecognized state!");
			break;
	}
}

// Output a description of current state to cout
void Chain_ship::describe() const {
	cout << "\nChain_ship ";
	Ship::describe();

	if (chained_ship.empty()) {
		cout << get_name() << " currently has no Ship chained" << endl;
		return;
	}

	cout << get_name() << " currently chains: " << endl;

	// Print what Ships are chained to this Chain_ship
	bool print_comma = false;
	for (const auto& pair : chained_ship) {
		if (print_comma)
			cout << ", ";
		cout << pair.second->get_name();
		print_comma = true;
	}

	cout << endl;
}

// Command this Ship's chained Ships to also set_destination_position_and_speed
void Chain_ship::set_destination_position_and_speed
(Point destination_point, double speed) {
	Ship::set_destination_position_and_speed(destination_point, speed);
	for (const auto& pair : chained_ship)
		pair.second->Ship::
		set_destination_position_and_speed(destination_point, speed);
}

// Command this Ship's chained Ships to also set_destination_island_and_speed
void Chain_ship::set_destination_island_and_speed
(shared_ptr<Island> destination_island, double speed) {
	Ship::set_destination_island_and_speed(destination_island, speed);
	for (const auto& pair : chained_ship)
		pair.second->Ship::set_destination_island_and_speed(destination_island, speed);
}

// Command this Ship's chained Ships to also set_course_and_speed
void Chain_ship::set_course_and_speed(double course, double speed) {
	Ship::set_course_and_speed(course, speed);
	for (const auto& pair : chained_ship)
		pair.second->Ship::set_course_and_speed(course, speed);
}

// Command this Ship's chained Ships to stop
void Chain_ship::stop() {
	Ship::stop();
	state = State::not_moving_to_chain_ship;
	num_of_ship_chained = 0;
	num_of_ship_needed_to_chain = 0;
	for (const auto& pair : chained_ship)
		pair.second->Ship::stop();
}

// This Ship's chained Ships also take hit
void Chain_ship::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr) {
	Ship::receive_hit(hit_force, attacker_ptr);
	for (const auto& pair : chained_ship)
		pair.second->receive_hit(hit_force, attacker_ptr);
}

// Helper function that finds a Ship that is closest to this Chain_ship
void Chain_ship::find_closest_ship_to_chain() {
	double shortest_distance = numeric_limits<double>::max();

	auto it = map_of_ship_to_chain.cbegin();
	auto closest_ship = map_of_ship_to_chain.cbegin();
	while (it != map_of_ship_to_chain.cend()) {
		double distance =
			cartesian_distance(get_location(), it->second->get_location());

		// When the distance to the current Ship is less than the shortest distance,
		// update the shortest distance
		// and set ship_to_chain to the current Ship.
		if (distance < shortest_distance) {
			shortest_distance = distance;
			ship_to_chain = it->second;
			closest_ship = it;
		}
		++it;
	}

	location_of_ship_to_chain = ship_to_chain->get_location();
	ship_to_chain->stop();

	// Erase the Ship from map_of_ship_to_chain because this Chained_ship
	// does not need to chain the Ship anymore.
	map_of_ship_to_chain.erase(closest_ship);
	set_destination_position_and_speed(ship_to_chain->get_location(), get_maximum_speed());
}