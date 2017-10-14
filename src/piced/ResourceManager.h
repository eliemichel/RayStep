
#include "utils/strutils.h"
#include "Logger.h"

extern "C" {
#include <upng.h>
}
#include <json.hpp>
using json = nlohmann::json;

#include <fstream>
#include <stdexcept>
#include <regex>

#include "utils/fileutils.h"
#include "Future.h"

class Uri {
	// TODO: see rfc3986
public:
	Uri(const std::string & str, const std::string & base = s_defaultBasePath) : m_raw(str), m_basePath(base) { parse(); }
	Uri(const char* str) : m_raw(str), m_basePath(s_defaultBasePath) { parse(); }
    bool isDataUri() const;

	void setBasePath(const std::string & basePath);
    std::string absoluteFilename() const;
	std::string parentDir() const;

public:
	static void setDefaultBasePath(const std::string & basePath);

private:
	void parse();
	static std::regex s_uriExpr;
	static std::string s_defaultBasePath;

private:
    std::string m_raw;
	std::string m_scheme;
	std::string m_filename;
	std::string m_basePath;
};
std::regex Uri::s_uriExpr("^([:alnum:]+:)?(.*)$");
std::string Uri::s_defaultBasePath("");

bool Uri::isDataUri() const {
    return m_scheme == "data";
}

void Uri::setBasePath(const std::string & basePath) {
	m_basePath = basePath;
}

std::string Uri::absoluteFilename() const {
	if (m_scheme != "file") {
		throw std::invalid_argument("Cannot get absolute filename from non-file uri: " + m_raw);
	}
	return fixPath(joinPath(m_basePath, m_filename));
}

std::string Uri::parentDir() const {
	return baseDir(absoluteFilename());
}

void Uri::setDefaultBasePath(const std::string & basePath) {
	s_defaultBasePath = basePath;
}

void Uri::parse() {
	std::smatch match;
	if (!std::regex_match(m_raw, match, s_uriExpr)) {
		throw std::invalid_argument("Uri misformed: " + m_raw);
	}
	m_scheme = match[1].str().substr(0, -1);
	if (m_scheme.empty()) {
		m_scheme = "file";
	}
	m_filename = match[2].str();
}



typedef upng_t* Image;
typedef std::vector<char> Buffer;


class Resource {
public:
    void load(const Uri & uri);

	std::future<Image> asImage();
    std::future<json> asJson();
    std::future<Buffer> asBuffer();

private:
    static Buffer loadFile(const std::string & filename);

private:
    std::future<Buffer> m_content;
};

void Resource::load(const Uri & uri) {
    m_content = std::async(loadFile, uri.absoluteFilename());
}

std::future<Image> Resource::asImage() {
    return then(m_content, [](Buffer & content) {
		const unsigned char* data = reinterpret_cast<unsigned char*>(&content[0]);
		unsigned long length = static_cast<unsigned long>(content.size());
		Image img = upng_new_from_bytes(data, length);
		upng_decode(img);
		return img;
    });
}

std::future<json> Resource::asJson() {
	return then(m_content, [](Buffer & content) {
		return json::parse(content.begin(), content.end());
	});
}

std::future<Buffer> Resource::asBuffer() {
	return std::move(m_content);
}

Buffer Resource::loadFile(const std::string & filename) {
    std::ifstream in(filename.c_str(), std::ios::binary|std::ios::ate);
	if (!in.is_open()) {
		throw std::invalid_argument("file not found: " + filename);
	}

	std::ifstream::pos_type len = in.tellg();
	Buffer result(static_cast<size_t>(len));

    in.seekg(0, std::ios::beg);
    in.read(&result[0], len);

    return result;
}


class ResourceCache {
    // TODO
public:
    bool hit(const Uri & key, Resource & value) { return false; }
};


class ResourceManager {
public:
    static Resource fetch(const Uri & uri);

private:
    static ResourceCache s_cache;
};
ResourceCache ResourceManager::s_cache;

Resource ResourceManager::fetch(const Uri & uri) {
    Resource resource;

    if (uri.isDataUri() || !s_cache.hit(uri, resource)) {
        resource.load(uri);
    }

    return resource;
}

