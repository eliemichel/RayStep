// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#include "ShaderPreprocessor.h"
#include "Logger.h"
#include "utils/strutils.h"
#include "utils/fileutils.h"

#include <fstream>
#include <map>

using namespace std;

constexpr char* BEGIN_INCLUDE_TOKEN = "// _AUGEN_BEGIN_INCLUDE";
constexpr char* END_INCLUDE_TOKEN = "// _AUGEN_END_INCLUDE";

bool ShaderPreprocessor::load(const string & filename, const vector<string> & defines) {
	return loadShaderSourceAux(filename, defines, m_lines);
}

void ShaderPreprocessor::source(vector<GLchar> & buf) const {
	size_t length = 0;
	for (string s : m_lines) {
		length += s.size() + 1;
		buf.reserve(length);
		for (char& c : s) {
			buf.push_back(c);
		}
		buf.push_back('\n');
	}
	buf.push_back(0);
}

void ShaderPreprocessor::logTraceback(size_t line) const {
	LOG << "Traceback:";
	size_t i = 0, localOffset = 0;
	string filename = "";
	vector<pair<string, size_t>> stack;
	size_t ignore = 0;
	for (auto l : m_lines) {
		//DEBUG_LOG << i << "/" << line << " | " << filename << ":" << localOffset << " | " << l;
		if (startsWith(l, BEGIN_INCLUDE_TOKEN)) {
			// Stack context
			stack.push_back(make_pair(filename, localOffset));
			if (i > line) {
				++ignore;
			}
			//DEBUG_LOG << "stack >> " << filename << ", return offset: " << localOffset << ", ignore: " << ignore;
			size_t n = string(BEGIN_INCLUDE_TOKEN).size();
			filename = l.substr(n, l.size() - n);
			localOffset = 0;
		}
		else if (startsWith(l, END_INCLUDE_TOKEN)) {
			if (!stack.empty()) {
				pair<string, size_t> p = stack.back();
				stack.pop_back();
				if (!stack.empty() && i > line) {
					if (ignore) {
						ignore--;
					}
					else {
						LOG << "Included in " << p.first << ", line " << p.second;
					}
				}
				// Unstack context
				filename = p.first;
				localOffset = p.second;
				//DEBUG_LOG << "stack << " << p.first << " : " << p.second << ", ignore: " << ignore << " (at i = " << i << ", j = " << localOffset << ")";
			}
		}
		else if (!stack.empty() && line == i) {
			LOG << "In " << filename << ", line " << Color::bold_green << (localOffset - 1) << Color::nocolor;
		}
		++i;
		++localOffset;
	}
}


// Note: no include loop check is done, beware of infinite loops
bool ShaderPreprocessor::loadShaderSourceAux(const string & filename, const vector<string> & defines, vector<string> & lines_accumulator) {
	static const string includeKeywordLower = "#include";
	static const string defineKeywordLower = "#define";
	static const string systemFilename = "sys:defines";

	if (endsWith(filename, systemFilename)) {
		lines_accumulator.push_back(string() + BEGIN_INCLUDE_TOKEN + " " + systemFilename);
		for (auto def : defines) {
			lines_accumulator.push_back(defineKeywordLower + " " + def);
		}
		lines_accumulator.push_back(string() + END_INCLUDE_TOKEN + " " + systemFilename);
		return true;
	}

	lines_accumulator.push_back(string() + BEGIN_INCLUDE_TOKEN + " " + filename);

	ifstream in(filename);
	if (!in.is_open()) {
		WARN_LOG << "Unable to open file: " << filename;
		return false;
	}

	string line;

	size_t i = 0;
	while (getline(in, line)) {
		++i;
		// Poor man's #include directive parser
		if (startsWith(toLower(line), includeKeywordLower)) {
			string includeFilename = line.substr(includeKeywordLower.size());
			trim(includeFilename);
			if (includeFilename[0] != '"' || includeFilename[includeFilename.size() - 1] != '"') {
				ERR_LOG << "Syntax error in #include directive at line " << i << " in file " << filename;
				ERR_LOG << "  filename is expected to be enclosed in double quotes (\")";
				return false;
			}
			includeFilename = includeFilename.substr(1, includeFilename.size() - 2);
			string fullIncludeFilename = joinPath(baseDir(filename), includeFilename);
			if (!loadShaderSourceAux(fullIncludeFilename, defines, lines_accumulator)) {
				ERR_LOG << "Include error at line " << i << " in file " << filename;
				return false;
			}
		}
		else {
			lines_accumulator.push_back(line);
		}
	}
	lines_accumulator.push_back(END_INCLUDE_TOKEN);
	return true;
}
