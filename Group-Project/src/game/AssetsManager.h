#pragma once
#include <string>
#include "../CSC8503/GameWorld.h"
#include "../common/TextureBase.h"

class AssetsManager {
public:
	~AssetsManager();
	static void LoadAssets(std::string configFile);

	static void LoadTextureFromFile(std::string name, std::string fileName);
	static void LoadShaderFromFile(std::string name, std::string fileName);
	static void LoadMeshFromFile(std::string name, std::string fileName);

	static void LoadTexture(std::string name, TextureBase* texture);
	static void LoadShader(std::string name, ShaderBase* shader);
	static void LoadMesh(std::string name, MeshGeometry* mesh);

	static TextureBase* FetchTexture(std::string name);
	static ShaderBase* FetchShader(std::string name);
	static MeshGeometry* FetchMesh(std::string name);

	static void UnloadTexture(std::string name);
	static void UnloadShader(std::string name);
	static void UnloadMesh(std::string name);
private:
	AssetsManager() {};
	static std::map<std::string, TextureBase*> texturePool;
	static std::map<std::string, ShaderBase*> shaderPool;
	static std::map<std::string, MeshGeometry*> meshPool;
	static RendererBase* renderer;
};