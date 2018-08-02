#pragma once

#include <string>

enum class MessageBoxButtons {
	OK
};

enum class MessageBoxType {
	Default, Error
};

class IOUtils {
public:
	static void copyToClipboard(const std::string& string);

	static void showMessageBox(const std::string& caption, const std::string& message,
		MessageBoxButtons buttons = MessageBoxButtons::OK, MessageBoxType type = MessageBoxType::Default);
private:
	IOUtils() = delete;
	~IOUtils() = delete;

	IOUtils(const IOUtils& other) = delete;
};