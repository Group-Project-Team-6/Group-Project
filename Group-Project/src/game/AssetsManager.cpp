#include "AssetsManager.h"
#include "../common/TextureLoader.h"

RendererBase* AssetsManager::renderer = nullptr;
std::map<std::string, TextureBase*> AssetsManager::texturePool;
std::map<std::string, ShaderBase*> AssetsManager::shaderPool;
std::map<std::string, MeshGeometry*> AssetsManager::meshPool;

AssetsManager::~AssetsManager() {
	for (auto& it = texturePool.begin(); it != texturePool.end(); it++) {
		if(it->second) delete(it->second);
	}
	texturePool.clear();
	for (auto& it = shaderPool.begin(); it != shaderPool.end(); it++) {
		if (it->second) delete(it->second);
	}
	shaderPool.clear();
	for (auto& it = meshPool.begin(); it != meshPool.end(); it++) {
		if (it->second) delete(it->second);
	}
	meshPool.clear();


}
void AssetsManager::LoadAssets(std::string configFileName) {
	std::ifstream configFile(Assets::DATADIR + configFileName);
	std::string line;
	auto f = [&]() -> std::map<std::string, std::string> {
		std::map<std::string, std::string> map;
		std::string input;
		configFile >> input;
		if (input != "{") return map ;
		std::string name = "";
		std::string FileName = "";
		while (name != "}") {
			configFile >> name;
			configFile >> FileName;
			map[name] = FileName;
		}
		return map;
	};

	if (configFile.is_open())
	{
		while (!configFile.eof()) {
			std::string input;
			configFile >> input;
			std::map<std::string, std::string> map;
			map = f();
			if (input == "Texture") {
				for (auto& it = map.begin(); it != map.end(); it++) {
					LoadTextureFromFile(it->first,it->second);
				}
			}else if (input == "Shader") {
				for (auto& it = map.begin(); it != map.end(); it++) {
					LoadShaderFromFile(it->first, it->second);
				}
			}
			else if (input == "Mesh") {
				for (auto& it = map.begin(); it != map.end(); it++) {
					LoadMeshFromFile(it->first, it->second);
				}
			}
		}
		configFile.close();
	}
}

void AssetsManager::LoadTextureFromFile(std::string name, std::string fileName) {
	if (renderer) {
		TextureBase* t = TextureLoader::LoadAPITexture(fileName);
		if (t) LoadTexture(name,t);
	}
}
void AssetsManager::LoadShaderFromFile(std::string name, std::string fileName) {
	if (renderer) {
		ShaderBase* s = renderer->LoadShader(fileName);
		if (s) LoadShader(name,s);
	}
}
void AssetsManager::LoadMeshFromFile(std::string name, std::string fileName) {
	if (renderer) {
		MeshGeometry* m = renderer->LoadMesh(fileName);
		if (m) LoadMesh(name,m);
	}
}

void AssetsManager::LoadTexture(std::string name, TextureBase* texture) {
	if (texturePool[name] != nullptr) return;
	if (texture) texturePool[name] = texture;
}
void AssetsManager::LoadShader(std::string name, ShaderBase* shader) {
	if (shaderPool[name] != nullptr) return;
	if (shader) shaderPool[name] = shader;
}
void AssetsManager::LoadMesh(std::string name, MeshGeometry* mesh) {
	if (meshPool[name] != nullptr) return;
	if (mesh) meshPool[name] = mesh;
}

TextureBase* AssetsManager::FetchTexture(std::string name) {
	return texturePool[name];
}
ShaderBase* AssetsManager::FetchShader(std::string name) {
	return shaderPool[name];
}
MeshGeometry* AssetsManager::FetchMesh(std::string name) {
	return meshPool[name];
}

void AssetsManager::UnloadTexture(std::string name) {
	if(texturePool[name])
		texturePool.erase(name);
}
void AssetsManager::UnloadShader(std::string name) {
	if (shaderPool[name])
		shaderPool.erase(name);
}
void AssetsManager::UnloadMesh(std::string name) {
	if (meshPool[name])
		meshPool.erase(name);
}
