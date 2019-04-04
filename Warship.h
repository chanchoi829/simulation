#ifndef WARSHIP_H
#define WARSHIP_H

#include "Ship.h"
#include <memory>
#include <string>

// Intermediate class for different types of Warships to derive from

class Warship : public Ship {
public:
	// Update the state of the Warship
	void update() override;

	// Output a description of current state to cout
	void describe() const override;

	// Start an attack on a target ship
	void attack(std::shared_ptr<Ship> target_ptr_) override;

	void stop_attack() override;

protected:
	Warship(const std::string& name_, Point position_,
		int firepower_, double max_range, double fuel_capacity_,
		double maximum_speed_, double fuel_consumption_, int resistance_);

	// Getters
	bool target_out_of_range() const
		{return out_of_range;}

	bool is_attacking() const
		{return state == Warship_state::attacking;}

	std::weak_ptr<Ship> get_target() const
		{return target;}

private:
	std::weak_ptr<Ship> target;
	int firepower;
	double max_range;

	enum class Warship_state {
		not_attacking,
		attacking,
	};

	Warship_state state;
	bool out_of_range;
};

#endif