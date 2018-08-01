#include <Windows.h>
#include "Game/Game.h"

int main() {
	setlocale(LC_CTYPE, "Russian");

	Game game("Labyrinth", 1324, 768);
	game.run();

	return 0;
}