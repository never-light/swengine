#include <Windows.h>
#include "Game/Application.h"

int main() {
	setlocale(LC_CTYPE, "Russian");

	Application app("TestApplication", 1024, 768);
	app.run();

	return 0;
}