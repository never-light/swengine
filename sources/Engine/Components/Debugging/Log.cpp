#include "Log.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

Log::Log(std::ostream& stream = std::cout, LogType type = LOG_INFO)
	: m_stream(stream), m_type(type) {
	if (m_type == LOG_ERRORS) {
		m_stream << "[ERROR]";
	}

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	m_stream << "[" << std::put_time(std::localtime(&in_time_t), "%X") << "] ";
}

Log::Log(const Log& debug) : m_stream(debug.m_stream), m_type(debug.m_type) {

}

Log::~Log() {
	m_stream << std::endl;
}

Log errlog() {
	return Log(std::cout, LOG_ERRORS);
}

Log infolog() {
	return Log(std::cout, LOG_INFO);
}