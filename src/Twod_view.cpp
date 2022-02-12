#include "Twod_view.h"
#include <cmath>

using namespace std;

Twod_view::Twod_view(int size_, double scale_, Point origin_) 
	: size(size_), scale(scale_), origin(origin_){}

// Populate a two-dimensional vector
// and a vector of objects which are outside the view of the map
void Twod_view::draw() {
	vector<vector<string>>twod_map;
	map<string, Point>object_temp_map = get_object_info_map();
	object_outside_map.clear();

	for (int j = 0; j < size; ++j) {
		vector<string>oned_map;
		for (int k = 0; k < size; ++k) {
			// count keeps track of how many objects are
			// in the same spot.
			int count = 0;

			// pushed indicates whether an object is inserted
			// into the current index or not.
			bool pushed = false;

			int x, y;
			auto it = object_temp_map.cbegin();

			while (it != object_temp_map.cend()) {
				bool inside = get_subscripts(x, y, it->second);

				// Check if x and y from get_subscripts match j and k
				if (x == k && y == size - j - 1) {
					if (count == 0) {
						oned_map.push_back(string(it->first.cbegin(),
							it->first.cbegin() + 2));
						++count;
						pushed = true;
					}
					// If more than one object was pushed into a certain point,
					// get rid of whatever is in that point and push "*"
					else {
						oned_map.pop_back();
						oned_map.push_back("*");
						++count;
					}

					object_temp_map.erase(it++);
				}
				// If the object is outside the map, 
				// add it to object_outside_map
				// and then erase it from object_temp_map
				else if (!inside) {
					object_outside_map.push_back(it->first);
					object_temp_map.erase(it++);
				}
				else
					++it;
			}

			// If nothing was pushed, push "."
			if (!pushed)
				oned_map.push_back(".");
		}
		twod_map.push_back(oned_map);
	}
	two_dimensional_map = twod_map;
}

void Twod_view::set_size(int size_) {
	size = size_;
}

void Twod_view::set_scale(double scale_) {
	scale = scale_;
}

void Twod_view::set_origin(Point origin_) {
	origin = origin_;
}

void Twod_view::set_defaults() {
	size = 25;
	scale = 2.0;
	origin = Point(-10, -10);
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool Twod_view::get_subscripts(int &ix, int &iy, Point location) const {
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// Truncate coordinates to appropriate integer after taking the floor.
	// The floor function will produce the greatest integer that is 
	// smaller than the argument, even for negative values. 
	// So - 0.05 is floored to -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
	}
	return true;
}