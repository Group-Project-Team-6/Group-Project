#include "AssetsManager.h"
#include "../common/TextureLoader.h"

RendererPtr AssetsManager::renderer(nullptr);
std::map<std::string, std::vector<TexturePtr>> AssetsManager::texturePoolMap;
std::map<std::string, std::vector<ShaderPtr>> AssetsManager::shaderPoolMap;
std::map<std::string, std::vector<MeshPtr>> AssetsManager::meshPoolMap;
std::map<std::string, std::stack<TexID>> AssetsManager::TexStackMap;
std::map<std::string, std::stack<ShaderID>> AssetsManager::ShaderStackMap;
std::map<std::string, std::stack<MeshID>> AssetsManager::MeshStackMap;

AssetsManager::~AssetsManager() {
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

TexID AssetsManager::LoadTextureFromFile(std::string name, std::string fileName, bool isShared) {
	if (renderer) {
		std::shared_ptr<TextureBase> t;
		t.reset(TextureLoader::LoadAPITexture(fileName));
		if (t.get()) return LoadTexture(name,t,isShared);
	}
	return -1;
}
ShaderID AssetsManager::LoadShaderFromFile(std::string name, std::string fileName, bool isShared) {
	if (renderer) {
		std::shared_ptr<ShaderBase> s;
		s.reset(renderer->LoadShader(fileName));
		if (s.get()) return LoadShader(name,s, isShared);
	}
	return -1;
}
MeshID AssetsManager::LoadMeshFromFile(std::string name, std::string fileName, bool isShared) {
	if (renderer) {
		MeshPtr m;
		m.reset(renderer->LoadMesh(fileName));
		if (m.get()) return LoadMesh(name,m, isShared);
	}
	return -1;
}

TexID AssetsManager::LoadTexture(std::string name, TexturePtr texture, bool isShared) {
	if (texturePoolMap[name].size() <= 0)
		texturePoolMap[name].push_back(TexturePtr(nullptr));
	if (isShared) {
		if (texturePoolMap[name].size() > 0 && texturePoolMap[name][0].get()) return -1;
		if (texture.get()) {
			texturePoolMap[name][0] = texture;
			return 0;
		}
		return -1;
	}
	else {
		if (texture.get()) {
			TexID id = -1;
			if (TexStackMap[name].size() > 0) {
				id = TexStackMap[name].top();
				texturePoolMap[name][id] = texture;
				TexStackMap[name].pop();
			}
			else {
				id = texturePoolMap[name].size();
				texturePoolMap[name].push_back(texture);
			}
			return id;
		}
		return -1;
	}
}
ShaderID AssetsManager::LoadShader(std::string name, ShaderPtr shader, bool isShared) {
	if (shaderPoolMap[name].size() <= 0)
		shaderPoolMap[name].push_back(ShaderPtr(nullptr));
	if (isShared) {
		if (shaderPoolMap[name].size() > 0 && shaderPoolMap[name][0].get()) return -1;
		if (shader.get()) {
			if (shaderPoolMap[name].size() <= 0)
				shaderPoolMap[name].push_back(shader);
			else
				shaderPoolMap[name][0] = shader;
			return 0;
		}
		return -1;
	}
	else {
		if (shader.get()) {
			TexID id = -1;
			if (ShaderStackMap[name].size() > 0) {
				id = ShaderStackMap[name].top();
				shaderPoolMap[name][id] = shader;
				ShaderStackMap[name].pop();
			}
			else {
				id = shaderPoolMap[name].size();
				shaderPoolMap[name].push_back(shader);
			}
			return id;
		}
		return -1;
	}
}
MeshID AssetsManager::LoadMesh(std::string name, MeshPtr mesh, bool isShared) {
	if (meshPoolMap[name].size() <= 0)
		meshPoolMap[name].push_back(MeshPtr(nullptr));
	if (isShared) {
		if (meshPoolMap[name].size() > 0 && meshPoolMap[name][0].get()) return -1;
		if (mesh.get()) {
			if (meshPoolMap[name].size() <= 0)
				meshPoolMap[name].push_back(mesh);
			else 
				meshPoolMap[name][0] = mesh;
			return 0;
		}
		return -1;
	}
	else {
		if (mesh.get()) {
			TexID id = -1;
			if (MeshStackMap[name].size() > 0) {
				id = MeshStackMap[name].top();
				meshPoolMap[name][id] = mesh;
				MeshStackMap[name].pop();
			}
			else {
				id = meshPoolMap[name].size();
				meshPoolMap[name].push_back(mesh);
			}
			return id;
		}
		return -1;
	}
}

TexturePtr AssetsManager::FetchTexture(std::string name, TexID id) {
	if (id >= 0 && texturePoolMap[name].size() > id) {
		return texturePoolMap[name][id];
	}
	else {
		if(texturePoolMap[name].size() == 0)
			texturePoolMap[name].clear();
		return nullptr;
	}
}
ShaderPtr AssetsManager::FetchShader(std::string name, ShaderID id) {
	if (id >= 0 && shaderPoolMap[name].size() > id) {
		return shaderPoolMap[name][id];
	}
	else {
		if(shaderPoolMap[name].size() == 0) 
			shaderPoolMap[name].clear();
		return nullptr;
	}
}
MeshPtr AssetsManager::FetchMesh(std::string name, MeshID id) {
	if (id >= 0 && meshPoolMap[name].size() > id) {
		return meshPoolMap[name][id];
	}
	else {
		if(meshPoolMap[name].size() == 0)
			meshPoolMap[name].clear();
		return nullptr;
	}
}

void AssetsManager::UnloadTexture(std::string name, TexID id) {
	if (id >= 0 && texturePoolMap[name].size() > id) {
		if (texturePoolMap[name][id].get()) {
			texturePoolMap[name][id].reset();
		}
		if(id > 0) TexStackMap[name].push(id);
	}
	else {
		if (texturePoolMap[name].size() == 0)
			texturePoolMap.erase(name);
	}
}
void AssetsManager::UnloadShader(std::string name, ShaderID id) {
	if (id >= 0 && shaderPoolMap[name].size() > id) {
		if (shaderPoolMap[name][id].get()) {
			shaderPoolMap[name][id].reset();
		}
		if (id > 0) ShaderStackMap[name].push(id);
	}
	else {
		if (shaderPoolMap[name].size() == 0)
		shaderPoolMap.erase(name);
	}
}
void AssetsManager::UnloadMesh(std::string name, MeshID id) {
	if (id >= 0 && meshPoolMap[name].size() > id) {
		if (meshPoolMap[name][id].get()) {
			meshPoolMap[name][id].reset();
		}
		if (id > 0) MeshStackMap[name].push(id);
	}
	else {
		if (meshPoolMap[name].size() == 0)
			meshPoolMap.erase(name);
	}
}

void AssetsManager::SetRenderer(RendererPtr renderer) {
	AssetsManager::renderer = renderer;
}

RendererPtr AssetsManager::GetRenderer() {
	return renderer;
}