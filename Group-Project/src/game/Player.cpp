#include "Player.h"


Player::Player() {
	//Intialise to nullptr

	//Temp //
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("capsule.msh", &playerMesh);

	playerTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	playerShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
	//Temp //

	//Replace with loadAsset Class
}

Player::~Player() {
	delete playerMotion;
	delete playerShape;
}

void Player::AddPlayer(const Vector3& position, string name) {
	Player* player = new Player();
	player->GetTransform()
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 0, 0, 1 });

	player->SetRenderObject(new RenderObject(&player->GetTransform(), playerMesh, playerTex, playerShader));
	transformConverter.BTNCLConvert(player->GetTransform(), player->GetbtTransform());

	playerMotion = new btDefaultMotionState();
	playerShape = new btCapsuleShape(0.5, 1);
	playerMass = 80;
	playerInertia = { 1, 1, 1 };
	playerShape->calculateLocalInertia(playerMass, playerInertia);
	//btRigidBody::btRigidBodyConstructionInfo()

	//player->SetRigidBody()
}