#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include "Twod_view.h"
#include <string>

struct Point;

class Map_view : public Twod_view
{
public:
    // default constructor sets the default size, scale, and origin, outputs constructor message
    Map_view();

    // prints out the current map
    void draw() override;

    // Modify the display parameters:
    // If the size is out of bounds will throw Error("New map size is too big!")
    // or Error("New map size is too small!").
    void set_size(int size_) override;

    // If scale is not postive, will throw Error("New map scale must be positive!");
    void set_scale(double scale_) override;
};

#endif
