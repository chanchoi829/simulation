/*
Ship_composite represents a class which can contain
Ship_components which can be either a Ship or a Ship_composite.
When you issue a ship command to a Ship_composite, the Ship_composite
performs the command on its container of Ship_components. If a Ship_component
throws an Error because a command could not be performed on it, the Error
is simply ignored.
*/

#ifndef SHIP_COMPOSITE_H
#define SHIP_COMPOSITE_H

#include "Ship_component.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Island;
struct Point;

class Ship_composite
    : public Ship_component
    , public std::enable_shared_from_this<Ship_composite>
{
public:
    Ship_composite(std::string composite_name_);

    // Add a new Ship_component to ship_components map
    virtual void add_component(std::shared_ptr<Ship_component> new_ship_component) override;

    // Remove a Ship_component from ship_components
    virtual void remove_component(std::shared_ptr<Ship_component> ship_component) override;

    // Find Ship_composite from ship_components
    virtual std::shared_ptr<Ship_component> find_ship_composite(const std::string& name) override;

    // Find Ship_composite and remove it
    virtual bool find_and_remove_ship_composite(const std::string& name) override;

    // Return a bool indicating if a Ship exists in ship_components
    virtual bool check_if_ship_exists(const std::string& ship) const override;

    // Describe ship_components
    virtual void describe_component() const override;

    virtual std::string get_name() const override;

    /*** Command functions ***/

    // When given a command, this Ship_composite also performs the command on its container
    // of Ship_components. If a Ship_component throws an Error because a command could not
    // be performed on it, the Error is simply ignored.

    // Start moving to a destination position at a speed
    virtual void set_destination_position_and_speed(Point destination_position, double speed);

    // Start moving to a destination Island at a speed
    virtual void set_destination_island_and_speed(std::shared_ptr<Island> destination_island, double speed);

    // Start moving on a course and speed
    virtual void set_course_and_speed(double course, double speed);

    // Stop moving
    virtual void stop();

    // dock at an Island - set our position = Island's position, go into Docked state
    virtual void dock(std::shared_ptr<Island> island_ptr);

    // Refuel - must already be docked at an island; fill takes as much as possible
    virtual void refuel();

    // Chain other Ships
    virtual void chain_all_ship();

    // Chain other Ship
    virtual void chain_ship(std::shared_ptr<Ship>);

    // Unchain a Ship
    virtual void unchain_ship(std::shared_ptr<Ship>);

    // Set an Island as the load destination
    virtual void set_load_destination(std::shared_ptr<Island>);

    // Set an Island as the unload destination
    virtual void set_unload_destination(std::shared_ptr<Island>);

    // Tell Warships to attack a target
    virtual void attack(std::shared_ptr<Ship>);

    // Tell Warships to stop attacking
    virtual void stop_attack();

private:
    std::map<std::string, std::shared_ptr<Ship_component>> ship_components;
    std::string composite_name;
};

#endif