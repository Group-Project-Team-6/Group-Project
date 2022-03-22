#include "Wall.h"
#include "AssetsManager.h"

Wall::Wall(Transform buildTransform) {
	InitAssets();
	name = "Wall";
	transform = buildTransform;		

	isStatic = false; //change
	this->SetRenderObject(new RenderObject(&transform, wallMesh.get(), wallTex.get(), wallShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	wallMotion = new btDefaultMotionState(bttransform);
	Vector3 scale = transform.GetScale();
	wallShape = new btBoxShape({ (scale.x / 2.0f), (scale.y / 2.0f), (scale.z / 2.0f) });
	btRigidBody::btRigidBodyConstructionInfo itemCI(0,wallMotion, wallShape, { 0,0,0 });
	rigidBody = new btRigidBody(itemCI);
	rigidBody->isStaticObject();
	//rigidBody->setGravity({ 0, 0, 0 });
	//rigidBody->setActivationState(0);
	//rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//rigidBody->setFriction(0.8);
	//rigidBody->setRestitution(0.8);
	rigidBody->setUserPointer(this);

	/*world.AddGameObject(this);*/
	//physicsWorld.addRigidBody(wallRigidBody);

	//wallObject = new btCollisionObject();
	//wallObject->setCollisionShape(wallShape);
	//wallObject->setWorldTransform(bttransform);
	//wallObject->setUserPointer(this);
}

Wall::~Wall() {
	delete wallMotion;
	delete wallShape;
	delete rigidBody;
}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("WallMesh");
	TexID texID = AssetsManager::LoadTextureFromFile("WallTex","corridor_walls_and_floor_c_2.png",false);
	if(texID != -1) wallTex = AssetsManager::FetchTexture("WallTex",texID);
	wallTex.get()->Init({ "FBO" });
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

