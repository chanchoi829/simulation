/*
A Torpedo_boat is similar to a Cruiser, but when commanded to attack, a Torpedo_boat
closes with its target by changing its course on every update. When it is close enough,
it fires at the target and continues until the target is sunk like Cruiser. However,
if a Torpedo_boat is fired upon, instead of counter-attacking like Cruiser, it runs away
to an Island of refuge.

Initial values:
fuel capacity and initial amount: 800, maximum speed 12., fuel consumption 5.tons/nm,
resistance 9, firepower 3, maximum attacking range 5
*/

#ifndef TORPEDO_BOAT_H
#define TORPEDO_BOAT_H

#include "Warship.h"

struct Point;

class Torpedo_boat : public Warship {
public:
	Torpedo_boat(const std::string& name_, Point position_);

	// When target is out of range this Torpedo_boat can move,
	// set the target's loation as the destination.
	void update() override;

	// Describe this Torpedo_boat's state
	void describe() const override;

	// Perform Torpedo_boat specific behavior for receive_hit
	void receive_hit(int hit_force, std::shared_ptr<Ship> attacker_ptr) override;
};

#endif