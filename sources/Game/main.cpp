#include <Windows.h>
#include "Game/Game.h"

#include <Engine\Utils\io.h>
#include "Exceptions\FatalErrorException.h"

#define ENABLE_EXCEPTIONS_REPORTING 1
#define DISABLE_EXCEPTIONS_REPORTING 0

#define EXCEPTIONS_REPORTING_MODE ENABLE_EXCEPTIONS_REPORTING

using namespace std::string_literals;

void showFatalErrorBox(const std::string& message) {
	IOUtils::showMessageBox("Fatal error", message, MessageBoxButtons::OK, MessageBoxType::Error);
}

int main() {
	setlocale(LC_CTYPE, "Russian");

#if EXCEPTIONS_REPORTING_MODE == ENABLE_EXCEPTIONS_REPORTING
	try {
#endif
		Game game("Labyrinth", 1280, 720);
		game.run();

#if EXCEPTIONS_REPORTING_MODE == ENABLE_EXCEPTIONS_REPORTING
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
#endif

	return 0;
}