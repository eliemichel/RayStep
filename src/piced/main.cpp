
#include <iostream>
#include <sstream>
#include <iomanip>

#include <json.hpp>
using json = nlohmann::json;

#include "ResourceManager.h"
#include "Future.h"

Uri imageUri(size_t i) {
	std::ostringstream oss;
	std::ostringstream padded;
	padded << std::setw(4) << std::setfill('0') << i;
	oss << "data/Meshes/Grain/ws_normal/view" << padded.str() << ".png";
	return oss.str();
}
constexpr size_t nbImages = 128;


int main(int argc, char **argv) {
	Uri::setDefaultBasePath("E:/SourceCode/Augen.js/src");
	Uri gltfAssetUri = "data/BusterDrone/busterDrone.gltf";

	auto fGltfAsset = ResourceManager::fetch(gltfAssetUri).asJson();

	auto fBuffers = then(fGltfAsset, [gltfAssetUri](json gltfAsset) {
		std::vector<json> buffersInfo = gltfAsset["buffers"];
		std::vector<std::future<Buffer>> buffers;
		buffers.reserve(buffersInfo.size());
		for (size_t i = 0; i < buffersInfo.size(); ++i) {
			std::string s = buffersInfo[i]["uri"];
			Uri uri(s, gltfAssetUri.parentDir());
			buffers.push_back(ResourceManager::fetch(uri).asBuffer());
		}
		return all(buffers);
	});

	auto fTest = ResourceManager::fetch(gltfAssetUri).asBuffer();
	auto fTest2 = then(fTest, [](Buffer buf) {
		return buf.size();
	});
	auto fTest3 = then(fTest, fTest2, [](Buffer test, size_t test2) {
		return test;
	});

	auto done = then(fBuffers, fTest3, [](std::vector<Buffer> buffers, Buffer test) {
		LOG << buffers[0].size();
	});
	
	try { done.get(); }
	catch (const std::invalid_argument & e) { ERR_LOG << "[InvalidArgument] " << e.what(); }
	catch (std::exception const& e) { ERR_LOG << "[Exception] " << e.what(); }
	catch (...) { ERR_LOG << "an error occured"; }

	system("pause");
	return 0;
}

