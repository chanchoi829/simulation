#include "Cruiser.h"
#include <iostream>

using namespace std;

/* Public Function Definitions */

Cruiser::Cruiser(const string& name_, Point position_)
    : Warship(name_, position_, 3, 15.0, 1000, 20, 10, 6)
{ }

// perform Cruiser-specific behavior
void Cruiser::update()
{
    Warship::update();

    if (target_out_of_range()) {
        cout << get_name() << " target is out of range" << endl;
        stop_attack();
    }
}

// Describe this Cruiser's state
void Cruiser::describe() const
{
    cout << "\nCruiser ";
    Warship::describe();
}

// respond to an attack
void Cruiser::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr)
{
    Ship::receive_hit(hit_force, attacker_ptr);

    if (!is_afloat())
        return;

    // Counter attack if Cruiser is not attacking.
    if (!is_attacking())
        attack(attacker_ptr);
}