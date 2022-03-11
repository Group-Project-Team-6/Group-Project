#include "Wall.h"
#include "AssetsManager.h"

Wall::Wall(Transform buildTransform) {
	InitAssets(); //Temp, Replace with loadAsset Class

	transform = buildTransform;		

	this->SetRenderObject(new RenderObject(&transform, wallMesh, wallTex, wallShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	wallMotion = new btDefaultMotionState(bttransform);
	wallShape = new btCapsuleShape(0.5, 1);
	btRigidBody::btRigidBodyConstructionInfo itemCI(0, wallMotion, wallShape, { 0,0,0 });
	wallRigidBody = new btRigidBody(itemCI);
}

Wall::~Wall() {

}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("CubeMesh");

	wallTex = AssetsManager::FetchTextureUnique("CheckerBoardTex");
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

