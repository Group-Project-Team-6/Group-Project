#include "Player.h"


Player::Player(Vector3 position, string newName) {
	InitAssets(); //Temp, Replace with loadAsset Class

	name = newName;
	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 1, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, playerMesh, playerTex, playerShader));
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
	bullets = new BulletPool();
}

Player::~Player() {
	delete playerMotion;
	delete playerShape;
	delete playerConstraint;
	delete playerRigidBody;

	delete playerMesh;
	delete playerShader;
	delete bullets;
}

void Player::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("capsule.msh", &playerMesh);

	playerTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	playerShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

}

