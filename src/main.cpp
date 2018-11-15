
#include <iostream>
#include "app_controller.hpp"

int main(int argc, char* argv[])
{
	AppController app_ctrl {argc, argv};
	app_ctrl.run();

	return 0;
}