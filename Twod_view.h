/*
Twod_view represents an intermdiate class for Views which
display a two-dimensional view. Twod_view's draw function
populates a two-dimensional vector with remembered information
of locations of simulation objects.
*/

#ifndef TWOD_VIEW_H
#define TWOD_VIEW_H

#include "View.h"
#include <string>
#include <vector>

class Twod_view : public View {
public:
	// Populate a two-dimensional vector
	// and a vector of objects which are outside the view of the map
	virtual void draw() override;

	virtual void set_size(int size_) override;

	virtual void set_scale(double scale_) override;

	virtual void set_origin(Point origin_) override;

	virtual void set_defaults() override;

protected:
	// Protected constructor for quasi-abstractness
	Twod_view(int size_, double scale_, Point origin_);

	// Getters
	std::vector<std::string> get_object_outside_map() const
		{return object_outside_map;}

	std::vector<std::vector<std::string>> get_twod_map() const
		{return two_dimensional_map;}

	int get_size() const
		{return size;}

	double get_scale() const
		{return scale;}

	Point get_origin() const
		{return origin;}

private:
	int size;			// current size of the display
	double scale;		// distance per cell of the display
	Point origin;		// coordinates of the lower-left-hand corner

	std::vector<std::string> object_outside_map;
	std::vector<std::vector<std::string>> two_dimensional_map;

	// Calculate the cell subscripts corresponding to the supplied location parameter, 
	// using the current size, scale, and origin of the display. 
	// This function assumes that origin is a  member variable of type Point, 
	// scale is a double value, and size is an integer for the number of rows/columns 
	// currently being used for the grid.
	// Return true if the location is within the grid, false if not
	bool get_subscripts(int &ix, int &iy, Point location) const;
};

#endif