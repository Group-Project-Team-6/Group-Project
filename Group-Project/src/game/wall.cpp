#include "Wall.h"
#include "AssetsManager.h"

Wall::Wall(Transform buildTransform, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {
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
	wallRigidBody->isStaticObject();
	wallRigidBody->setUserPointer(this);

	world.AddGameObject(this);
	physicsWorld.addRigidBody(wallRigidBody);
}

Wall::~Wall() {
	delete wallMotion;
	delete wallShape;
	delete wallRigidBody;
}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("WallMesh");
	TexID texID = AssetsManager::LoadTextureFromFile("WallTex","corridor_walls_and_floor_c_2.png",false);
	if(texID != -1) wallTex = AssetsManager::FetchTexture("WallTex",texID);
	wallTex.get()->Init({ "FBO" });
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

