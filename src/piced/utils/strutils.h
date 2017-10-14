// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#pragma once

#include <string>

/**
 * Trim whitespaces string from start (in place)
 */
void ltrim(std::string& s);

/**
 * Trim whitespaces string from end (in place)
 */
void rtrim(std::string& s);

/**
 * Trim whitespaces string in place
 */
void trim(std::string& s);

/**
 * Convert string to lower
 */
std::string toLower(const std::string & s);

bool startsWith(const std::string & s, const std::string & postfix);

bool endsWith(const std::string & s, const std::string & postfix);

std::string replaceAll(std::string str, const std::string& search, const std::string& replace);