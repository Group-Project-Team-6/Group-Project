#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/SpringConstraint.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include <iostream>

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	onGoing = true;
	forceMagnitude	= 500.0f;
	useGravity		= true;
	inSelectionMode = false;
	layer			= 0;
	Debug::SetRenderer(renderer);

	bot = new Bot();
	player = new Player();

	InitialiseAssets();
}

/*
Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!
*/
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("Male1.msh"	 , &charMeshA);
	loadFunc("courier.msh"	 , &charMeshB);
	loadFunc("security.msh"	 , &enemyMesh);
	loadFunc("coin.msh"		 , &bonusMesh);
	loadFunc("capsule.msh"	 , &capsuleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	InitCamera();
	InitWorld();
}

TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

bool TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}
	UpdateKeys();

	SelectObject();
	MoveSelectedObject();

	bot->SetPosition(EnemySphere->GetRenderObject()->GetTransform()->GetPosition());
	player->SetPosition(PlayerSphere->GetRenderObject()->GetTransform()->GetPosition());

	switch (sceneState) {
	case 0:
		Debug::Print("Press Green Button To Start First Level.", Vector2(2, 5));
		Debug::Print("Press Blue Button To Start Second Level.", Vector2(2, 10));
		Debug::Print("Press Red Button To Quit Game.", Vector2(2, 15));
		world->GetMainCamera()->SetPosition(Vector3(2000, 10, 1000));
		world->GetMainCamera()->SetPitch(0);
		world->GetMainCamera()->SetYaw(0);
		break;

	case 1:
		time += dt;
		timeString = std::to_string(time);
		Debug::Print("Time: " + timeString, Vector2(60, 15));
		if (physics->GetgoalState()) {
			std::cout << "Reached Goal" << std::endl;
			physics->SetGravityState(false);
			antiGravityBlock = AddCubeToWorld(Vector3(1036.5f, 9.5f, -88.5f), Vector3(0.5f, 0.5f, 0.5f), Quaternion(0, 0, 0, 0), 3, true, 0.0f, "antiGravityBlock");
			antiGravityBlock->GetPhysicsObject()->SetCollisionResponces(false);
			antiGravityBlock->GetRenderObject()->SetColour(Vector3(0, 0, 0));
			MainSphere->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			PushableCube->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			MainSphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(1097, 26.0f, -98));
			PushableCube->GetRenderObject()->GetTransform()->SetPosition(Vector3(1098.5f, 25.8f, -98));
			SetSceneState(2);
			physics->SetgoalState(false);
		}
		if (physics->GetButtonState()) {
			//Apply force to jump cube
			for (int i = 0; i < jumpCubes.size(); ++i) {
				if (jumpCubes.at(i)->GetPhysicsObject()->GetLinearVelocity().y < 30) {
					KeepForce = true;
				}
			}
			physics->SetButtonState(false);
		}
		if (physics->GetReset()) {
			physics->SetGravityState(false);
			antiGravityBlock = AddCubeToWorld(Vector3(1036.5f, 9.5f, -88.5f), Vector3(0.5f, 0.5f, 0.5f), Quaternion(0, 0, 0, 0), 3, true, 0.0f, "antiGravityBlock");
			antiGravityBlock->GetPhysicsObject()->SetCollisionResponces(false);
			antiGravityBlock->GetRenderObject()->SetColour(Vector3(0, 0, 0));
			MainSphere->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			PushableCube->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			MainSphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(1097, 26.0f, -98));
			PushableCube->GetRenderObject()->GetTransform()->SetPosition(Vector3(1098.5f, 25.8f, -98));
			physics->SetReset(false);
		}
		if (KeepForce == true) {
			for (int i = 0; i < jumpCubes.size(); ++i) {
				if (jumpCubes.at(i)->GetPhysicsObject()->GetLinearVelocity().y > 30) {
					KeepForce = false;
				}
				else {
					jumpCubes.at(i)->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 40, 0));
				}
			}
		}

		for (int i = 0; i < jumpCubes.size(); ++i) {
			if (jumpCubes.at(i)->GetRenderObject()->GetTransform()->GetPosition().y > 25.0f - i * 2) {
				MainSphere->GetPhysicsObject()->SetLinearVelocity(Vector3(-8, 14, 0));
				jumpCubes.at(i)->GetPhysicsObject()->SetLinearVelocity(Vector3(0, -40, 0));
			}
		}

		for (int i = 0; i < jumpCubes.size(); ++i) {
			jumpCubes.at(i)->GetPhysicsObject()->SetLinearVelocity(jumpCubes.at(i)->GetPhysicsObject()->GetLinearVelocity() * Vector3(0, 1, 0));
		}

		if (physics->GetGravityState()) {
			MainSphere->GetPhysicsObject()->SetLinearVelocity(MainSphere->GetPhysicsObject()->GetLinearVelocity() + Vector3(0, 1.5f, 0));
			world->RemoveGameObject(antiGravityBlock, false);
		}

		if (MainSpherePos != Vector3(0, 0, 0)) {
			if ((MainSpherePos - MainSphere->GetRenderObject()->GetTransform()->GetPosition()).Length() < 0.001f) {
				MainSphere->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
				MainSphere->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
			}
		}
		if (!jumpCubesPos.empty()) {
			for (int i = 0; i < jumpCubes.size(); ++i) {
				if (jumpCubesPos.at(i) != Vector3(0, 0, 0)) {
					if ((jumpCubesPos.at(i) - jumpCubes.at(i)->GetTransform().GetPosition()).Length() < 0.1f) {
						jumpCubes.at(i)->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 1, 0));
						jumpCubes.at(i)->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
					}
				}
			}
		}

		MainSpherePos = MainSphere->GetRenderObject()->GetTransform()->GetPosition();
		for (int i = 0; i < jumpCubes.size(); ++i) {
			jumpCubesPos.push_back(jumpCubes.at(i)->GetTransform().GetPosition());
		}

		testStateObject->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
		break;

	case 2:
		Debug::Print("You Have Reached the Goal!", Vector2(2, 5));
		Debug::Print("You Have Spent " + timeString + " Seconds to Complete!", Vector2(2, 10));
		Debug::Print("Press Green Button To Back Main Menu.", Vector2(2, 15));
		world->GetMainCamera()->SetPosition(Vector3(3000, 10, 1000));
		world->GetMainCamera()->SetPitch(0);
		world->GetMainCamera()->SetYaw(0);
		break;

	case 3:
		world->GetMainCamera()->SetPosition(Vector3(50, 150, 50));
		world->GetMainCamera()->SetPitch(-90);
		world->GetMainCamera()->SetYaw(0);
		time += dt;

		if (physics->GetCatchState()) {
			bot->ClearNodes();
			score -= 10;
			PlayerSphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(80, 1, 80));
			EnemySphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(10, 1, 10));
			EnemySphere->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			physics->SetCatchState(false);
		}

		if (physics->GetReachMazeGoal()) {
			bot->ClearNodes();
			score += 1000;
			PlayerSphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(80, 1, 80));
			EnemySphere->GetRenderObject()->GetTransform()->SetPosition(Vector3(10, 1, 10));
			EnemySphere->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
			physics->SetReachMazeGoal(false);
			SetSceneState(4);
		}

		timeString = std::to_string(time);
		Debug::Print("Time: " + timeString, Vector2(65, 15));
		scoreString = std::to_string(score);
		Debug::Print("Score: " + scoreString, Vector2(65, 20));


		bot->FindPlayer(bot->GetPosition(), player->GetPosition());
		if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::LEFT)) {
			PlayerSphere->GetPhysicsObject()->AddForce(Vector3(-1, 0, 0) * forceMagnitude * 0.1);
		}
		if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::RIGHT)) {
			PlayerSphere->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * forceMagnitude * 0.1);
		}
		if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::UP)) {
			PlayerSphere->GetPhysicsObject()->AddForce(Vector3(0, 0, -1) * forceMagnitude * 0.1);
		}
		if (Window::GetKeyboard()->KeyDown(NCL::KeyboardKeys::DOWN)) {
			PlayerSphere->GetPhysicsObject()->AddForce(Vector3(0, 0, 1) * forceMagnitude * 0.1);
		}
		bot->DisplayPathfinding();
		if (bot->GettestNodes().size() > 2) {
			EnemySphere->GetPhysicsObject()->AddForce((bot->GettestNodes()[1] - bot->GettestNodes()[0]) * 5);
		}
		else if ((EnemySphere->GetRenderObject()->GetTransform()->GetPosition() - PlayerSphere->GetRenderObject()->GetTransform()->GetPosition()).Length() < 20) {
			EnemySphere->GetPhysicsObject()->AddForce((PlayerSphere->GetRenderObject()->GetTransform()->GetPosition() - EnemySphere->GetRenderObject()->GetTransform()->GetPosition()) * 5);
		}
		break;

	case 4:
		Debug::Print("You Have Reached the Goal!", Vector2(2, 5));
		Debug::Print("You Have Spent " + timeString + " Seconds to Complete!", Vector2(2, 10));
		Debug::Print("Your Final Score(s): " + scoreString, Vector2(2, 15));
		Debug::Print("Press Green Button To Back Main Menu.", Vector2(2, 20));
		world->GetMainCamera()->SetPosition(Vector3(3000, 10, 1000));
		world->GetMainCamera()->SetPitch(0);
		world->GetMainCamera()->SetYaw(0);
		break;
	}
	
	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;
		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));
		Matrix4 modelMat = temp.Inverse();
		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler();
		//Looking front object
		Vector3 posOffset = lockedObject->GetBoundingVolume()->halfSize();
		Ray ray(objPos + Vector3{ 0, 0, -posOffset.GetMaxElement() }, Vector3(0, 0, -1));
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			lookedObject = (GameObject*)closestCollision.node;
			lookedObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
		}
		Debug::DrawAxisLines(Matrix4::Translation(selectionObject->GetTransform().GetPosition()), 2.0f);
	}
	else if (lookedObject != nullptr) {
		lookedObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
		lookedObject = nullptr;
	}

	if (testStateObject) {
		testStateObject->Update(dt);
	}

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();
	return onGoing;
}

void TutorialGame::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		for (int i = 0; i <= jumpCubes.size(); ++i) {
			jumpCubes.pop_back();
		}
		selectionObject = nullptr;
		lockedObject	= nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U)) {
		std::cout << "Bridge Falling." << std::endl;
		BridgeConstraintTest();
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}

	ChangeCollisionLayer();
}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0,-10,0));
	}
}

void TutorialGame::ChangeCollisionLayer() {
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM0)) {
		layer = 0;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM1)) {
		layer = 1;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM2)) {
		layer = 2;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM3)) {
		layer = 3;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM4)) {
		layer = 4;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
		layer = 5;
	}
}

void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(70, 35, -70));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	MainMenu();
	GoalMenu();
	ScoreMenu();
	InitFirstLevel();
	InitSecondLevel();
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float invCubeMass	= 5;
	int numLinks		= 10;
	float maxDistance	= 20;
	float cubeDistance	= 20;

	Vector3 startPos = Vector3(1000, 120, 100);

	GameObject* start		= AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, Quaternion(0,0,0,0), 0);
	GameObject* end			= AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, Quaternion(0, 0, 0, 0), 0, 0);
	GameObject* previous	= start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + i) * cubeDistance, 0, 0), cubeSize, Quaternion(0, 0, 0, 0), 0, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);
	world->AddConstraint(constraint);
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject("Floor");

	Vector3 floorSize	= Vector3(100, 2, 100);
	AABBVolume* volume	= new AABBVolume(floorSize);
	floor->SetLayer(0);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float innerRadius, int layer, float inverseMass, string name) {
	GameObject* sphere = new GameObject(name);

	Vector3 sphereSize = Vector3(radius, radius, radius);
	Vector3 innerSize = Vector3(innerRadius, innerRadius, innerRadius);
	SphereVolume* volume = new SphereVolume(radius, innerRadius);
	sphere->SetLayer(layer);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume(), 1.0f));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	GameObject* capsule = new GameObject("Capsule");

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetLayer(0);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;

}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, Quaternion orientation, int layer, bool aabb, float inverseMass, string name) {
	GameObject* cube = new GameObject(name);
	AABBVolume* aabbvolume = new AABBVolume(dimensions);
	OBBVolume* obbvolume = new OBBVolume(dimensions);

	if (aabb) {
		cube->SetBoundingVolume((CollisionVolume*)aabbvolume);
	}
	else {
		cube->SetBoundingVolume((CollisionVolume*)obbvolume);
	}
	cube->SetLayer(layer);
	
	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(orientation);;
	

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume(), 0.1f));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius, float innerRadius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, innerRadius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	float innerRadius = 0.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims, Quaternion(0,0,0,0), 0, false);
			}
			else {
				AddSphereToWorld(position, sphereRadius, innerRadius, 0);
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, Quaternion(0, 0, 0, 0), 0, 1.0f);
		}
	}
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	AddBonusToWorld(Vector3(10, 5, 0));
}

void TutorialGame::InitFirstLevel() {
	//Layer 0 -> Pushable Object / Buttons
	//Layer 1 -> Wall / Floor
	//Layer 2 -> MainSphere
	//Layer 3 -> PowerUp
	//Layer 4 -> Goal
	//Layer 5 -> Obstacle

	//Main Sphere Spawn
	MainSphere = AddSphereToWorld(Vector3(1097, 26.0f, -98), 0.8f, 0.0f, 2, 10, "MainSphere");

	//Starting Area
	PushableCube = AddCubeToWorld(Vector3(1098.5f, 25.8f, -98), Vector3(0.5f, 1, 1), Quaternion(0, 0, 0, 0), 0, true, 1.0f);
	PushableCube->GetRenderObject()->SetColour(Vector3(0.5f, 0.5f, 0));
	AddCubeToWorld(Vector3(1096.3f, 25.0f, -98), Vector3(3, 0.2f, 1), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1096.3f, 25.9f, -96.8f), Vector3(3, 1.2f, 0.2f), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1096.3f, 25.9f, -99.2f), Vector3(3, 1.2f, 0.2f), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1093.6f, 27.0f, -98), Vector3(0.1f, 0.1f, 1.4f), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1099.4f, 27.0f, -98), Vector3(0.1f, 0.1f, 1.4f), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1088.0f, 22.8f, -98), Vector3(0.2f, 5, 1), Quaternion(0, 0, -0.65, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1088.0f, 23.9f, -96.8f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, -0.65, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1088.0f, 23.9f, -99.2f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, -0.65, 1), 1, false, 0.0f);

	//Jump Cube Area
	for (int x = 0; x < 3; ++x) {
		Vector3 position(1081 - x * 11, 17.5f - 1.5f * x, -98.0f);
		AddJumpCubeSet(position);
		
	}
	testStateObject = AddStateObjectToWorld(Vector3(1054, 14, -98.0f));
	testStateObject->GetRenderObject()->SetColour(Vector3(1, 0, 0));
	AddCubeToWorld(Vector3(1075, 18, -70.5f), Vector3(3.2f, 0.2f, 2), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1078.1f, 20, -70.5f), Vector3(0.1f, 1, 2), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1071.9f, 20, -70.5f), Vector3(0.1f, 1, 2), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1075, 18.5, -68.5f), Vector3(3.2f, 0.5f, 0.1f), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	AddCubeToWorld(Vector3(1075, 22.5f, -70.5f), Vector3(3.2f, 0.2f, 2), Quaternion(0, 0, 0, 0), 1, true, 0.0f);
	buttonBase = AddCubeToWorld(Vector3(1075, 20, -72), Vector3(3, 2, 0.2f), Quaternion(0, 0, 0, 0), 0, true, 0.0f, "ButtonBase");
	button = AddCubeToWorld(Vector3(1075, 20, -70), Vector3(3, 2, 1), Quaternion(0, 0, 0, 0), 0, true, 5.0f, "Button");
	buttonBase->GetRenderObject()->SetColour(Vector3(0.5f, 0.5f, 0));
	button->GetRenderObject()->SetColour(Vector3(0.5f, 0.5f, 0));
	SpringConstraint* constraint = new SpringConstraint(buttonBase, button, Vector3(0, 0, 2), Vector3(0, 0, 1));
	world->AddConstraint(constraint);

	//Down Slope Area
	AddCubeToWorld(Vector3(1050.0f, 15.0f, -98.0f), Vector3(5, 0.2f, 1), Quaternion(0, 0, 0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1050.0f, 16.1f, -96.8f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, -0.75, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1050.0f, 16.1f, -99.2f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, -0.75, 1), 1, false, 0.0f);

	AddCubeToWorld(Vector3(1044.0f, 12.0f, -95.0f), Vector3(5, 0.2f, 1), Quaternion(0, 1, 0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1044.0f, 14.1f, -100.0f), Vector3(1.5f, 0.2f, 0.2f), Quaternion(0, 0, 0, 0), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1042.7f, 13.1f, -95.0f), Vector3(0.2f, 5, 0.2f), Quaternion(1, 0, 0, 0.85), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1045.3f, 13.1f, -93), Vector3(0.2f, 3.5f, 0.2f), Quaternion(1, 0, 0, 0.85), 1, false, 0.0f);
	
	AddCubeToWorld(Vector3(1040.5f, 9.0f, -88.5f), Vector3(5, 0.2f, 1), Quaternion(0, 0, 0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1040.5f, 10.1f, -87.2f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, -0.75, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1039.3f, 10.1f, -89.7f), Vector3(0.2f, 3.7f, 0.2f), Quaternion(0, 0, -0.75, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1045.5f, 12.1f, -88.5f), Vector3(0.2f, 0.2f, 1.0f), Quaternion(0, 0, 0, 0), 1, false, 0.0f);

	//Anti Gravity Area
	antiGravityBlock = AddCubeToWorld(Vector3(1036.5f, 9.5f, -88.5f), Vector3(0.5f, 0.5f, 0.5f), Quaternion(0, 0, 0, 0), 3, true, 0.0f, "antiGravityBlock");
	antiGravityBlock->GetPhysicsObject()->SetCollisionResponces(false);
	antiGravityBlock->GetRenderObject()->SetColour(Vector3(0, 0, 0));

	AddCubeToWorld(Vector3(1035.5f, 14.0f, -88.5f), Vector3(5, 0.2f, 1), Quaternion(0, 0, -0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1035.5f, 13.1f, -87.2f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, 0.75, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1035.5f, 13.1f, -89.7f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, 0.75, 1), 1, false, 0.0f);

	AddCubeToWorld(Vector3(1027.5f, 19.0f, -88.5f), Vector3(5, 0.2f, 1), Quaternion(0, 0, -0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1027.5f, 18.1f, -87.2f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, 0.75, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1027.5f, 18.1f, -89.7f), Vector3(0.2f, 5, 0.2f), Quaternion(0, 0, 0.75, 1), 1, false, 0.0f);

	AddCubeToWorld(Vector3(1022.0f, 24.0f, -85.0f), Vector3(5, 0.2f, 1), Quaternion(0, 1, -0.15, 1), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1020.7f, 23.1f, -85.0f), Vector3(0.2f, 5, 0.2f), Quaternion(1, 0, 0, -0.85), 1, false, 0.0f);
	AddCubeToWorld(Vector3(1023.3f, 23.1f, -85), Vector3(0.2f, 5, 0.2f), Quaternion(1, 0, 0, -0.85), 1, false, 0.0f);

	//GOAL
	Goal = AddCubeToWorld(Vector3(1021.0f, 30.0f, -78.0f), Vector3(3, 0.2f, 3), Quaternion(0, 0, 0, 0), 4, false, 0.0f);
	Goal->GetRenderObject()->SetColour(Vector3(0, 0, 1));

	topBlock = AddCubeToWorld(Vector3(1000, 40, -90), Vector3(100, 0.2f, 20), Quaternion(0, 0, 0, 0), 5, true, 0.0f, "Top Block");
	topBlock->GetRenderObject()->SetColour(Vector3(1, 0, 0));
	bottomBlock = AddCubeToWorld(Vector3(1000, 0, -90), Vector3(100, 0.2f, 20), Quaternion(0, 0, 0, 0), 5, true, 0.0f, "Bottom Block");
	bottomBlock->GetRenderObject()->SetColour(Vector3(1, 0, 0));
}

void TutorialGame::AddJumpCubeSet(Vector3 _position) {
	GameObject* jumpCube = AddCubeToWorld(_position, Vector3(0.7f, 2, 1), Quaternion(0, 0, -0.55, 1), 1, false, 0.5f, "JumpCube");
	jumpCubes.push_back(jumpCube);
	AddCubeToWorld(_position - Vector3(0.4, -1.2f, 0), Vector3(1, 0.1f, 1), Quaternion(0, 0, 0, 0), 1, false, 0.0f, "JumpCube");
	GameObject* sphere = AddSphereToWorld(_position - Vector3(-1.5f, 1, 0), 1, 0.0f, 1, 0.0f);
	AddSphereToWorld(_position - Vector3(2, 0.7f, 0), 1, 0.0f, 1, 0.0f);
	AddSphereToWorld(_position - Vector3(-4, -1, 0), 1, 0.0f, 1, 0.0f);
}

void TutorialGame::InitSecondLevel() {
	PlayerSphere = AddSphereToWorld(Vector3(80, 1, 80), 4, 0, 6, 1, "PlayerSphere");
	PlayerSphere->GetRenderObject()->SetColour(Vector3(0, 1, 0));
	EnemySphere = AddSphereToWorld(Vector3(10, 1, 10), 4, 0, 6, 1, "EnemySphere");
	EnemySphere->GetRenderObject()->SetColour(Vector3(1, 0, 0));
	AddCubeToWorld(Vector3(50, -0.5f, 50), Vector3(50, 0.2f, 50), Quaternion(0, 0, 0, 0), 1, true, 0, "Floor");
	AddCubeToWorld(Vector3(0, 2, 50), Vector3(5, 10, 50), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(90, 2, 25), Vector3(5, 10, 20), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(90, 2, 75), Vector3(5, 10, 20), Quaternion(0, 0, 0, 0), 1, true, 0);
	MazeGoal = AddCubeToWorld(Vector3(100, 2, 50), Vector3(5, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0, "MazeGoal");
	MazeGoal->GetRenderObject()->SetColour(Vector3(0, 1, 0));

	AddCubeToWorld(Vector3(50, 2, 0), Vector3(50, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(50, 2, 90), Vector3(50, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(20, 2, 15), Vector3(5, 10, 10), Quaternion(0, 0, 0, 0), 1, true, 0); //2V
	AddCubeToWorld(Vector3(65, 2, 20), Vector3(10, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(60, 2, 35), Vector3(5, 10, 10), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(30, 2, 40), Vector3(25, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(15, 2, 70), Vector3(10, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(40, 2, 65), Vector3(5, 10, 10), Quaternion(0, 0, 0, 0), 1, true, 0);
	AddCubeToWorld(Vector3(65, 2, 70), Vector3(20, 10, 5), Quaternion(0, 0, 0, 0), 1, true, 0);

}

void TutorialGame::MainMenu() {
	world->GetMainCamera()->SetPosition(Vector3(2000, 10, 1000));
	world->GetMainCamera()->SetPitch(0);
	world->GetMainCamera()->SetYaw(0);
	FirstLevel = AddCubeToWorld(Vector3(1950, 10, 900), Vector3(10, 10, 10), Quaternion(0, -0.56, 0, 1), 0, true, 0, "First Level");
	FirstLevel->GetRenderObject()->SetColour(Vector3(0, 1, 0));
	SecondLevel = AddCubeToWorld(Vector3(2000, 10, 900), Vector3(10, 10, 10), Quaternion(0, 0, 0, 0), 0, true, 0, "Second Level");
	SecondLevel->GetRenderObject()->SetColour(Vector3(0, 0, 1));
	Quit = AddCubeToWorld(Vector3(2050, 10, 900), Vector3(10, 10, 10), Quaternion(0, 0.56, 0, 1), 0, true, 0, "Quit");
	Quit->GetRenderObject()->SetColour(Vector3(1, 0, 0));
}

void TutorialGame::GoalMenu() {
	BackMenu = AddCubeToWorld(Vector3(3000, -10, 900), Vector3(10, 10, 10), Quaternion(0, 0, 0.45f, 1), 0, true, 0, "Menu Button");
	BackMenu->GetRenderObject()->SetColour(Vector3(0, 1, 0));
	world->GetMainCamera()->SetPosition(Vector3(3000, 10, 1000));
	world->GetMainCamera()->SetPitch(0);
	world->GetMainCamera()->SetYaw(0);
}

void TutorialGame::ScoreMenu() {
	
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 3.0f;
	float inverseMass	= 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

StateGameObject* TutorialGame::AddStateObjectToWorld(const Vector3& position) {
	StateGameObject* wall = new StateGameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.1f, 2, 1));
	wall->SetBoundingVolume((CollisionVolume*)volume);
	wall->GetTransform()
		.SetScale(Vector3(0.1f, 2, 1) * 2)
		.SetPosition(position);

	wall->SetRenderObject(new RenderObject(&wall->GetTransform(), cubeMesh, nullptr, basicShader));
	wall->SetPhysicsObject(new PhysicsObject(&wall->GetTransform(), wall->GetBoundingVolume()));

	wall->GetPhysicsObject()->SetInverseMass(1.0f);
	wall->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(wall);

	return wall;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				int layer = selectionObject->GetLayer();
				switch (layer) {
				case 0:
					selectionObject->GetRenderObject()->SetColour(Vector4(0.5f, 0.5f, 0, 1));
					break;
				case 3:
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));
					break;
				case 4:
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
					break;
				case 5:
					selectionObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
					break;
				default:
					selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
					break;
				}

				if (selectionObject->GetName() == "First Level") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				}
				if (selectionObject->GetName() == "Second Level") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
				}
				if (selectionObject->GetName() == "Quit") {
					selectionObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
				}
				if (selectionObject->GetName() == "Menu Button") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				}

				selectionObject = nullptr;
				lockedObject	= nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				renderer->DrawString("Name:" + selectionObject->GetName(), Vector2(10, 10));
				std::string canCollied;
				if (selectionObject->GetPhysicsObject()->GetCollisionResponces()) {
					canCollied = "True";
				}
				else { canCollied = "False"; }
				renderer->DrawString("Colliedability: " + canCollied, Vector2(10, 15));
				int positionx = selectionObject->GetRenderObject()->GetTransform()->GetPosition().x;
				int positiony = selectionObject->GetRenderObject()->GetTransform()->GetPosition().y;
				int positionz = selectionObject->GetRenderObject()->GetTransform()->GetPosition().z;
				renderer->DrawString("Position: " + std::to_string(positionx) + " " + std::to_string(positiony) + " " + std::to_string(positionz), Vector2(10, 20));

				if (selectionObject->GetName() == "First Level") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
					SetSceneState(1);
					world->GetMainCamera()->SetPosition(Vector3(1080, 30, -60));
				}
				if (selectionObject->GetName() == "Second Level") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 0, 1, 1));
					SetSceneState(3);
					world->GetMainCamera()->SetPosition(Vector3(50, 150, 50));
					world->GetMainCamera()->SetPitch(-90);
				}
				if (selectionObject->GetName() == "Quit") {
					selectionObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
					onGoing = false;

				}
				if (selectionObject->GetName() == "Menu Button") {
					selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
					time = 0;
					score = 0;
					SetSceneState(0);
				}

				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (lockedObject) {
		renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if(selectionObject){
		renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void TutorialGame::MoveSelectedObject() {
	//renderer->DrawString("Click Force: " + std::to_string(forceMagnitude), Vector2(10, 20));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 50.0f;

	if (!selectionObject) {
		return;
	}

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForce(ray.GetDirection() * forceMagnitude);
			}
		}
	}
}