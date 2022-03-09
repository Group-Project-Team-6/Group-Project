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
	InitItems();
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

	//delete world
	delete world;
	delete renderer;

	//delete GameEntities
	delete ground;
	for (auto i : players) {
		delete i;
	}

	for (auto i : items) {
		delete i;
	}

	for (auto i : walls) {
		delete i;
	}

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

	//Replace with loadAsset Class*/
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
	//Should be a static bodies
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
	//btCollisionShape* groundShape = new btStaticPlaneShape({ 0, 1, 0 }, 40); //Breaks Renderer static objects btTransforms work differently
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, {0, 0, 0});
	ground->SetRigidBody(new btRigidBody(groundCI));
	ground->GetRigidBody()->setFriction(0.5);
	ground->GetRigidBody()->setRestitution(0.5);
	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());

	Transform wallTransform;
	walls[0] = new Wall(wallTransform);
	world->AddGameObject(walls[0]);
	dynamicsWorld->addRigidBody(walls[0]->GetRigidBody());
	//maybe use foreach loops for static objects
}

void Game::InitItems() {
	items[0] = new Item({ 0, 2, 0 }, 1);
	world->AddGameObject(items[0]);
	dynamicsWorld->addRigidBody(items[0]->GetRigidBody());
}

void Game::InitCharacter() {

	for (int i = 0; i < 4; i++) {
		players[i] = new Player({25, 5, -25}, ""); //Positions set from map data	 
		dynamicsWorld->addRigidBody(players[i]->GetRigidBody());
		world->AddGameObject(players[i]);
		//dynamicsWorld->addConstraint(players[i]->GetPlayerConstraints());
	}

	//Networking to tell which player to camera
	
	//world->GetMainCamera()->SetPosition(players[0]->GetCameraPosition().GetPosition());
}

void Game::UpdateGame(float dt) {
	dynamicsWorld->stepSimulation(dt, 0);
	

	//Vector3 Test = players[0]->GetTransform().GetOrientation().ToEuler();
	world->GetMainCamera()->UpdateCamera(players[0]->GetTransform().GetPosition(), players[0]->GetTransform().GetOrientation().ToEuler().y, dt);
	//players[0]->GetTransform().GetOrientation().x;
	//std::cout << players[0]->GetTransform().GetOrientation().ToEuler().x << std::endl;
	command = playerInput.handleInput();
	if (command) {
		command->execute(*players[0], *world, *dynamicsWorld); //Learn which player from networking
	}

	//Anims
	world->UpdatePositions(); //Maybe Change
	renderer->Render();
	//players[1]->GetBulletPool().Animate(); will not work without shooting first?! Assert or If Statement

	/*std::cout <<
		std::to_string(character->GetTransform().GetPosition().x) +
		std::to_string(character->GetTransform().GetPosition().y) +
		std::to_string(character->GetTransform().GetPosition().z) << std::endl;

	std::cout << std::to_string(
		character->GetbtTransform().getOrigin().x())
		+ std::to_string(character->GetbtTransform().getOrigin().y()) +
		std::to_string(character->GetbtTransform().getOrigin().z()) << std::endl;*/
}