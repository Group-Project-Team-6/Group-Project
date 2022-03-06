#include "Player.h"


Player::Player(Vector3 position, string newName) {
	IntitAssets(); //Temp, Replace with loadAsset Class

	name = newName;
	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 0, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, playerMesh, playerTex, playerShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	playerMotion = new btDefaultMotionState(bttransform);
	playerShape = new btCapsuleShape(0.5, 1);
	playerMass = 80;
	playerFriction = 0.5;
	playerRestitution = 0.5;
	playerInertia = { 1, 1, 1 };
	playerShape->calculateLocalInertia(playerMass, playerInertia);
	btRigidBody::btRigidBodyConstructionInfo playerCI(playerMass, playerMotion, playerShape, playerInertia);
	playerRigidBody = new btRigidBody(playerCI);

	playerConstraint = new btGeneric6DofConstraint(*playerRigidBody, bttransform, true);
	playerConstraint->setLimit(0, 1, -1);
	playerConstraint->setLimit(1, 1, -1);
	playerConstraint->setLimit(2, 1, -1);
	playerConstraint->setLimit(3, 0, 0);
	playerConstraint->setLimit(4, 1, -1);
	playerConstraint->setLimit(5, 0, 0);

	playerRigidBody->setFriction(playerFriction);
	playerRigidBody->setRestitution(playerRestitution);
}

Player::~Player() {
	delete playerMotion;
	delete playerShape;
	delete playerConstraint;
	delete playerRigidBody;

	delete playerMesh;
	delete playerShader;
}

void Player::IntitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("capsule.msh", &playerMesh);

	playerTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	playerShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

}

