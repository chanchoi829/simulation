# Ship Simulation
Ship Simulation is a fun C++ simulation program!
Note: this README is still WIP

### Build Instructions
Git clone:
```bash
$ git clone https://github.com/chanchoi829/simulation.git
$ cd simulation
```

Make and Run:
```bash
$ make
$ ./simulation
```

### Ships
A Ship has a name, initial position, amount of fuel, and parameters that govern its movement.
The initial amount of fuel is equal to the supplied fuel capacity - a full fuel tank.
A Ship can be commanded to move to either a position, and Island, or follow a course, or stop,
dock at or refuel at an Island. It consumes fuel while moving, and becomes immobile
if it runs out of fuel. It inherits the Sim_object interface to the rest of the system,
and the Track_base class provides the basic movement functionality, with the unit of time
corresponding to 1.0 for one "tick" - an hour of simulated time. The speeds and rates
are specified as per unit time, but in this project, the update time is always 1.0.

The update function updates the position and/or state of the ship.
The describe function outputs information about the ship state.
Accessors make the ship state available to either the public or to derived classes.
The is a "fat interface" for the capabilities of derived types of Ships. These
functions are implemented in this class to throw an Error exception.

**Chain_ship**:
```
A Chain_ship can be told to chain anoter Ship in the simulation 
world. When commanded to set_destination_island, set_destination_position,
or set_course_and_speed, a Chain_ship also tells its chained Ships 
to also perform the same task. 

Initial values:
fuel capacity and initial amount 1500 tons, maximum speed 10., 
fuel consumption 4.tons/nm, resistance 1.
```

**Cruiser**:
```
A Cruiser is a Ship with moderate speed, firepower, and resistance.
When told to attack it will start firing at the target if it is in range.
At each update, it will stop attacking if the target is either no longer afloat
(i.e. is sinking or sunk), or is out of range. As long as the target is both afloat
and in range, it will keep firing at it.

Initial values:
fuel capacity and initial amount: 1000, maximum speed 20., fuel consumption 10.tons/nm,
resistance 6, firepower 3, maximum attacking range 15
```

**Cruise_ship**:
```
A Cruise_ship can automatically visit all of the islands in a 
leisurely fashion. It behaves like a normal ship until you
tell it to go to an island with the "destination" command.

Initial values:
fuel capacity and initial amount 500 tons, maximum speed 15., 
fuel consumption 2.tons/nm, resistance 0.
```

**Tanker**:
```
A Tanker is a ship with a large corgo capacity for fuel.
It can be told an Island to load fuel at, and an Island to unload at.
Once it is sent to the loading destination, it will start shuttling between
the loading and unloading destination. At the loading destination,
it will first refuel then wait until its cargo hold is full, then it will
go to the unloading destination.

Initial values:
fuel capacity and initial amount 100 tons, maximum speed 10., fuel consumption 2.tons/nm,
resistance 0, cargo capacity 1000 tons, initial cargo is 0 tons.
```

**Torpedo_boat**:
```
A Torpedo_boat is similar to a Cruiser, but when commanded to attack, a Torpedo_boat
closes with its target by changing its course on every update. When it is close enough,
it fires at the target and continues until the target is sunk like Cruiser. However,
if a Torpedo_boat is fired upon, instead of counter-attacking like Cruiser, it runs away
to an Island of refuge.

Initial values:
fuel capacity and initial amount: 800, maximum speed 12., fuel consumption 5.tons/nm,
resistance 9, firepower 3, maximum attacking range 5
```

### Views
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

**Local_view**:
```
Local_view shows a local view of a Ship
```
**Map_view**:
```
Map_view shows a map of all simulation objects
```
**Sailing_view**:
```
Sailing_view shows data of all Ships
```

### Commands

**View Commands**:
```
open_map_view - create the Map View and attach to the Model.

close_map_view - close the Map View

open_sailing_view - open the Sailing View

close_sailing_view - close the Sailing View

open_local_view - open a Local View of a Ship

close_local_view - close a Local View of a Ship

default - restore the default settings of the map

size - read an integer for the size of the map

zoom - read a double for the scale of the Map

pan - read double values for  the origin of the Map

show - tell the Map View to draw the Map
```

**Ship Commands**:
```
course - read a compass heading and a speed (doubles)
Check: 0.0 <= compass heading < 360.0, speed >= 0.0

position - read an (x, y) position and a speed (doubles) for a Ship to go to
Check: x, y can have any value, speed >= 0.0

destination - read an Island name and a speed (double) for a Ship to go to
Check:  Island must exist, speed >= 0.0

load_at - tell a Tanker to load at an Island
Check: Island must exist

unload_at - tell a Tanker to unload at an Island
Check: Island must exist

chain_all - tell a Chain_ship to chain all other Ships

chain - tell a Chain_ship to chain a Ship

unchain - tell a Chain_ship to unchain its Ship

dock_at - tell a Ship to dock at an Island
Check: Island must exist

attack - tell a Warship to attack a Ship
Check: Ship must exist

refuel - tell a Ship to refuel

stop - tell a Ship to stop whatever it's doing

stop_attack - tell a Warship to stop attacking
```

**Model Commands**:
```
quit - quit the program

status - tell all objects to describe themselves

go - call the Model::update() function to update the status of all objects

create - create a new Ship

create_group - create a group

remove_group - remove a group

remove_ship_from_group - remove a certain Ship from a group

add_ship_to_group - add a Ship to a group

add_group_to_group - add a group to a group

describe_groups - describe all groups

```

### Example Usage
```
Time 0: Enter command: status

Cruiser Ajax at (15.00, 15.00), fuel: 1000.00 tons, resistance: 6
Stopped

Island Bermuda at position (20.00, 20.00)
Fuel available: 0.00 tons

Island Exxon at position (10.00, 10.00)
Fuel available: 1000.00 tons

Island Shell at position (0.00, 30.00)
Fuel available: 1000.00 tons

Island Treasure_Island at position (50.00, 5.00)
Fuel available: 100.00 tons

Tanker Valdez at (30.00, 30.00), fuel: 100.00 tons, resistance: 0
Stopped
Cargo: 0.00 tons, no cargo destinations

Cruiser Xerxes at (25.00, 25.00), fuel: 1000.00 tons, resistance: 6
Stopped

Time 0: Enter command: open_map_view

Time 0: Enter command: open_sailing_view

Time 0: Enter command: create James Torpedo_boat 14 2

Time 0: Enter command: open_local_view James

Time 0: Enter command: James attack Ajax
James will attack Ajax

Time 0: Enter command: Valdez load_at Shell
Valdez will load at Shell

Time 0: Enter command: show
Display size: 25, scale: 2.00, origin: (-10.00, -10.00)
Treasure_Island outside the map
  38 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  32 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . Sh. . . . . . . . . . . . . . Va. . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  26 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . Xe. . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  20 . . . . . . . . . . . . . . . Be. . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  14 . . . . . . . . . . . . Aj. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . Ex. . . . . . . . . . . . . .
   8 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
   2 . . . . . . . . . . . . Ja. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  -4 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
 -10 . . . . . . . . . . . . . . . . . . . . . . . . .
   -10    -4     2     8    14    20    26    32    38
----- Sailing Data -----
      Ship      Fuel    Course     Speed
      Ajax   1000.00      0.00      0.00
     James    800.00      0.00      0.00
    Valdez    100.00      0.00      0.00
    Xerxes   1000.00      0.00      0.00
Local view for James at position (14.00, 2.00)
     . . Ex. . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . Ja. . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .

Time 0: Enter command: go
Ajax stopped at (15.00, 15.00)
Island Exxon now has 1200.00 tons
James stopped at (14.00, 2.00)
James is attacking
James will sail on course 4.40 deg, speed 12.00 nm/hr to (15.00, 15.00)
Island Shell now has 1200.00 tons
Island Treasure_Island now has 105.00 tons
Valdez stopped at (30.00, 30.00)
Xerxes stopped at (25.00, 25.00)

Time 1: Enter command: show
Display size: 25, scale: 2.00, origin: (-10.00, -10.00)
Treasure_Island outside the map
  38 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  32 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . Sh. . . . . . . . . . . . . . Va. . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  26 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . Xe. . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  20 . . . . . . . . . . . . . . . Be. . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  14 . . . . . . . . . . . . Aj. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . Ex. . . . . . . . . . . . . .
   8 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
   2 . . . . . . . . . . . . Ja. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  -4 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
 -10 . . . . . . . . . . . . . . . . . . . . . . . . .
   -10    -4     2     8    14    20    26    32    38
----- Sailing Data -----
      Ship      Fuel    Course     Speed
      Ajax   1000.00      0.00      0.00
     James    800.00      4.40     12.00
    Valdez    100.00      0.00      0.00
    Xerxes   1000.00      0.00      0.00
Local view for James at position (14.00, 2.00)
     . . Ex. . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . Ja. . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .

Time 1: Enter command: go
Ajax stopped at (15.00, 15.00)
Island Exxon now has 1400.00 tons
James now at (14.92, 13.96)
James is attacking
James fires
Ajax hit with 3, resistance now 3
Ajax will attack James
Island Shell now has 1400.00 tons
Island Treasure_Island now has 110.00 tons
Valdez stopped at (30.00, 30.00)
Xerxes stopped at (25.00, 25.00)

Time 2: Enter command: show
Display size: 25, scale: 2.00, origin: (-10.00, -10.00)
Treasure_Island outside the map
  38 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  32 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . Sh. . . . . . . . . . . . . . Va. . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  26 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . Xe. . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  20 . . . . . . . . . . . . . . . Be. . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  14 . . . . . . . . . . . . Aj. . . . . . . . . . . .
     . . . . . . . . . . . . Ja. . . . . . . . . . . .
     . . . . . . . . . . Ex. . . . . . . . . . . . . .
   8 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
   2 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  -4 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
 -10 . . . . . . . . . . . . . . . . . . . . . . . . .
   -10    -4     2     8    14    20    26    32    38
----- Sailing Data -----
      Ship      Fuel    Course     Speed
      Ajax   1000.00      0.00      0.00
     James    740.00      4.40     12.00
    Valdez    100.00      0.00      0.00
    Xerxes   1000.00      0.00      0.00
Local view for James at position (14.92, 13.96)
     . . . . . . . . .
     . . . . . . . Be.
     . . . . . . . . .
     . . . . Aj. . . .
     . . . . Ja. . . .
     . . . . . . . . .
     . . Ex. . . . . .
     . . . . . . . . .
     . . . . . . . . .

Time 2: Enter command: go
Ajax stopped at (15.00, 15.00)
Ajax is attacking
Ajax fires
James hit with 3, resistance now 6
James taking evasive action
James stopping attack
James will sail on course 317.06 deg, speed 12.00 nm/hr to Shell
Island Exxon now has 1600.00 tons
James now at (6.75, 22.75)
Island Shell now has 1600.00 tons
Island Treasure_Island now has 115.00 tons
Valdez stopped at (30.00, 30.00)
Xerxes stopped at (25.00, 25.00)

Time 3: Enter command: show
Display size: 25, scale: 2.00, origin: (-10.00, -10.00)
Treasure_Island outside the map
  38 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  32 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . Sh. . . . . . . . . . . . . . Va. . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  26 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . Xe. . . . . . .
     . . . . . . . . Ja. . . . . . . . . . . . . . . .
  20 . . . . . . . . . . . . . . . Be. . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  14 . . . . . . . . . . . . Aj. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . Ex. . . . . . . . . . . . . .
   8 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
   2 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  -4 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
 -10 . . . . . . . . . . . . . . . . . . . . . . . . .
   -10    -4     2     8    14    20    26    32    38
----- Sailing Data -----
      Ship      Fuel    Course     Speed
      Ajax   1000.00      0.00      0.00
     James    680.00    317.06     12.00
    Valdez    100.00      0.00      0.00
    Xerxes   1000.00      0.00      0.00
Local view for James at position (6.75, 22.75)
     . Sh. . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . Ja. . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . .
     . . . . . . . . Aj

Time 3: Enter command: status

Cruiser Ajax at (15.00, 15.00), fuel: 1000.00 tons, resistance: 3
Stopped
Attacking James

Island Bermuda at position (20.00, 20.00)
Fuel available: 0.00 tons

Island Exxon at position (10.00, 10.00)
Fuel available: 1600.00 tons

Torpedo_boat James at (6.75, 22.75), fuel: 680.00 tons, resistance: 6
Moving to Shell on course 317.06 deg, speed 12.00 nm/hr

Island Shell at position (0.00, 30.00)
Fuel available: 1600.00 tons

Island Treasure_Island at position (50.00, 5.00)
Fuel available: 115.00 tons

Tanker Valdez at (30.00, 30.00), fuel: 100.00 tons, resistance: 0
Stopped
Cargo: 0.00 tons, no cargo destinations

Cruiser Xerxes at (25.00, 25.00), fuel: 1000.00 tons, resistance: 6
Stopped

Time 3: Enter command: quit
Done
```