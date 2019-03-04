#pragma once

#include <string>

/*!
 * \brief Class for messages logging
 *
 */
class Logger {
public:
	Logger();
	virtual ~Logger();

	/*!
	 * \brief Adds message to the log
	 * 
	 * \param message message text
	 */
	virtual void log(const std::string& message);
};