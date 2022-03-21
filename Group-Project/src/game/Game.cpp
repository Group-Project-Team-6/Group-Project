#include "Game.h"
#include "../common/TextureLoader.h"
#include "PlayerInput.h"
#include "LevelGen.h"

//Namespaces?

Game::Game() {
	command = nullptr;
	InitWorld();
	InitPhysics();
	//void InitAudio();
	InitAssets();
	InitScene();
	InitItems();
	LevelGeneration();
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
	maxProxies = 2048;
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

void Game::LevelGeneration() {

	int length = 10;
	int width = 10;

	float scale = 5;

	LevelGen* levelGenerator = new LevelGen();
	levelGenerator->Generate(length, width);
	vector<string> maze = levelGenerator->GetLevelStrings();
	//TEST
	// length = 2;
	// width = 2;
	//vector<string> maze = {"#P<P", "#VSP", "#SPP"};


	Transform wallsTransform;
	wallsTransform.SetPosition({ 50,2,0 });
	wallsTransform.SetScale({ scale,scale,scale });
	wallsTransform.SetOrientation({ 1,0,0,1 });

	Transform stairsTransform;
	stairsTransform.SetScale({ scale, scale ,0.5 });
	stairsTransform.SetOrientation({0.5,0,0,1});
	stairsTransform.SetPosition({ 10,2,0 });

	Transform floorsTransform;
	floorsTransform.SetScale({ scale, scale ,0.1 });
	floorsTransform.SetOrientation({ 1,0,0,1 });
	
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
	vector<Wall*> floors;

	float unitLength = scale; 
	int numWalls = 0;
	int numFloors = 0;
	for (int i = 0; i < 4; i++)
	{
		for (float level = 0; level < maze.size(); level++)
		{
			for (float l = 0; l < length; l++)
			{
				for (float w = 0; w < width; w++)
				{
					char ch = maze[level][l * width + w];

					switch (ch)
					{
					case 'P':
						if (level >= 0) {
							floorsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, ((level * unitLength) + 3) - (scale / 2), ((w + 0.5f) * unitLength) - 40 });
							floors.push_back(new Wall(floorsTransform));
							floors[numFloors]->UpdateCollShape(scale, scale, 0.1);
							dynamicsWorld->addRigidBody(floors[numFloors]->GetRigidBody());
							world->AddGameObject(floors[numFloors]);
							numFloors++;
						}
						break;
					case '#':
						wallsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(wallsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale, scale, scale);
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;

						break;
					case 'S':
						break;
					case '<':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ 0.42,0,0,1 });
						stairsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(stairsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale, scale + (scale / 2.5f), 0.5);
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case '>':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ -0.42,0,0,1 });
						stairsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(stairsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale, scale + (scale / 2.5f), 0.5);
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'V':
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ 0.39,1,1,0.39 });
						stairsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(stairsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale + (scale / 2.5f), scale, 0.5);
						dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'A':
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ -0.39,1,1,-0.39 });
						stairsTransform.SetPosition({ ((l + 0.5f) * unitLength) - 40, (level * unitLength) + 3, ((w + 0.5f) * unitLength) - 40 });
						vecWalls.push_back(new Wall(stairsTransform));
						vecWalls[numWalls]->UpdateCollShape(scale + (scale / 2.5f), scale, 0.5);
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
	collectablesTransform.SetScale({ scale / 5,scale / 5,scale / 5});
	collectablesTransform.SetOrientation({ 1,0,0,1 });

	vector<vector<int>> collectablePos;
	vector<Wall*> vecCollectables;
	int numCollectablesPlaced = 0;
	for (int i = 0; i < maze.size(); i++) {

		int numCollectables = ((length * width) / 50);

		for (int x = 0; x < numCollectables; x++) {

			int randomNum = rand() % maze[i].length();

			if (maze[i][randomNum] == 'P') {
				int posLength = randomNum / length;
				int posWidth = randomNum - (posLength * width);

				collectablesTransform.SetPosition({ ((posLength + 0.5f) * unitLength) - 40, (i * unitLength) + 3, ((posWidth + 0.5f) * unitLength) - 40 });
				vecCollectables.push_back(new Wall(collectablesTransform));
				vecCollectables[numCollectablesPlaced]->UpdateCollShape(0.1, 0.1, 0.1);
				dynamicsWorld->addRigidBody(vecCollectables[numCollectablesPlaced]->GetRigidBody());
				world->AddGameObject(vecCollectables[numCollectablesPlaced]);
				numCollectablesPlaced++;

			}
			else { x--; }

		}

	}

}