/*
Ship_component serves as an interface for Ship and Ship_composite
to derive from so that Ship and Ship_composite can be treated as
Ship_component type by the Controller. Since Ship and Ship_composite
can be treated as Ship_component, Ship_composite is allowed to contain
both Ships and Ship_composites in its container.
*/

#ifndef SHIP_COMPONENT_H
#define SHIP_COMPONENT_H

#include <memory>
#include <string>

class Island;
class Ship;
struct Point;

class Ship_component
{
public:
    // Returns an index counter which is used to indicate
    // in which level a Ship_component exists in a group hierarchy.
    // e.g. when index_count is 0, Ship_component is a top group, and when
    // index_count is 1, it is right below a top group.
    static int& index_counter();

    // Always return nullptr
    virtual std::shared_ptr<Ship_component> find_ship_composite(const std::string& name);

    // Always return false
    virtual bool find_and_remove_ship_composite(const std::string& name);

    // Always return false
    virtual bool check_if_ship_exists(const std::string& ship) const;

    /*** Fat Interface Functioins ***/

    // Every function below always throws an Error.
    // These functions are not meant to be called.
    virtual void add_component(std::shared_ptr<Ship_component>);

    virtual void remove_component(std::shared_ptr<Ship_component>);

    virtual void describe_component() const;

    virtual std::string get_name() const;

    virtual void set_destination_position_and_speed(Point destination_position, double speed);

    virtual void set_destination_island_and_speed(std::shared_ptr<Island> destination_island, double speed);

    virtual void set_course_and_speed(double course, double speed);

    virtual void stop();

    virtual void dock(std::shared_ptr<Island> island_ptr);

    virtual void refuel();

    virtual void chain_all_ship();

    virtual void chain_ship(std::shared_ptr<Ship>);

    virtual void unchain_ship(std::shared_ptr<Ship>);

    virtual void set_load_destination(std::shared_ptr<Island>);

    virtual void set_unload_destination(std::shared_ptr<Island>);

    virtual void attack(std::shared_ptr<Ship>);

    virtual void stop_attack();

protected:
    // Protected constructor for quasi-abstractness
    Ship_component()
    { }
};

#endif