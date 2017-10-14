// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace Color
{
#ifdef _WIN32
	static const std::string CSI = "\x1b[";
#else
	static const std::string CSI = "\033[";
#endif
	const std::string bold_grey = CSI + "1;30m";
	const std::string bold_red = CSI + "1;31m";
	const std::string bold_green = CSI + "1;32m";
	const std::string bold_yellow = CSI + "1;33m";
	const std::string bold_blue = CSI + "1;34m";
	const std::string bold_purple = CSI + "1;35m";
	const std::string bold_lightblue = CSI + "1;36m";
	const std::string bold_white = CSI + "1;37m";

	const std::string black = CSI + "0;30m";
	const std::string red = CSI + "0;31m";
	const std::string green = CSI + "0;32m";
	const std::string yellow = CSI + "0;33m";
	const std::string blue = CSI + "0;34m";
	const std::string purple = CSI + "0;35m";
	const std::string lightblue = CSI + "0;36m";
	const std::string white = CSI + "0;37m";

	const std::string nocolor = CSI + "0m";
} // namespace Color


class Logger {
public:
    enum Level
    {
        LDEBUG,
        LVERBOSE,
        LINFO,
        LWARNING,
        LERROR
    };

public:
    Logger(const char *func, const char *file, int line, enum Level level = LINFO);
    ~Logger();

    inline std::ostream &stream() { return m_ss; }

private:
    static const char * shorterFilename(const char *filename);
    static void align(std::stringstream &ss);
	static void init();

private:
    std::ostringstream m_ss;

private:
    static size_t align_width;
};

#define DEBUG_LOG Logger(__func__, __FILE__, __LINE__, Logger::LDEBUG).stream()
#define LOG Logger(__func__, __FILE__, __LINE__, Logger::LINFO).stream()
#define WARN_LOG Logger(__func__, __FILE__, __LINE__, Logger::LWARNING).stream()
#define ERR_LOG Logger(__func__, __FILE__, __LINE__, Logger::LERROR).stream()

