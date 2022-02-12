/*
A Chain_ship can be told to chain anoter Ship in the simulation 
world. When commanded to set_destination_island, set_destination_position,
or set_course_and_speed, a Chain_ship also tells its chained Ships 
to also perform the same task. 

Initial values:
fuel capacity and initial amount 1500 tons, maximum speed 10., 
fuel consumption 4.tons/nm, resistance 1.
*/

#ifndef CHAIN_SHIP_H
#define CHAIN_SHIP_H

#include "Ship.h"
#include <map>
#include <memory>
#include <string>

class Chain_ship : public Ship {
public :
	Chain_ship(const std::string& name_, Point position_);

	// Chain every other Ship in the simulation world.
	void chain_all_ship() override;

	// Chain a Ship.
	// Throws an Error when target_to_chain is already chained to this Ship.
	void chain_ship(std::shared_ptr<Ship>target_to_chain) override;

	// Unchain a Ship.
	// Throws an Error ship_to_unchain is not chained to this Ship.
	void unchain_ship(std::shared_ptr<Ship>ship_to_unchain) override;

	// Update the state of Chain_ship
	void update() override;

	// Output a description of current state to cout
	void describe() const override;

	// Command this Ship's chained Ships to also set_destination_position_and_speed
	void set_destination_position_and_speed
	(Point destination_point, double speed) override;

	// Command this Ship's chained Ships to also set_destination_island_and_speed
	void set_destination_island_and_speed
	(std::shared_ptr<Island> destination_island, double speed) override;

	// Command this Ship's chained Ships to also set_course_and_speed
	void set_course_and_speed(double course, double speed) override;

	// Command this Ship's chained Ships to stop
	void stop() override;

	// This Ship's chained Ships also take hit
	void receive_hit(int hit_force, std::shared_ptr<Ship> attacker_ptr) override;

private :
	std::map<std::string, std::shared_ptr<Ship>> chained_ship;
	std::map<std::string, std::shared_ptr<Ship>> map_of_ship_to_chain;
	std::shared_ptr<Ship> ship_to_chain;

	Point location_of_ship_to_chain;
	int num_of_ship_needed_to_chain, num_of_ship_chained;

	enum class State {
		moving_to_chain_all_ship,
		moving_to_chain_ship,
		not_moving_to_chain_ship,
	};

	State state;

	// Helper function that finds a Ship that is closest to this Chain_ship
	void find_closest_ship_to_chain();
};

#endif