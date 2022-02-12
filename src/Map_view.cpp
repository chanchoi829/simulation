#include "Map_view.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// default constructor sets the default size, 
// scale, and origin
Map_view::Map_view() : Twod_view(25, 2.0, Point(-10, -10)) {}

// prints out the current map
void Map_view::draw() {
	Twod_view::draw();
	cout << "Display size: " << get_size() << ", scale: "
		<< get_scale() << ", origin: " << get_origin() << endl;

	// Save cout settings and set precision to 0.
	ios::fmtflags old_settings = cout.flags();
	int old_precision = cout.precision();
	cout.precision(0);

	const vector<vector<string>>twod_map = 
		get_twod_map();

	const vector<string>object_outside_map = get_object_outside_map();

	bool print_comma = false;
	for (const string& name : object_outside_map) {
		if (print_comma)
			cout << ", ";
		cout << name;
		print_comma = true;
	}

	if (!object_outside_map.empty())
		cout << " outside the map" << endl;

	for (int j = 0; j < get_size(); ++j) {
		// coord_printed indicates if a coordinate was printed
		// in a certain row.
		bool coord_printed = false;

		// Check if you should print a y coordinate in a certain row.
		if ((get_size() - j - 1) % 3 == 0) {
			cout << setw(4) << (get_size() - j - 1) * get_scale() + get_origin().y;
			coord_printed = true;
		}

		bool printed_str_before = false;

		// Set width accordingly.
		for (int k = 0; k < get_size(); ++k) {
			// Case when a coord was printed, and it is the first point in a row,
			// and it is a string 
			if (!coord_printed && k == 0 &&
				twod_map[j][k] != "." && twod_map[j][k] != "*") {
				cout << setw(7);
				printed_str_before = true;
			}
			// Case when a coord was not printed and it is the first
			// point in a row.
			else if (!coord_printed && k == 0) {
				cout << setw(6);
				printed_str_before = false;
			}
			// Case when a string was not printed before, and the current point
			// is a string.
			else if (!printed_str_before &&
				twod_map[j][k] != "." && twod_map[j][k] != "*") {
				cout << setw(3);
				printed_str_before = true;
			}
			// Case when a string was not printed before, and the current point
			// is not a string.
			else if (!printed_str_before &&
				(twod_map[j][k] == "." || twod_map[j][k] == "*")) {
				cout << setw(2);
			}
			// Case when a string was printed before, and the current point is
			// not a string.
			else if (printed_str_before &&
				(twod_map[j][k] == "." || twod_map[j][k] == "*")) {
				printed_str_before = false;
			}

			cout << twod_map[j][k];
		}
		cout << " " << endl;
	}

	// Print x coordinates
	for (int a = 0; a < get_size(); ++a)
		if (a % 3 == 0)
			cout << setw(6) << a * get_scale() + get_origin().x;
	cout << endl;

	// Reset cout info.
	cout.flags(old_settings);
	cout.precision(old_precision);
}

// Modify the display parameters:
// If the size is out of bounds will throw Error("New map size is too big!")
// or Error("New map size is too small!").
void Map_view::set_size(int size_) {
	if (size_ > 30)
		throw Error("New map size is too big!");

	if (size_ <= 6)
		throw Error("New map size is too small!");

	Twod_view::set_size(size_);
}

// If scale is not postive, will throw Error("New map scale must be positive!");
void Map_view::set_scale(double scale_) {
	if (scale_ < 0.0)
		throw Error("New map scale must be positive!");

	Twod_view::set_scale(scale_);
}