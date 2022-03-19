#include "Game.h"
#include "../common/TextureLoader.h"
#include "PlayerInput.h"
#include "LevelGen.h"
#include "Painter.h"
#include "../Bullet/BulletCollision/CollisionDispatch/btGhostObject.h"

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
	loading = false;
}

void Game::InitWorld() {
	world = new GameWorld();
	world->SetLocalGame(false);
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

	btGhostPairCallback* ghostPair = new btGhostPairCallback();
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPair);
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
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, {0, 0, 0});
	ground->SetRigidBody(new btRigidBody(groundCI));
	ground->GetRigidBody()->setFriction(0.5);
	ground->GetRigidBody()->setRestitution(0.5);
	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());
}

void Game::InitItems() {
	items[0] = new Item({ 20,2, -25 }, 1, *world, *dynamicsWorld);
}

void Game::InitCharacter() {

	for (int i = 0; i < 4; i++) {
		players[i] = new Player({25 * 2.f*i, 5, -25}, "", *world, *dynamicsWorld); //Positions set from map data	 
		world->AddPlayer(players[i]);
		if ((world->IsLocalGame() || i == 0) && i < 5) {
			world->SetLocalPlayerCount(world->GetLocalPlayerCount() + 1);
			world->AddMainCamera();
			world->GetMainCamera(i)->SetNearPlane(0.1f); //Graphics - Check planes Positions, can they be default
			world->GetMainCamera(i)->SetFarPlane(1000.0f); //Graphics - Check planes Positions
		}
	}
}
/////////////////Build Game///////////////////////////

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

	vector<Wall*> vecWalls;

	float unitLength = scale; //int
	int numWalls = 0;
	int numItems = 0;
	for (int i = 0; i < 1; i++)
	{
		for (float level = 0; level < maze.size(); level += 1.0f) //int
		{
			for (float l = 0; l < length; l += 1.0f) //int
			{
				for (float w = 0; w < width; w += 1.0f) //int
				{
					//AddChild(i, GetSymbol(level, l, w), level, l, w);
					//AddChild(i, maze[level][l * width + w], level, l, w);

					char ch = maze[level][l * width + w];
					Vector3 position({ ((l + 0.5f) * unitLength) - 40 , (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
					switch (ch)
					{
					case 'P':
						if (numItems > 36) continue;
						items[numItems] = new Item(position, 1, *world, *dynamicsWorld);
						numItems++;
						break;
					case '#':
						wallsTransform.SetPosition(position);
						wallsTransform.SetOrientation(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0, i * 90, 0));
						vecWalls.push_back(new Wall(wallsTransform, *world, *dynamicsWorld));
						wallsTransform.SetScale({ scale - 0.01f, scale - 0.01f, scale - 0.01f });
						numWalls++;

						break;
					case 'S':
						break;
					case 'A':
						//stairs
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
/////////////////Build Level//////////////////////////

/////////////////Other Functions//////////////////////
void Game::GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d) {
	d->GetMemoryAllocationSize(*world);
	d->GetMemoryAllocationSize(*audioManager);
	d->GetMemoryAllocationSize(*renderer);
}

void Game::exectureTriggers() {
	for (int i = 0; i < world->GetGameObjects().size(); i++) {
		if (world->GetGameObjects()[i]->getTrigger() && world->GetGameObjects()[i]->getGhostObject()->getNumOverlappingObjects()) {
			GameEntity* objA = world->GetGameObjects()[i];
			for (int j = 0; j < world->GetGameObjects()[i]->getGhostObject()->getNumOverlappingObjects(); j++) {
				{
					GameEntity* objB = (GameEntity*)world->GetGameObjects()[i]->getGhostObject()->getOverlappingObject(j)->getUserPointer();
					std::cout << objA->GetName() << std::endl;
					std::cout << objB->GetName() << std::endl;
						//Execute triggers
					if (objA->GetName() == "Item" && objB->GetName() == "Player") {
						std::cout << "Player has picked up item" << std::endl;
						return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Player") {
						std::cout << "Player Shot" << std::endl;
						return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Wall") {
						std::cout << "Wall Painted" << std::endl;
						return;
					}
				}
			}			
		}
	}
}
/////////////////Other Functions///////////////////////

/////////////////Update Game//////////////////////////
void Game::UpdateGame(float dt) {

	dynamicsWorld->stepSimulation(dt, 0);
	audioManager->AudioUpdate(world, dt);
	world->GetMainCamera(0)->UpdateCamera(players[0]->GetTransform().GetPosition(), players[0]->GetTransform().GetOrientation().ToEuler().y,
		players[0]->GetTransform().GetOrientation().ToEuler().x, dt);

	std::queue<ControlsCommand*>& command = playerInput[0].handleInput();
	while (command.size() > 0) {
		command.front()->execute(*players[0], *world->GetMainCamera(0), *audioManager); //Learn which player from networking
		command.pop();
	}

	world->UpdatePositions(); //Maybe Change
	GameTimer t;
	renderer->Render();
	t.Tick();
	float ti = t.GetTimeDeltaSeconds();
	//if (1.0f / ti < 60) std::cout << "Update Time: " << ti << "s -- fps: " << 1.0f / ti << std::endl;
	players[0]->GetBulletPool()->Animate(*players[0]->GetRigidBody(), dt);

	exectureTriggers();

}
/////////////////Update Game//////////////////////////


