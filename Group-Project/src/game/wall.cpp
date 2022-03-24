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
	rigidBody = new btRigidBody(itemCI);
	rigidBody->isStaticObject();
	rigidBody->setUserPointer(this);

	//wallObject = new btCollisionObject();
	//wallObject->setCollisionShape(wallShape);
	//wallObject->setWorldTransform(bttransform);
	//wallObject->setUserPointer(this);
}

Wall::~Wall() {
	delete wallMotion;
	delete wallShape;
	AssetsManager::UnloadTexture("WallTex", texID);
}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("WallMesh");
	texID = AssetsManager::LoadTextureFromFile("WallTex","nyan.png",false);
	if(texID != -1) wallTex = AssetsManager::FetchTexture("WallTex",texID);
	wallTex.get()->Init({ "FBO" });
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

