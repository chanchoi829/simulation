#ifndef LOCAL_VIEW_H
#define LOCAL_VIEW_H

#include "Twod_view.h"
#include <string>

class Local_view : public Twod_view
{
public:
    Local_view(const std::string& ship_name_);

    // Change origin of the Local_view when name matches Local_view's ship name.
    void update_location(const std::string& name, Point location) override;

    // If name matches the Local_view's Ship's name,
    // mark it as sunk.
    void update_remove(const std::string& name) override;

    // Print local view for a Ship.
    void draw() override;

    // Throw an Error because you cannot perform these functions
    void set_size(int size_) override;

    void set_scale(double scale_) override;

    void set_origin(Point origin_) override;

    void set_defaults() override;

private:
    bool sunk;
    Point ship_location;
    std::string ship_name;
};

#endif
