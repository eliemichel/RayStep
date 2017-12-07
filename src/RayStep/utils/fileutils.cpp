// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#include "utils/fileutils.h"

#include <algorithm>
#include <string>

using namespace std;

string baseDir(const string & path) {
	size_t pos = path.find_last_of(PATH_DELIM);
	return pos != string::npos ? path.substr(0, pos) : "";
}
string shortFileName(const string & path) {
	size_t pos = path.find_last_of(PATH_DELIM);
	return pos != string::npos ? path.substr(pos + 1) : path;
}

std::string fixPath(const std::string & path) {
	std::string p = path;
	p = replaceAll(p, "/", PATH_DELIM);
	p = replaceAll(p, "\\", PATH_DELIM);
	return p;
}

bool isFile(const std::string & filename) {
	FILE *file = fopen(filename.c_str(), "r");
	if (file) {
		fclose(file);
		return true;
	}
	return false;
}
