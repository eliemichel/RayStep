// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#include "Logger.h"
#include "utils/fileutils.h"
#include <chrono>
#include <ctime>
#include <cstring>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

size_t Logger::align_width = 0;

bool EnableVTMode()
{
#ifdef _WIN32
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return false;
	}
#endif
	return true;
}

Logger::Logger(const char *func, const char *file, int line, Logger::Level level)
{
	if (Logger::align_width == 0) {
		Logger::init();
		Logger::align_width = 1;
	}

    {
        using namespace std::chrono;
        std::time_t now_c = system_clock::to_time_t(system_clock::now());
	struct tm * timeinfo = std::localtime(&now_c);
	char buffer[26];
	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
	m_ss << buffer << ": ";
    }

    switch(level)
    {
    case Logger::LDEBUG:
        m_ss << Color::bold_grey   << "DEBUG  " << Color::nocolor;
        break;
    case Logger::LVERBOSE:
        m_ss << Color::bold_white  << "VERBOSE" << Color::nocolor;
        break;
    case Logger::LINFO:
        m_ss << Color::bold_white  << "INFO   " << Color::nocolor;
        break;
    case Logger::LWARNING:
        m_ss << Color::bold_yellow << "WARNING" << Color::nocolor;
        break;
    case Logger::LERROR:
        m_ss << Color::bold_red    << "ERROR  " << Color::nocolor;
        break;
    }

    if( level == LDEBUG )
    {
        std::stringstream pos;
        pos << " (" << func << "():" << Logger::shorterFilename(file) << ":" << line << ")";
        Logger::align(pos);
        m_ss << Color::blue << pos.str() << Color::nocolor;
    }

    m_ss << " ";

    switch( level) {
    case Logger::LDEBUG:   m_ss << Color::bold_grey;    break;
    case Logger::LVERBOSE: m_ss << Color::nocolor; break;
    case Logger::LINFO:    m_ss << Color::nocolor; break;
    case Logger::LWARNING: m_ss << Color::yellow;  break;
    case Logger::LERROR:   m_ss << Color::red;     break;
    }
}

Logger::~Logger()
{
    m_ss << Color::nocolor;
    std::string str = m_ss.str();
    // trim extra newlines
    while ( str.empty() == false && str[str.length() - 1] == '\n')
        str.resize(str.length() - 1);
    std::cerr << str << std::endl;
}

const char * Logger::shorterFilename(const char *filename)
{
    const char *last_slash  = std::strrchr(filename, PATH_DELIM_CHAR);
    if ( last_slash == 0)
        return filename;
    while ( last_slash > filename && last_slash[-1] != PATH_DELIM_CHAR)
        --last_slash;
    return last_slash;
}

void Logger::align(std::stringstream &ss)
{
	size_t l = ss.str().length();
    Logger::align_width = max(Logger::align_width, l);
	size_t x = Logger::align_width;  // why is that needed?
	ss << std::setw(x - l) << "";
}

void Logger::init() {
	if (!EnableVTMode()) {
		std::cerr << "Warning: Logger could not enable Virtual Terminal mode." << std::endl;
	}
}
