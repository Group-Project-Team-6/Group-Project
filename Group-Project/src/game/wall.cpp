#include "Wall.h"
#include "AssetsManager.h"

Wall::Wall(Transform buildTransform) {
	InitAssets(); //Temp, Replace with loadAsset Class
	name = "Wall";
	transform = buildTransform;		

	this->SetRenderObject(new RenderObject(&transform, wallMesh.get(), wallTex.get(), wallShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	wallMotion = new btDefaultMotionState(bttransform);
	Vector3 scale = transform.GetScale();
	wallShape = new btBoxShape({ (scale.x / 2.0f), (scale.y / 2.0f), (scale.z / 2.0f) });
	btRigidBody::btRigidBodyConstructionInfo itemCI(0,wallMotion, wallShape, { 0,0,0 });
	wallRigidBody = new btRigidBody(itemCI);
}

Wall::~Wall() {

}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("CubeMesh");
	TexID texID = AssetsManager::LoadTextureFromFile("CheckerBoardTex","CheckerBoard.png",false);
	if(texID != -1) wallTex = AssetsManager::FetchTexture("CheckerBoardTex",texID);
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

