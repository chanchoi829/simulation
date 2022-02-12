/*
The Track_base class defines a base class of track objects, which are objects that move
according to course and speed. They have a Point, a Course_speed, and an altitude 
(set to zero for surface tracks). When updated, they change their Point 
as a function of their Course_speed.

Various values can be calculated for this track's position or motion as viewed from
some other track.
*/

#ifndef CRUISER_H
#define CRUISER_H

#include "Warship.h"

/*
A Cruiser is a Ship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm,
resistance 6, firepower 3, maximum attacking range 15
*/

class Cruiser : public Warship {
public:
	// initialize, then output constructor message
	Cruiser(const std::string& name_, Point position_);

	// perform Cruiser-specific behavior
	void update() override;

	// Describe this Cruiser's state
	void describe() const override;

	// respond to an attack
	void receive_hit(int hit_force, std::shared_ptr<Ship> attacker_ptr) override;
};

#endif