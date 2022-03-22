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

	//delete Physics
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;
	delete ghostPair;

	//delete world
	delete world;

	for (int i = 0; i < 4; i++) {
		if(playerInput[i]) 
			delete playerInput[i];
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

	ghostPair = new btGhostPairCallback();
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
	//btCollisionShape* groundShape = new btStaticPlaneShape({ 0, 1, 0 }, 40); //Breaks Renderer static objects btTransforms work differently
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, {0, 0, 0});
	ground->SetRigidBody(new btRigidBody(groundCI));
	ground->GetRigidBody()->setFriction(0.5);
	ground->GetRigidBody()->setRestitution(0.5);
	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());

	Transform wallTransform;
	wallTransform.SetPosition({ 25, 6, -20 });
	wallTransform.SetScale({ 10, 10, 2 });
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
		if ((world->IsLocalGame() || i == 0) && i < 4) {
			world->SetLocalPlayerCount(world->GetLocalPlayerCount() + 1);
			world->AddMainCamera();
			world->GetMainCamera(i)->SetNearPlane(0.1f); //Graphics - Check planes Positions, can they be default
			world->GetMainCamera(i)->SetFarPlane(1000.0f); //Graphics - Check planes Positions
			world->GetMainCamera(i)->SetDistance(8.0f);

		}
		dynamicsWorld->addRigidBody(players[i]->GetRigidBody());
		world->AddGameObject(players[i]);
	}
}

void Game::InitPlayerInput() {
	playerInput[0] = new PlayerInput();
	playerInput[1] = new PlayerInput2();
	playerInput[2] = nullptr;
	playerInput[3] = nullptr;
}

void Game::LevelGeneration() {

	int length = 5;
	int width = 5;

	float scale = 5;

	LevelGen* levelGenerator = new LevelGen();
	levelGenerator->Generate(length, width);
	vector<string> maze = levelGenerator->GetLevelStrings();

	Transform wallsTransform;
	wallsTransform.SetPosition({ 50,2,0 });
	wallsTransform.SetScale({ scale,scale,scale });
	//wallsTransform.SetOrientation({ 1,0,0,1 });

	Transform stairsTransform;
	stairsTransform.SetScale({ scale, scale ,0.5 });
	//stairsTransform.SetOrientation({ 0.5,0,0,1 });
	stairsTransform.SetPosition({ 10,2,0 });

	Transform floorsTransform;
	floorsTransform.SetScale({ scale, scale ,0.1 });
	//floorsTransform.SetOrientation({ 1,0,0,1 });


	vector<Wall*> vecWalls;
	vector<Wall*> floors;

	float unitLength = scale;
	int numWalls = 0;
	int numFloors = 0;
	for (int i = 0; i < 1; i++)
	{
		for (float level = 0; level < maze.size(); level+=1.0f)
		{
			for (float l = 0; l < length; l+=1.0f)
			{
				for (float w = 0; w < width; w += 1.0f)
				{
					char ch = maze[level][l * width + w];
					Vector3 position({ ((l + 0.5f) * unitLength) - 40 , (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
					switch (ch)
					{
					case 'P':
						if (level >= 0) {
							floorsTransform.SetPosition(position + Vector3(0,-unitLength*.45f,0));
							floorsTransform.SetScale({ scale, 0.1f, scale });
							floors.push_back(new Wall(floorsTransform));
							//dynamicsWorld->addCollisionObject(floors[numFloors]->getCollisionObject());
							dynamicsWorld->addRigidBody(floors[numFloors]->GetRigidBody());
							world->AddGameObject(floors[numFloors]);
							numFloors++;
						}
						break;
					case '#':
						wallsTransform.SetPosition(position);
						wallsTransform.SetScale({ scale, scale, scale });
						vecWalls.push_back(new Wall(wallsTransform));
						//dynamicsWorld->addCollisionObject(vecWalls[numWalls]->getCollisionObject());
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;

						break;
					case 'S':
						break;
					case '<':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ 0.42,0,0,1 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new Wall(stairsTransform));
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case '>':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ -0.42,0,0,1 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new Wall(stairsTransform));
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'V':
						break;
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ 0.39,1,1,0.39 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new Wall(stairsTransform));
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'A':
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ -0.39,1,1,-0.39 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new Wall(stairsTransform));
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					}


				}
			}

		}
	}

	Transform collectablesTransform;
	collectablesTransform.SetScale({ scale / 5,scale / 5,scale / 5 });
	//collectablesTransform.SetOrientation({ 1,0,0,1 });

	vector<vector<int>> collectablePos;
	vector<Item*> vecCollectables;
	int numCollectablesPlaced = 0;
	for (int i = 0; i < maze.size(); i++) {

		int numCollectables = ((length * width) / 50);

		for (int x = 0; x < numCollectables; x++) {

			int randomNum = rand() % maze[i].length();

			if (maze[i][randomNum] == 'P') {
				int posLength = randomNum / length;
				int posWidth = randomNum - (posLength * width);

				collectablesTransform.SetPosition({ ((posLength + 0.5f) * unitLength) - 40, (i * unitLength) + 3, ((posWidth + 0.5f) * unitLength) - 40 });
				collectablesTransform.SetScale({ 0.1, 0.1, 0.1 });
				vecCollectables.push_back(new Item(collectablesTransform.GetPosition(),1));
				dynamicsWorld->addCollisionObject(vecCollectables[numCollectablesPlaced]->getGhostObject());
				world->AddGameObject(vecCollectables[numCollectablesPlaced]);
				numCollectablesPlaced++;

			}
			else { x--; }

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
						//return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Player") {
						std::cout << "Player Shot" << std::endl;
						//return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Wall") {
						std::cout << "Wall Painted" << std::endl;
						//return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Bullet") {
						std::cout << "Bullets Collided" << std::endl;
						//return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Item") {
						std::cout << "Item Hit" << std::endl;
						//return;
					}
					if (objA->GetName() == "Bullet" && objB->GetName() == "Ground") {
						std::cout << "Ground Hit" << std::endl;
						//return;
					}
					objB = nullptr;
				}
			}
			objA = nullptr;			
		}
	}
}
/////////////////Other Functions///////////////////////

/////////////////Update Game//////////////////////////
void Game::UpdateGame(float dt) {

	dynamicsWorld->stepSimulation(dt, 0);
	audioManager->AudioUpdate(world, dt);
	
	for (int i = 0; i < world->GetLocalPlayerCount(); i++) {
		world->GetMainCamera(i)->UpdateCamera(players[i]->GetTransform().GetPosition(), players[i]->GetTransform().GetOrientation().ToEuler().y, players[i]->GetPitch(), dt);
		players[i]->GetBulletPool()->Animate(*(players[i]->GetRigidBody()), dt);
		players[i]->GetRigidBody()->setAngularVelocity({ 0,0,0 });
		if (playerInput[i]) {
			std::queue<ControlsCommand*>& command = playerInput[i]->handleInput();
			while (command.size() > 0) {
				command.front()->execute(*players[i], *(world->GetMainCamera(i)), *audioManager); //Learn which player from networking
				command.pop();
			}
		}
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


