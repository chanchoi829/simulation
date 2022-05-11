#include "Tanker.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>

using namespace std;

// initialize, the output constructor message
Tanker::Tanker(const string& name_, Point position_)
    : Ship(name_, position_, 100, 10, 2, 0)
    , cargo(0)
    , cargo_capacity(1000)
    , tanker_state(Tanker_state::no_destination)
{ }

// This class overrides these Ship functions so that it can
// check if this Tanker has assigned cargo destinations.
// if so, throw an Error("Tanker has cargo destinations!");
// otherwise, simply call the Ship functions.
void Tanker::set_destination_position_and_speed(Point destination_point, double speed)
{
    if (tanker_state != Tanker_state::no_destination)
        throw Error("Tanker has cargo destinations!");

    Ship::set_destination_position_and_speed(destination_point, speed);
}

void Tanker::set_destination_island_and_speed(shared_ptr<Island> destination_island, double speed)
{
    if (tanker_state != Tanker_state::no_destination)
        throw Error("Tanker has cargo destinations!");

    Ship::set_destination_island_and_speed(destination_island, speed);
}

void Tanker::set_course_and_speed(double course, double speed)
{
    if (tanker_state != Tanker_state::no_destination)
        throw Error("Tanker has cargo destinations!");

    Ship::set_course_and_speed(course, speed);
}

// Set the loading and unloading Island destinations
// if both cargo destination are already set,
// throw Error("Tanker has cargo destinations!").
// if they are the same, leave at the set values, and
// throw Error("Load and unload cargo destinations are the same!")
// if both destinations are now set, start the cargo cycle
void Tanker::set_load_destination(shared_ptr<Island> island_ptr)
{
    if (tanker_state != Tanker_state::no_destination)
        throw Error("Tanker has cargo destinations!");

    load_destination = island_ptr;

    if (load_destination == unload_destination)
        throw Error("Load and unload cargo destinations are the same!");

    cout << get_name() << " will load at " << island_ptr->get_name() << endl;

    // If both load_destination and unload_destination
    // are set, start the cargo cycle.
    start_cargo_cycle();
}
void Tanker::set_unload_destination(shared_ptr<Island> island_ptr)
{
    if (tanker_state != Tanker_state::no_destination)
        throw Error("Tanker has cargo destinations!");

    unload_destination = island_ptr;

    if (load_destination == unload_destination)
        throw Error("Load and unload cargo destinations are the same!");

    cout << get_name() << " will unload at " << island_ptr->get_name() << endl;

    // If both load_destination and unload_destination
    // are set, start the cargo cycle.
    start_cargo_cycle();
}

// when told to stop, clear the cargo destinations and stop
void Tanker::stop()
{
    Ship::stop();
    load_destination = nullptr;
    unload_destination = nullptr;
    tanker_state = Tanker_state::no_destination;
    cout << get_name() << " now has no cargo destinations" << endl;
}

// perform Tanker-specific behavior
// Call Ship::update() first and update this Tanker's
// specific states.
void Tanker::update()
{
    Ship::update();
    // If it cannot move reset its destination pointers
    // and set its state to no destination.
    if (!can_move()) {
        tanker_state = Tanker_state::no_destination;
        load_destination = nullptr;
        unload_destination = nullptr;
        cout << get_name() << " now has no cargo destinations" << endl;
        return;
    }

    switch (tanker_state) {
    case Tanker_state::no_destination:
        break;

        // If this Tanker is moving to loading but is not moving and can
        // dock at its load destination, dock there and set the state
        // to loading.
    case Tanker_state::moving_to_loading:
        if (!is_moving() && can_dock(load_destination)) {
            dock(load_destination);
            tanker_state = Tanker_state::loading;
        }
        break;

        // If this Tanker is moving to unloading but is not moving and can
        // dock at its unload destination, dock there and set the state
        // to unloading.
    case Tanker_state::moving_to_unloading:
        if (!is_moving() && can_dock(unload_destination)) {
            dock(unload_destination);
            tanker_state = Tanker_state::unloading;
        }
        break;

    case Tanker_state::loading: {
        refuel();
        double fuel_needed = cargo_capacity - cargo;
        // If fuel_needed is less than 0.005, start moving
        // to unload_destination and set its state to
        // moving to unloading.
        if (fuel_needed < 0.005) {
            cargo = cargo_capacity;
            Ship::set_destination_island_and_speed(unload_destination, get_maximum_speed());
            tanker_state = Tanker_state::moving_to_unloading;
            break;
        }

        // Otherwise, get fuel_needed from its docked Island.
        cargo += get_docked_Island()->provide_fuel(fuel_needed);
        cout << get_name() << " now has " << cargo << " of cargo" << endl;
        break;
    }

    case Tanker_state::unloading:
        // If its cargo is 0.0, start moving to its load destination
        // and set the state to moving to loading.
        if (cargo == 0.0) {
            Ship::set_destination_island_and_speed(load_destination, get_maximum_speed());
            tanker_state = Tanker_state::moving_to_loading;
            break;
        }

        // Otherwise, provide its cargo to its docked Island.
        get_docked_Island()->accept_fuel(cargo);
        cargo = 0.0;
    }
}

// Call Ship::describe() first, and describe this Tanker's
// specific states.
void Tanker::describe() const
{
    cout << "\nTanker ";
    Ship::describe();
    cout << "Cargo: " << cargo << " tons";

    switch (tanker_state) {
    case Tanker_state::no_destination:
        cout << ", no cargo destinations" << endl;
        break;
    case Tanker_state::loading:
        cout << ", loading" << endl;
        break;
    case Tanker_state::unloading:
        cout << ", unloading" << endl;
        break;
    case Tanker_state::moving_to_loading:
        cout << ", moving to loading destination" << endl;
        break;
    case Tanker_state::moving_to_unloading:
        cout << ", moving to unloading destination" << endl;
        break;
    }
}

// If both load_destination and unload_destination are
// set, change this Tanker's state and information depending
// on its state.
void Tanker::start_cargo_cycle()
{
    // Check if both pointers are set; otherwise,
    // do not do anything.
    if (load_destination && unload_destination) {
        if (is_docked()) {
            // If this Tanker's docked at its load_destination,
            // set its state to loading.
            if (get_docked_Island() == load_destination) {
                tanker_state = Tanker_state::loading;
                return;
            }
            // If this Tanker's docked at its unload_destination,
            // set its state to unloading.
            else if (get_docked_Island() == unload_destination) {
                tanker_state = Tanker_state::unloading;
                return;
            }
        }

        if (!is_moving()) {
            // If cargo is 0.0 and can dock at load_destination,
            // do so.
            if (cargo == 0.0 && can_dock(load_destination)) {
                dock(load_destination);
                tanker_state = Tanker_state::loading;
                return;
            }

            // If cargo greater than 0.0 and can dock at load_destination,
            // do so.
            if (cargo > 0.0 && can_dock(unload_destination)) {
                dock(unload_destination);
                tanker_state = Tanker_state::unloading;
                return;
            }
        }

        // If its cargo is 0.0, make this Tanker go to
        // its load_destination.
        if (cargo == 0.0) {
            Ship::set_destination_island_and_speed(load_destination, get_maximum_speed());
            tanker_state = Tanker_state::moving_to_loading;
        }

        // If its cargo is greater than 0.0, make this Tanker
        // go to its unload_destination.
        if (cargo > 0.0) {
            Ship::set_destination_island_and_speed(unload_destination, get_maximum_speed());
            tanker_state = Tanker_state::moving_to_unloading;
        }
    }
}