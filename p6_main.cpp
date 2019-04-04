#include "Controller.h"
#include <iostream>
#include <exception>

using namespace std;

// The main function creates the Controller object, then tells it to run.

int main()
{
	// Set output to show two decimal places
	//	cout << fixed << setprecision(2) << endl;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	try {
		// create the Controller and go
		Controller controller;

		controller.run();
	}
	// catch all exceptions not handled by Controller
	catch (std::exception& error) {
		cout << error.what() << endl;
	}
	catch (...) {
		cout << "Unknown exception caught" << endl;
	}

	return 0;
}
