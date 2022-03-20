#include "Game.h"
#include "../common/TextureLoader.h"
#include "PlayerInput.h"
#include "LevelGen.h"
#include "Painter.h"
#include <math.h>
#include <thread>
#include <mutex>

//Namespaces?

Game::Game() {
	loading = true;
	InitWorld();
	std::thread loadScreenThread(&Game::RenderLoading,this);
	Init();
	//std::thread initThread(&Game::Init,this);

	//initThread.join();
	loadScreenThread.join();
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

	for (int i = 0; i < 4; i++) {
		if(playerInput[i]) 
			delete playerInput[i];
	}

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

void Game::Init() {
	InitPhysics();
	InitAudio();
	InitAssets();
	InitScene();
	InitItems();
	LevelGeneration();
	InitCharacter();
	InitPlayerInput();
	loading = false;
}

void Game::InitWorld() {
	world = new GameWorld();
	world->SetLocalGame(true);
	renderer.reset(new GameTechRenderer(*world));// new GameTechRenderer(*world);
	AssetsManager::SetRenderer(renderer);
	world->SetRenderer(renderer.get());
}

void Game::RenderLoading() {
	RendererPtr loadingRenderer;
	loadingRenderer.reset(new GameLoadingRenderer());
	while (loading) {
		loadingRenderer.get()->Render();
		//wglMakeCurrent(NULL, NULL);
		Sleep(10);
	}
}

void Game::InitAssets() {
	AssetsManager::LoadMeshFromFile("SphereMesh", "Sphere.msh");
	AssetsManager::LoadMeshFromFile("CubeMesh", "Cube.msh");
	AssetsManager::LoadMeshFromFile("WallMesh", "corridor_wall_6.fbx");
	AssetsManager::LoadMeshFromFile("CapsuleMesh", "Capsule.msh");
	AssetsManager::LoadShaderFromFile("GameTechShaderSet", "GameTechShader.set");
	AssetsManager::LoadTextureFromFile("CheckerboardTex", "checkerboard.png");

	sphereMesh = AssetsManager::FetchMesh("SphereMesh");
	cubeMesh = AssetsManager::FetchMesh("CubeMesh");;
	capsuleMesh = AssetsManager::FetchMesh("CapsuleMesh");;

	basicTex = AssetsManager::FetchTexture("CheckerboardTex");
	basicTex.get()->Init({ "FBO" });
	basicShader = AssetsManager::FetchShader("GameTechShaderSet");
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

void Game::InitAudio() {
	audioManager = new AudioManager();
	audioManager->InitSystem();
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

	ground->SetRenderObject(new RenderObject(&ground->GetTransform(), cubeMesh.get(), basicTex.get(), basicShader.get()));
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

	//std::cout << &*world << std::endl;
	//std::cout << &*dynamicsWorld << std::endl;
}

void Game::InitItems() {
	/*items[0] = new Item({ 0, 2, 0 }, 1);
	world->AddGameObject(items[0]);
	dynamicsWorld->addRigidBody(items[0]->GetRigidBody());*/
}

void Game::InitCharacter() {

	for (int i = 0; i < 4; i++) {
		players[i] = new Player({25, 5, -25}, "", *world, *dynamicsWorld); //Positions set from map data	 
		world->AddPlayer(players[i]);
		if ((world->IsLocalGame() || i == 0) && i < 5) {
			world->SetLocalPlayerCount(world->GetLocalPlayerCount() + 1);
			world->AddMainCamera();
			world->GetMainCamera(i)->SetNearPlane(0.1f); //Graphics - Check planes Positions, can they be default
			world->GetMainCamera(i)->SetFarPlane(1000.0f); //Graphics - Check planes Positions

		}
		//dynamicsWorld->addRigidBody(players[i]->GetRigidBody());
		//world->AddGameObject(players[i]);
	}
}

void Game::InitPlayerInput() {
	playerInput[0] = new PlayerInput();
	playerInput[1] = new PlayerInput2();
	playerInput[2] = nullptr;
	playerInput[3] = nullptr;
}

void Game::UpdateGame(float dt) {

	dynamicsWorld->stepSimulation(dt, 0);

	audioManager->AudioUpdate(world, dt);

	for (int i = 0; i < world->GetLocalPlayerCount(); i++) {
		world->GetMainCamera(i)->UpdateCamera(players[i]->GetTransform().GetPosition(), players[i]->GetTransform().GetOrientation().ToEuler().y, players[i]->GetPitch(), dt);
		players[i]->GetBulletPool()->Animate(dt);
		players[i]->GetRigidBody()->setAngularVelocity({ 0,0,0 });
		if (playerInput[i]) {
			std::queue<ControlsCommand*>& command = playerInput[i]->handleInput();
			while (command.size() > 0) {
				command.front()->execute(*players[i], *world, *dynamicsWorld, *audioManager); //Learn which player from networking
				command.pop();
			}
		}
	}

	world->UpdatePositions(); //Maybe Change
	GameTimer t;
	renderer->Render();
	t.Tick();
	float ti = t.GetTimeDeltaSeconds();
	if (1.0f / ti < 60) std::cout << "Update Time: " << ti << "s -- fps: " << 1.0f / ti << std::endl;
	/*std::cout <<
		std::to_string(character->GetTransform().GetPosition().x) +
		std::to_string(character->GetTransform().GetPosition().y) +
		std::to_string(character->GetTransform().GetPosition().z) << std::endl;

	std::cout << std::to_string(
		character->GetbtTransform().getOrigin().x())
		+ std::to_string(character->GetbtTransform().getOrigin().y()) +
		std::to_string(character->GetbtTransform().getOrigin().z()) << std::endl;*/
	//t.Tick();
	//float ti = t.GetTimeDeltaSeconds();
	//if (ti > 0.01) std::cout << "Update Time: " << ti << "s -- fps: " << 1.0f / ti << std::endl;
}

void Game::LevelGeneration() {

	int length = 10;
	int width = 10;

	float scale = 5;

	LevelGen* levelGenerator = new LevelGen();
	levelGenerator->Generate(length, width);
	vector<string> maze = levelGenerator->GetLevelStrings();
	//vector<string> maze = levelGenerator->TestMap();


	Transform wallsTransform;
	wallsTransform.SetPosition({ 50,2,0 });
	wallsTransform.SetScale({ scale,scale,scale });
	wallsTransform.SetOrientation({ 1,0,0,1 });

	Transform stairsTransform;
	stairsTransform.SetScale({ scale, scale ,0.5 });
	stairsTransform.SetOrientation({ 0.5,0,0,1 });
	stairsTransform.SetPosition({ 10,2,0 });

	/*
	stairsTransform.SetOrientation({ 0.5,0,0,1 });
	Wall* stairs = new Wall(stairsTransform);
	stairs->UpdateCollShape(scale, scale, 0.5);
	dynamicsWorld->addRigidBody(stairs->GetRigidBody());
	world->AddGameObject(stairs);

	stairsTransform.SetOrientation({ -0.5,0,0,1 });
	Wall* stairs1 = new Wall(stairsTransform);
	stairs1->UpdateCollShape(scale, scale, 0.5);
	dynamicsWorld->addRigidBody(stairs1->GetRigidBody());
	world->AddGameObject(stairs1);
	stairsTransform.SetScale({ 0.5, scale ,scale });
	stairsTransform.SetOrientation({ 0.5,0,0,1 });
	Wall* stairs2 = new Wall(stairsTransform);
	stairs2->UpdateCollShape(scale, scale, 0.5);
	dynamicsWorld->addRigidBody(stairs2->GetRigidBody());
	world->AddGameObject(stairs2);
	stairsTransform.SetOrientation({ -0.5,0,0,1 });
	Wall* stairs3 = new Wall(stairsTransform);
	stairs3->UpdateCollShape(scale, scale, 0.5);
	dynamicsWorld->addRigidBody(stairs3->GetRigidBody());
	world->AddGameObject(stairs3);
	*/

	/*
	Wall* wall1 = new Wall(wallsTransform);
	dynamicsWorld->addRigidBody(wall1->GetRigidBody());
	world->AddGameObject(wall1);
	wallsTransform.SetPosition({ 0,2,50 });
	Wall* wall2 = new Wall(wallsTransform);
	dynamicsWorld->addRigidBody(wall2->GetRigidBody());
	world->AddGameObject(wall2);
	wallsTransform.SetPosition({ 0,2,-50 });
	Wall* wall3 = new Wall(wallsTransform);
	dynamicsWorld->addRigidBody(wall3->GetRigidBody());
	world->AddGameObject(wall3);
	wallsTransform.SetPosition({ -50,2,0 });
	Wall* wall4 = new Wall(wallsTransform);
	dynamicsWorld->addRigidBody(wall4->GetRigidBody());
	world->AddGameObject(wall4);
	*/

	vector<Wall*> vecWalls;

	float unitLength = scale; //int
	int numWalls = 0;
	int numItems = 0;
	for (int i = 0; i < 1; i++)
	{
		for (float level = 0; level < maze.size(); level+=1.0f) //int
		{
			for (float l = 0; l < length; l+=1.0f) //int
			{
				for (float w = 0; w < width; w+=1.0f) //int
				{
					//AddChild(i, GetSymbol(level, l, w), level, l, w);
					//AddChild(i, maze[level][l * width + w], level, l, w);

					char ch = maze[level][l * width + w];
					Vector3 position ({ ((l + 0.5f) * unitLength) - 40 , (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40});
					switch (ch)
					{
					case 'P':
						if (numItems > 36) continue;
						items[numItems] = new Item(position, 1);
						world->AddGameObject(items[numItems]);
						dynamicsWorld->addRigidBody(items[numItems]->GetRigidBody());
						numItems++;
						break;
					case '#':
						wallsTransform.SetPosition(position);
						wallsTransform.SetOrientation(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0,i*90,0));
						vecWalls.push_back(new Wall(wallsTransform));
						wallsTransform.SetScale({ scale - 0.01f, scale - 0.01f, scale - 0.01f });
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;

						break;
					case 'S':
						break;
					case 'A':
						/*
						stairsTransform.SetOrientation({ 0.5,0,0,1 });
						stairsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(stairsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale, scale, 0.5);
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						*/
						break;
					case 'V':

						break;
					case '<':

						break;
					case '>':

						break;
					}


				}
			}
			//collGen.Collectables(level, maze[level], width, length, unitLength);
		}
	}

}

void Game::GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d) {
	d->GetMemoryAllocationSize(*world);
	d->GetMemoryAllocationSize(*audioManager);
	d->GetMemoryAllocationSize(*renderer);
}