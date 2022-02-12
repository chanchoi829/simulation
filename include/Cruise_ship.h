/*
A Cruise_ship can automatically visit all of the islands in a 
leisurely fashion. It behaves like a normal ship until you
tell it to go to an island with the "destination" command.

Initial values:
fuel capacity and initial amount 500 tons, maximum speed 15., 
fuel consumption 2.tons/nm, resistance 0.
*/

#ifndef CRUISER_SHIP_H
#define CRUISER_SHIP_H

#include "Ship.h"
#include <memory>
#include <string>
#include <vector>

class Cruise_ship : public Ship {
public :
	Cruise_ship(const std::string& name_, Point position_);

	// Update the state of Cruise_ship
	void update() override;

	// Output a description of current state to cout
	void describe() const override;

	// When Cruise_ship is cruising, cancel it.
	void set_destination_position_and_speed
	(Point destination_point, double speed) override;

	// When Cruise_ship was initially cruising, cancel the cruise and a new
	// Island destination. Otherwise, set a new destination and set up a 
	// cruise to the Islands.
	void set_destination_island_and_speed
	(std::shared_ptr<Island> destination_island, double speed) override;

	// Cancel cruise if Cruise_ship was cruising.
	void set_course_and_speed(double course, double speed) override;

	// Cancel cruise if Cruise_ship was cruising.
	void stop() override;

private :
	// Bool vector to indicate which islands have been visited.
	std::vector<bool>visited_vec;

	// Indicates which Island to visit during the cruise.
	std::shared_ptr<Island>island_to_visit;

	// An Island pointer to remember where the cruise started
	std::shared_ptr<Island>starting_island;

	enum class Cruise_ship_state {
		not_cruising,
		set_cruise,
		just_docked,
		refuel,
		waiting,
		going_back,
	};

	Cruise_ship_state state;
	int island_visited;
	double starting_speed;

	void cancel_cruise_and_print();
};

#endif