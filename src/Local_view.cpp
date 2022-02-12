#include "Local_view.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>

using namespace std;

Local_view::Local_view(const string& ship_name_) : Twod_view(9, 2.0, Point(0, 0))
	, sunk(false), ship_name(ship_name_) {}

// Change origin of the Local_view when name matches Local_view's ship name.
void Local_view::update_location(const string& name, Point location) {
	View::update_location(name, location);

	if (ship_name == name && !sunk) {
		ship_location = location;
		Twod_view::set_origin(Point(location.x - 
			(get_size() / 2.0) * get_scale(), 
			location.y - (get_size() / 2.0) * get_scale()));
	}
}

// If name matches the Local_view's Ship's name,
// mark it as sunk.
void Local_view::update_remove(const string& name) {
	View::update_remove(name);

	if (ship_name == name)
		sunk = true;
}

// Print local view for a Ship.
void Local_view::draw() {
	Twod_view::draw();
	if (sunk)
		cout << "Local view for " << ship_name 
		<< " sunk at " << ship_location << endl;
	else
		cout << "Local view for " << ship_name 
		<< " at position " << ship_location << endl;

	const vector<vector<string>>twod_map = get_twod_map();

	for (int i = 0; i < get_size(); ++i) {
		// printed_str_before indicates if
		// the loop printed a string before
		bool printed_str_before = false;
		cout << "    ";
		for (int j = 0; j < get_size(); ++j) {
			// Format strings according to each case
			if (twod_map[i][j] == ".") {
				if (!printed_str_before)
					cout << setw(2);

				if (j == get_size() - 1 && !printed_str_before)
					cout << " . ";
				else if (j == get_size() - 1)
					cout << ". ";
				else
					cout << ".";

				printed_str_before = false;
			}
			else if (twod_map[i][j] == "*") {
				if (!printed_str_before)
					cout << setw(2);
				cout << "*";
				printed_str_before = false;
			}
			// Case when it is an object's name.
			else {
				if (!printed_str_before)
					cout << setw(3);
				// Print the first two characters of the object's name.
				cout << string(twod_map[i][j].cbegin(), twod_map[i][j].cbegin() + 2);
				printed_str_before = true;
			}
		}
		cout << endl;
	}
}

// Throw an Error because you cannot perform these functions
void Local_view::set_size(int size_) {
	throw Error("Cannot set size for this View!");
}

void Local_view::set_scale(double scale_) {
	throw Error("Cannot set scale for this View!");
}

void Local_view::set_origin(Point origin_) {
	throw Error("Cannot set origin for this View!");
}

void Local_view::set_defaults() {
	throw Error("Cannot set default for this View!");
}