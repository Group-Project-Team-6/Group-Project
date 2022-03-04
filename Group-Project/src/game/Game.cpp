#include "Game.h"
#include "../common/TextureLoader.h"
#include "PlayerInput.h"

//Namespaces?

Game::Game() {
	command = nullptr;
	InitWorld();
	InitPhysics();
	//void InitAudio();
	InitAssets();
	InitScene();
	//void LevelGeneration();
	InitCharacter();
	//void InitHUD
	//InitNetworking?

}

Game::~Game() {

	//delete Physics
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;

	//delete GameEntities
	delete player;
	delete ground;

	//delete world
	delete world;
	delete renderer;

	//nullptr?
}

void Game::InitWorld() {
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);

	world->GetMainCamera()->SetNearPlane(0.1f); //Graphics - Check planes Positions, can they be default
	world->GetMainCamera()->SetFarPlane(1000.0f); //Graphics - Check planes Positions
}

void Game::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("cube.msh", &cubeMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	//Replace with loadAsset Class
}

void Game::InitPhysics() {
	maxProxies = 1024;
	worldAabbMin = { -1000, -1000, -1000 };
	worldAabbMax = { 1000, 1000, 1000 };
	broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void Game::InitScene() {
	world->ClearAndErase();
	dynamicsWorld->clearForces();

	//ground
	//Should be static bodies
	ground = new GameEntity("Ground");
	ground->GetTransform()
		.SetPosition(Vector3(0, 0, 0))
		.SetScale(Vector3(100, 1, 100))
		.SetOrientation(Quaternion(0, 0, 0, 1));

	ground->SetRenderObject(new RenderObject(&ground->GetTransform(), cubeMesh, basicTex, basicShader));

	transformConverter.BTNCLConvert(ground->GetTransform(), ground->GetbtTransform());

	int groundMass = 0;

	btDefaultMotionState* groundMotion = new btDefaultMotionState(ground->GetbtTransform());
	btCollisionShape* groundShape = new btBoxShape({ 50, 0.5, 50 });
	//btCollisionShape* groundShape = new btStaticPlaneShape({ 0, 1, 0 }, 40); //Breaks Renderer somehow
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, {0, 0, 0});
	ground->SetRigidBody(new btRigidBody(groundCI));

	ground->GetRigidBody()->setFriction(0.3);

	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());
}

void Game::InitCharacter() {

	/*player = new Player();
	player->AddPlayer({ 25, 5, -25 });
	dynamicsWorld->addRigidBody(player->GetRigidBody());
	world->AddGameObject(player);*/

	//To be added to inheritance class of game Entity
	//Will need to limit velocity of all kinematic objects
	GameEntity* character = new GameEntity();
	character->GetTransform()
		.SetPosition({25, 4, -25})
		.SetScale({ 1, 1, 1 }) //Check Scale
		.SetOrientation({ 0, 0, 0, 1 }); 

	//Set all values with a strut?
	//Tidy up variables

	character->SetRenderObject(new RenderObject(&character->GetTransform(), capsuleMesh, basicTex, basicShader));

	transformConverter.BTNCLConvert(character->GetTransform(), character->GetbtTransform());

	btDefaultMotionState* characterMotion = new btDefaultMotionState(character->GetbtTransform());
	btCollisionShape* characterShape = new btCapsuleShape(0.5, 1);
	int characterMass = 80;
	btVector3 characterIntertia = { 1, 1, 1 };

	characterShape->calculateLocalInertia(characterMass, characterIntertia);

	btRigidBody::btRigidBodyConstructionInfo sphereCI(80, characterMotion, characterShape, {1, 1, 1});
	character->SetRigidBody(new btRigidBody(sphereCI));

	btVector3 lowerLimit = { 0, 0, 0 };
	btVector3 upperLimit = { 0, 0, 0 };
	btGeneric6DofConstraint* constraint = new btGeneric6DofConstraint(*character->GetRigidBody(), character->GetbtTransform(), true);
	constraint->setLimit(0, 1, -1);
	constraint->setLimit(1, 1, -1);
	constraint->setLimit(2, 1, -1);
	constraint->setLimit(3, 0, 0);
	constraint->setLimit(4, 1, -1);
	constraint->setLimit(5, 0, 0);

	character->GetRigidBody()->setFriction(0.3);
	//btConstraintParams
	//character->GetRigidBody()->


	world->AddGameObject(character);
	dynamicsWorld->addRigidBody(character->GetRigidBody());
	dynamicsWorld->addConstraint(constraint);



	//Build From Classes
	//Scene Graph?*/

}

void Game::UpdateGame(float dt) {
	dynamicsWorld->stepSimulation(1 / 60.0f, 10);
	world->GetMainCamera()->UpdateCamera(dt);

	command = playerInput.handleInput();
	if (command) {
		command->execute(*player);
	}

	world->UpdatePositions(); //Maybe Change
	renderer->Render();
}