#include "Game.h"

#include "../common/TextureLoader.h"

//Namespaces?

Game::Game() {
	void InitWorld();
	void InitPhysics();
	//void InitAudio();
	void InitAssets();
	void InitScene();
	//void LevelGeneration();
	void InitCharacter();
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
	delete character;
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
}

void Game::InitPhysics() {
	maxProxies = 1024;
	worldAabbMin = { -100, -100, -100 };
	worldAabbMax = { 100, 100, 100 };
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
}

void Game::InitCharacter() {
	character = new GameEntity();
	character->GetTransform()
		.SetPosition({0, 5, -0})
		.SetScale({ 1, 1, 1 }) //Check Scale
		.SetOrientation({ 0, 0, 0, 1 }); 

	character->SetRenderObject(new RenderObject(&character->GetTransform(), capsuleMesh, basicTex, basicShader));


}

void Game::Update(float dt) {
	dynamicsWorld->stepSimulation(1 / 60.0f, 10);
	world->GetMainCamera()->UpdateCamera(dt);
	//Player Controls

	world->UpdatePositions(); //Maybe Change
	renderer->Render();
}