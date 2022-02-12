
/*
Define the destructor function even if it was declared as a pure virtual function.
This seems odd, because pure virtual functions are usually not defined in the class
that declares them. But this is often done as a way to make a class abstract, 
if there is no other virtual function that makes sense to mark as pure. Here we
are defining it just to get the destructor message output.
*/

#include "Ship.h"
#include "Model.h"
#include "Island.h"
#include "Utility.h"
#include <iostream>

using namespace std;

// initialize, then output constructor message
Ship::Ship(const string& name_, Point position_, double fuel_capacity_,
	double maximum_speed_, double fuel_consumption_, int resistance_) : Sim_object(name_),
	fuel(fuel_capacity_), fuel_capacity(fuel_capacity_), maximum_speed(maximum_speed_), 
	fuel_consumption(fuel_consumption_), resistance(resistance_), tracker(position_), 
	ship_state(State::stopped) {}

string Ship::get_name() const {
	return Sim_object::get_name();
}

// Print this Ship's name.
void Ship::describe_component() const {
	// Print spaces depending on this Ship_component's place in
	// the hierarchy.
	for (int i = 0; i < Ship_component::index_counter(); ++i)
		cout << "   ";
	cout << get_name() << endl;
}

// Check if ship is equal to this Ship
bool Ship::check_if_ship_exists(const string& ship) const {
	return ship == get_name();
}

/*** Readers ***/
// Return true if ship can move 
// (it is not dead in the water or in the process or sinking); 
bool Ship::can_move() const {
	return ship_state != State::sunk 
		&& ship_state != State::dead_in_the_water;
}

// Return true if ship is moving
bool Ship::is_moving() const {
	return ship_state == State::moving_to_position ||
		ship_state == State::moving_to_island ||
		ship_state == State::moving_on_course;
}

// Return true if ship is docked
bool Ship::is_docked() const {
	return ship_state == State::docked;
}

// Return true if ship is afloat (not in process of sinking), false if not
bool Ship::is_afloat() const {
	return ship_state != State::sunk;
}

// Return true if the ship is Stopped and the distance to the supplied island
// is less than or equal to 0.1 nm
bool Ship::can_dock(shared_ptr<Island> island_ptr) const {
	return ship_state == State::stopped &&
		cartesian_distance(get_location(),
			island_ptr->get_location()) <= 0.1;
}

/*** Interface to derived classes ***/
// Update the state of the Ship according to its current state.
void Ship::update() {
	switch (ship_state) {
	case State::sunk:
		cout << get_name() << " sunk" << endl;
		break;

	// If this Ship is moving, calculate_movement() to
	// update its position on the map and notify
	// Model about its new location.
	case State::moving_on_course:
	case State::moving_to_island:
	case State::moving_to_position:
		calculate_movement();
		cout << get_name() << " now at " << get_location() << endl;

		Model::get_instance().
			notify_location(get_name(), get_location());
		Model::get_instance().
			notify_view_about_ship_fuel(get_name(), fuel);
		Model::get_instance().
			notify_view_about_ship_course(get_name(), tracker.get_course());
		Model::get_instance().
			notify_view_about_ship_speed(get_name(), tracker.get_speed());

		break;

	case State::stopped:
		cout << get_name() << " stopped at " << get_location() << endl;
		break;

	case State::docked:
		cout << get_name() << " docked at " << docked_island->get_name() << endl;
		break;

	case State::dead_in_the_water:
		cout << get_name() << " dead in the water at " << get_location() << endl;
		break;

	default:
		throw Error("Unrecognized state!");
	}
}
// output a description of current state to cout
void Ship::describe() const {
	cout << get_name() << " at " << get_location();

	if (ship_state == State::sunk) {
		cout << " sunk" << endl;
		return;
	}

	cout << ", fuel: " << fuel << " tons, resistance: "
		<< resistance << endl;

	// Output messages accordingly to this Ship's
	// state.
	switch (ship_state) {	
		case State::moving_to_position:
			cout << "Moving to " << destination_point << " on "
				<< tracker.get_course_speed() << endl;
			break;
		case State::moving_to_island:
			cout << "Moving to " << destination_Island->get_name()
				<< " on " << tracker.get_course_speed() << endl;
			break;
		case State::moving_on_course:
			cout << "Moving on " << tracker.get_course_speed() << endl;
			break;
		case State::docked:
			cout << "Docked at " << docked_island->get_name() << endl;
			break;
		case State::stopped:
			cout << "Stopped" << endl;
			break;
		case State::dead_in_the_water:
			cout << "Dead in the water" << endl;
			break;
		default:
			throw Error("Unrecognized state!");
	}
}

// Notify Model about this Ship's name and location.
void Ship::broadcast_current_state() const {
	Model::get_instance().notify_location(get_name(), get_location());
}

// Notify Model about this Ship's name, fuel, course, speed, and whether it is
// afloat or now.
void Ship::broadcast_ship_fuel() const {
	Model::get_instance().notify_view_about_ship_fuel(get_name(), fuel);
}

// Notify Model about this Ship's name, fuel, course, speed, and whether it is
// afloat or now.
void Ship::broadcast_ship_course() const {
	Model::get_instance().notify_view_about_ship_course(get_name(), tracker.get_course());
}

// Notify Model about this Ship's name, fuel, course, speed, and whether it is
// afloat or now.
void Ship::broadcast_ship_speed() const {
	Model::get_instance().notify_view_about_ship_speed(get_name(), tracker.get_speed());
}

/*** Command functions ***/
// Start moving to a destination position at a speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!")
void Ship::set_destination_position_and_speed 
(Point destination_position, double speed) {
	if (!can_move())
		throw Error("Ship cannot move!");

	if (speed > maximum_speed)
		throw Error("Ship cannot go that fast!");

	// Create Compass_vector with this Ship's location and
	// destination_position to get the Ship's direction.
	Compass_vector vec(get_location(), destination_position);
	tracker.set_course(vec.direction);
	tracker.set_speed(speed);

	// No longer docked, and this Ship is no longer
	// going to an Island, so reset docked_island and
	// destination_Island.
	if (is_docked())
		docked_island = nullptr;

	destination_Island = nullptr;
	destination_point = destination_position;
	ship_state = State::moving_to_position;

	cout << get_name() << " will sail on " << tracker.get_course_speed()
		<< " to " << destination_position << endl;
	Model::get_instance().
		notify_view_about_ship_course(get_name(), tracker.get_course());
	Model::get_instance().
		notify_view_about_ship_speed(get_name(), tracker.get_speed());
}

// Start moving to a destination Island at a speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!")
void Ship::set_destination_island_and_speed
(shared_ptr<Island> destination_island, double speed) {
	if (!can_move())
		throw Error("Ship cannot move!");

	if (speed > maximum_speed)
		throw Error("Ship cannot go that fast!");

	// Create Compass_vector with this Ship's location and
	// destination_position to get the Ship's direction.
	Compass_vector vec(get_location(), destination_island->get_location());
	tracker.set_course(vec.direction);
	tracker.set_speed(speed);

	// Reset docked_island because this Ship is no longer
	// docked.
	if (is_docked())
		docked_island = nullptr;

	destination_Island = destination_island;
	ship_state = State::moving_to_island;
	destination_point = destination_island->get_location();

	cout << get_name() << " will sail on " << tracker.get_course_speed()
		<< " to " << destination_island->get_name() << endl;

	Model::get_instance().
		notify_view_about_ship_course(get_name(), tracker.get_course());
	Model::get_instance().
		notify_view_about_ship_speed(get_name(), tracker.get_speed());
}

// Start moving on a course and speed
// may throw Error("Ship cannot move!")
// may throw Error("Ship cannot go that fast!");
void  Ship::set_course_and_speed(double course, double speed) {
	if (!can_move())
		throw Error("Ship cannot move!");

	if (speed > maximum_speed)
		throw Error("Ship cannot go that fast!");

	tracker.set_course(course);
	tracker.set_speed(speed);

	// No longer docked, and this Ship is no longer
	// going to an Island, so reset docked_island and
	// destination_Island.
	if (is_docked())
		docked_island = nullptr;

	destination_Island = nullptr;
	ship_state = State::moving_on_course;

	cout << get_name() << " will sail on " << tracker.get_course_speed() << endl;
	Model::get_instance().
		notify_view_about_ship_course(get_name(), tracker.get_course());
	Model::get_instance().
		notify_view_about_ship_speed(get_name(), tracker.get_speed());
}

// Stop moving
// may throw Error("Ship cannot move!");
void Ship::stop() {
	if (ship_state == State::dead_in_the_water || !is_afloat())
		throw Error("Ship cannot move!");

	tracker.set_speed(0);
	cout << get_name() << " stopping at " << get_location() << endl;
	ship_state = State::stopped;
	Model::get_instance().
		notify_view_about_ship_course(get_name(), tracker.get_course());
	Model::get_instance().
		notify_view_about_ship_speed(get_name(), tracker.get_speed());
}

// dock at an Island - set our position = Island's position, 
// go into Docked state may throw Error("Can't dock!");
void  Ship::dock(shared_ptr<Island> island_ptr) {
	// If this Ship is not stopped or if the distance from its location
	// and the island_ptr's location is greater than 0.1, the Ship cannot
	// dock, so throw an Error.
	if (ship_state != State::stopped || 
		cartesian_distance(get_location(), island_ptr->get_location()) > 0.1)
		throw Error("Can't dock!");

	tracker.set_position(island_ptr->get_location());
	Model::get_instance().notify_location(get_name(), get_location());
	docked_island = island_ptr;
	ship_state = State::docked;

	cout << get_name() << " docked at " << island_ptr->get_name() << endl;
}

// Refuel - must already be docked at an island; fill takes as much as possible
// may throw Error("Must be docked!");
void  Ship::refuel() {
	if (ship_state != State::docked)
		throw Error("Must be docked!");

	// Calculate amount needed and if it is less than 0.005,
	// completely refuel it to the capacity. 
	double fuel_needed = fuel_capacity - fuel;

	if (fuel_needed < 0.005) {
		fuel = fuel_capacity;
		Model::get_instance().
			notify_view_about_ship_fuel(get_name(), fuel);
		return;
	}

	// Otherwise, ask the docked_island for the fuel_needed.
	fuel += docked_island->provide_fuel(fuel_needed);
	cout << get_name() << " now has " << fuel << " tons of fuel" << endl;
	Model::get_instance().
		notify_view_about_ship_fuel(get_name(), fuel);
}

/*** Fat interface command functions ***/

void Ship::chain_all_ship() {
	throw Error("Cannot chain Ship!");
}

void Ship::chain_ship(shared_ptr<Ship>) {
	throw Error("Cannot chain Ship!");
}

void Ship::unchain_ship(shared_ptr<Ship>) {
	throw Error("Cannot drop Ship!");
}

// These functions throw an Error exception for this class
// will always throw Error("Cannot load at a destination!");
void  Ship::set_load_destination(shared_ptr<Island>) {
	throw Error("Cannot load at a destination!");
}
// will always throw Error("Cannot unload at a destination!");
void  Ship::set_unload_destination(shared_ptr<Island>) {
	throw Error("Cannot unload at a destination!");
}
// will always throw Error("Cannot attack!");
void  Ship::attack(shared_ptr<Ship>) {
	throw Error("Cannot attack!");
}
// will always throw Error("Cannot attack!");
void Ship::stop_attack() {
	throw Error("Cannot attack!");
}

// interactions with other objects
// receive a hit from an attacker
void Ship::receive_hit(int hit_force, shared_ptr<Ship> attacker_ptr) {
	resistance -= hit_force;
	cout << get_name() << " hit with " << hit_force << 
		", resistance now " << resistance << endl;

	// If this Ship's resistance is less than 0, and it
	// is still floating, it starts sinking with speed 0.
	if (resistance < 0) {
		ship_state = State::sunk;
		cout << get_name() << " sunk" << endl;
		tracker.set_speed(0.0);
		Model::get_instance().notify_gone(get_name());
		Model::get_instance().remove_ship(shared_from_this());
	}
}

double Ship::get_maximum_speed() const {
	return maximum_speed;
}
// return pointer to the Island currently docked at, or nullptr if not docked
shared_ptr<Island> Ship::get_docked_Island() const {
	return docked_island;
}
// return pointer to current destination Island, nullptr if not set
shared_ptr<Island> Ship::get_destination_Island() const {
	return destination_Island;
}

/* Private Function Definitions */

/*
Calculate the new position of a ship based on how it is moving, its speed, and
fuel state. This function should be called only if the state is 
moving_to_position, moving_to_island, or moving_on_course.

Track_base has an update_position(double time) function that computes the new position
of an object after the specified time has elapsed. If the Ship is going to move
for a full time unit (one hour), then it will get go the "full step" distance,
so update_position would be called with time = 1.0. If we can move less than that,
e.g. due to not enough fuel, update position  will be called with the corresponding
time less than 1.0.

For clarity in specifying the computation, this code assumes the specified private variable names, 
but you may change the variable names or enum class names, or state names if you wish (e.g. movement_state).
*/
void Ship::calculate_movement()
{
	// Compute values for how much we need to move, and how much we can, and how long we can,
	// given the fuel state, then decide what to do.
	double time = 1.0;	// "full step" time
	// get the distance to destination
	double destination_distance = cartesian_distance(get_location(), destination_point);
	// get full step distance we can move on this time step
	double full_distance = tracker.get_speed() * time;
	// get fuel required for full step distance
	double full_fuel_required = full_distance * fuel_consumption;	// tons = nm * tons/nm
	// how far and how long can we sail in this time period based on the fuel state?
	double distance_possible, time_possible;
	if(full_fuel_required <= fuel) {
		distance_possible = full_distance;
		time_possible = time;
		}
	else {
		distance_possible = fuel / fuel_consumption;	// nm = tons / tons/nm
		time_possible = (distance_possible / full_distance) * time;
		}
	
	// are we are moving to a destination, and is the destination within the distance possible?
	if((ship_state == State::moving_to_position || ship_state== State::moving_to_island) && destination_distance <= distance_possible) {
		// yes, make our new position the destination
		tracker.set_position(destination_point);
		// we travel the destination distance, using that much fuel
		double fuel_required = destination_distance * fuel_consumption;
		fuel -= fuel_required;
		tracker.set_speed(0.);
		ship_state = State::stopped;
		}
	else {
		// go as far as we can, stay in the same movement state
		// simply move for the amount of time possible
		tracker.update_position(time_possible);
		// have we used up our fuel?
		if(full_fuel_required >= fuel) {
			fuel = 0.0;
			tracker.set_speed(0.);
			ship_state = State::dead_in_the_water;
			}
		else {
			fuel -= full_fuel_required;
			}
		}
}