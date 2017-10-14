// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#include "utils/strutils.h"

#include <string>
#include <algorithm>
#include <functional>
#include <cctype>

// Credit: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring#217605
// trim from start (in place)
void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

std::string toLower(const std::string & s) {
	std::string r = s;
	std::transform(r.begin(), r.end(), r.begin(), ::tolower);
	return r;
}

bool startsWith(const std::string & s, const std::string & prefix) {
	if (prefix.size() > s.size()) return false;
	return equal(prefix.begin(), prefix.end(), s.begin());
}

bool endsWith(const std::string & s, const std::string & postfix) {
	if (postfix.size() > s.size()) return false;
	return equal(postfix.rbegin(), postfix.rend(), s.rbegin());
}

std::string replaceAll(std::string str, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = str.find(search, pos)) != std::string::npos) {
		str.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return str;
}
