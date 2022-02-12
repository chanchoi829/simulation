#include "Warship.h"
#include "Island.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>

using namespace std;

Warship::Warship(const string& name_, Point position_, 
	int firepower_, double max_range_, double fuel_capacity_, 
	double maximum_speed_, double fuel_consumption_, int resistance_) 
	: Ship(name_, position_, fuel_capacity_, maximum_speed_, 
		fuel_consumption_, resistance_), firepower(firepower_), 
	max_range(max_range_), state(Warship_state::not_attacking){}

// Update the state of the Warship
void Warship::update() {
	Ship::update();

	out_of_range = false;

	if (state == Warship_state::not_attacking)
		return;	

	// If Warship is not afloat or its target is not afloat,
	// stop attacking.
	if (!target.lock() || !target.lock()->is_afloat()) {
		stop_attack();
		target.reset();
		return;
	}

	cout << get_name() << " is attacking" << endl;

	// If the target is in range, make the target 
	// receive_hit with this Warship's firepower.
	if (cartesian_distance(get_location(), 
		target.lock()->get_location()) <= max_range) {
		cout << get_name() << " fires" << endl;
		target.lock()->receive_hit
		(firepower, shared_from_this());
		return;
	}

	out_of_range = true;
}

// Describe this Warship's state
void Warship::describe() const {
	Ship::describe();
	if (state == Warship_state::attacking) {
		if (!target.lock() || !target.lock()->is_afloat())
			cout << "Attacking absent ship" << endl;
		else
			cout << "Attacking " << target.lock()->get_name() << endl;
	}
}

// Start an attack on a target ship
void Warship::attack(shared_ptr<Ship> target_ptr_) {
	if (!is_afloat())
		throw Error("Cannot attack!");

	if (target_ptr_ == shared_from_this())
		throw Error("Cannot attack itself!");

	if (target.lock() == target_ptr_)
		throw Error("Already attacking this target!");

	// If this Warship is already attacking another target or if
	// this Warship is not attacking, set target to target_ptr
	// and set state to attacking.
	if (target.lock() || state == Warship_state::not_attacking) {
		target = target_ptr_;
		state = Warship_state::attacking;
		cout << get_name() << " will attack " << target.lock()->get_name() << endl;
	}
}

void Warship::stop_attack() {
	if (state != Warship_state::attacking)
		throw Error("Was not attacking!");

	// Change the state and reset target
	state = Warship_state::not_attacking;
	target.reset();
	cout << get_name() << " stopping attack" << endl;
}