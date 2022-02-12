#ifndef SHIP_COMPONENT_FACTORY_H
#define SHIP_COMPONENT_FACTORY_H

#include <memory>
#include <string>

struct Point;
class Ship;
class Ship_component;
/* This is a very simple form of factory, a function; you supply the information, it creates
the specified kind of object and returns a pointer to it. The Ship is allocated
with new, so some other component is resposible for deleting it.
*/

// may throw Error("Trying to create ship of unknown type!")
std::shared_ptr<Ship> create_ship(const std::string& name, const std::string& type, Point initial_position);

std::shared_ptr<Ship_component> create_composite(const std::string& name);

#endif