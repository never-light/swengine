#include <Windows.h>
#include "Game/Game.h"

#include <Engine\Utils\io.h>
#include "Exceptions\FatalErrorException.h"

using namespace std::string_literals;

void showFatalErrorBox(const std::string& message) {
	IOUtils::showMessageBox("Fatal error", message, MessageBoxButtons::OK, MessageBoxType::Error);
}

int main() {
	setlocale(LC_CTYPE, "Russian");

	try {
		Game game("Labyrinth", 1280, 768);
		game.run();
	}
	catch (const EngineException& exception) {
		std::string message = exception.what();

		if (!message.empty())
			message += "\n";

		message += "Code file: "s + exception.getFile() + "\n" +
			"Function: " + exception.getFunction() + " [Line: " + std::to_string(exception.getLine()) + "]";

		showFatalErrorBox(message);
	}
	catch (const std::exception& exception) {
		showFatalErrorBox(exception.what());
	 }
	catch (...) {
		showFatalErrorBox("Unknown internal error");
	}

	return 0;
}