CC = g++
LD = g++

CFLAGS = -c -pedantic-errors -std=c++14 -Wall
LFLAGS = -pedantic-errors -Wall

OBJS = p6_main.o Chain_ship.o Controller.o Cruise_ship.o Cruiser.o Geometry.o Island.o Local_view.o Map_view.o Model.o Navigation.o Sailing_view.o Ship.o Ship_component.o Ship_composite.o Ship_component_factory.o Sim_object.o Tanker.o Torpedo_boat.o Track_base.o Twod_view.o Utility.o View.o Warship.o
PROG = p6exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p6_main.o: p6_main.cpp Controller.h
	$(CC) $(CFLAGS) p6_main.cpp

Controller.o: Controller.cpp Controller.h 
	$(CC) $(CFLAGS) Controller.cpp

Chain_ship.o: Chain_ship.cpp Chain_ship.h Ship.h
	$(CC) $(CFLAGS) Chain_ship.cpp

Cruise_ship.o: Cruise_ship.cpp Cruise_ship.h Ship.h
	$(CC) $(CFLAGS) Cruise_ship.cpp

Cruiser.o: Cruiser.cpp Cruiser.h Warship.h
	$(CC) $(CFLAGS) Cruiser.cpp

Geometry.o: Geometry.cpp Geometry.h 
	$(CC) $(CFLAGS) Geometry.cpp

Island.o: Island.cpp Island.h Geometry.h
	$(CC) $(CFLAGS) Island.cpp

Local_view.o: Local_view.cpp Local_view.h Twod_view.h
	$(CC) $(CFLAGS) Local_view.cpp

Map_view.o: Map_view.cpp Map_view.h Twod_view.h
	$(CC) $(CFLAGS) Map_view.cpp

Model.o: Model.cpp Model.h 
	$(CC) $(CFLAGS) Model.cpp

Navigation.o: Navigation.cpp Navigation.h  
	$(CC) $(CFLAGS) Navigation.cpp

Sailing_view.o: Sailing_view.cpp Sailing_view.h View.h
	$(CC) $(CFLAGS) Sailing_view.cpp

Ship.o: Ship.cpp Ship.h Geometry.h Ship_component.h Sim_object.h Track_base.h 
	$(CC) $(CFLAGS) Ship.cpp

Ship_component.o: Ship_component.cpp Ship_component.h
	$(CC) $(CFLAGS) Ship_component.cpp

Ship_composite.o: Ship_composite.cpp Ship_composite.h Ship_component.h
	$(CC) $(CFLAGS) Ship_composite.cpp

Ship_component_factory.o: Ship_component_factory.cpp Ship_component_factory.h
	$(CC) $(CFLAGS) Ship_component_factory.cpp

Sim_object.o: Sim_object.cpp Sim_object.h
	$(CC) $(CFLAGS) Sim_object.cpp

Tanker.o: Tanker.cpp Tanker.h Ship.h
	$(CC) $(CFLAGS) Tanker.cpp

Torpedo_boat.o: Torpedo_boat.cpp Torpedo_boat.h Warship.h
	$(CC) $(CFLAGS) Torpedo_boat.cpp

Track_base.o: Track_base.cpp Track_base.h Geometry.h Navigation.h
	$(CC) $(CFLAGS) Track_base.cpp

Twod_view.o: Twod_view.cpp Twod_view.h View.h
	$(CC) $(CFLAGS) Twod_view.cpp

View.o: View.cpp View.h Geometry.h
	$(CC) $(CFLAGS) View.cpp

Warship.o: Warship.cpp Warship.h Ship.h
	$(CC) $(CFLAGS) Warship.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe
