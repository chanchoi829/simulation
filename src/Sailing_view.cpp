#include "Sailing_view.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Print Sailing data for all Ships.
void Sailing_view::draw() {
	cout << "----- Sailing Data -----" << endl;
	cout << "      Ship      Fuel    Course     Speed" << endl;

	std::map<std::string, Ship_info>ship_info = get_ship_info_map();
	for (const auto& info_pair : ship_info) {
		cout << setw(10) << info_pair.first;
		cout << setw(10) << info_pair.second.fuel;
		cout << setw(10) << info_pair.second.course;
		cout << setw(10) << info_pair.second.speed << endl;
	}
}