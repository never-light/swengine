#include "Game/Application.h"

#include <Windows.h>

int main() {
	setlocale(LC_CTYPE, "Russian");

	Application app;
	app.initialize("SpaceShooter", 640, 640);

	app.run();

	app.shutdown();

	return 0;
}