#include "Items.h"

Item::Item(Vector3 position, int score, GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) {
	InitAssets(); //Temp, Replace with loadAsset Class
	name = "Item";
	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 0, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, itemMesh.get(), itemTex.get(), itemShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	itemScore = score;
	itemMotion = new btDefaultMotionState(bttransform);
	itemShape = new btCapsuleShape(0.5, 1);
	btRigidBody::btRigidBodyConstructionInfo itemCI(0, itemMotion, itemShape, {0,0,0});
	itemRigidBody = new btRigidBody(itemCI);
	itemRigidBody->isStaticObject();
}

Item::~Item() {
	delete itemMotion;
	delete itemShape;
	delete itemRigidBody;
}

void Item::InitAssets() {
	itemMesh = AssetsManager::FetchMesh("CubeMesh");
	TexID texID = AssetsManager::LoadTextureFromFile("CheckerBoardTex", "CheckerBoard.png", false);
	if (texID != -1) itemTex = AssetsManager::FetchTexture("CheckerBoardTex", texID);
	itemShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Item::OnPlayerCollide() {
	//Free list pattern
}