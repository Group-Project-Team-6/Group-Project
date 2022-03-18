#include "Player.h"


Player::Player(Vector3 position, string newName, GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) {
	pitch = 0;
	InitAssets(); //Temp, Replace with loadAsset Class

	name = "player" + newName;
	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 1, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, playerMesh.get(), playerTex.get(), playerShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	playerMotion = new btDefaultMotionState(bttransform);
	playerShape = new btCapsuleShape(0.5, 1);
	playerMass = 80;
	playerFriction = 0.8;
	playerRestitution = 0.8;
	playerInertia = { 0, 0, 0 };
	btRigidBody::btRigidBodyConstructionInfo playerCI(playerMass, playerMotion, playerShape, playerInertia);
	playerRigidBody = new btRigidBody(playerCI);
	playerRigidBody->setActivationState(DISABLE_DEACTIVATION);

	playerRigidBody->setFriction(playerFriction);
	playerRigidBody->setRestitution(playerRestitution);
	dynamicsWorld.addRigidBody(playerRigidBody);
	world.AddGameObject(this);

	bullets = new BulletPool(world, dynamicsWorld);
}

Player::~Player() {
	delete playerMotion;
	delete playerShape;
	delete playerConstraint;
	delete playerRigidBody;

	delete bullets;
}

void Player::InitAssets() {
	playerMesh = AssetsManager::FetchMesh("CapsuleMesh");
	TexID texID = AssetsManager::LoadTextureFromFile("CheckerBoardTex", "CheckerBoard.png", false);
	if (texID != -1) playerTex = AssetsManager::FetchTexture("CheckerBoardTex", texID);
	playerTex.get()->Init({ "FBO" });
	playerShader = AssetsManager::FetchShader("GameTechShaderSet");
}

