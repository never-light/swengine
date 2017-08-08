#include <Windows.h>
#include "Game/Application.h"

int main() {
	setlocale(LC_CTYPE, "Russian");

	Application app;
	app.initialize("TestApplication", 640, 640);
	app.run();
	app.shutdown();

	return 0;
}