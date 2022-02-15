#include "PhysicsTestScene.h"
//#include "../Physics/btNClmotionState.h"
#include "../OpenGLRendering/OGLMesh.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../common/TextureLoader.h"

#include "../CSC8503/GameWorld.h"
#include "../Common/Camera.h"

#include <iomanip>

using namespace NCL;
using namespace CSC8503;
using namespace Maths;

PhysicsTestScene::PhysicsTestScene() {

	world = new GameWorld();
	renderer = new GameTechRenderer(*world);

	//Default Broadphase
	maxProxies = 1024;
	worldAabbMin = {-100, -100, -100};
	worldAabbMax = {100, 100, 100};
	broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	InitAssets();
	InitCamera();
	InitScene();
}

PhysicsTestScene::~PhysicsTestScene() {
	delete world;
	delete renderer;

	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;

	delete sphereMesh;
	delete basicTex;
	delete basicShader;

}

void PhysicsTestScene::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("cube.msh", &cubeMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

void PhysicsTestScene::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(1000.0f);
	world->GetMainCamera()->SetPitch(0.0f);
	world->GetMainCamera()->SetYaw(0.0f);
	world->GetMainCamera()->SetPosition(Vector3(0, 0, 50));
}

void PhysicsTestScene::InitScene() {
	world->ClearAndErase();
	dynamicsWorld->clearForces();

	//sphere
	//Set position
	sphere = new GameEntity("Sphere");
	sphere->GetTransform()
		.SetPosition(Vector3(0, 0, 0))
		.SetScale(Vector3(1, 1, 1))
		.SetOrientation({ 0,0,0,1 });

	//Set Renderer
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));

	//Set Physics
	//sphereMotion = new btNCLMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	sphere->SetMotionState();
	sphere->SetCollisionShape(new btSphereShape(1));
	//CI information
	sphere->SetRigidBody(new btRigidBody())

	//btRigidBody::btRigidBodyConstructionInfo sphereCI(0, sphereMotion, sphereShape, btVector3(0, 0, 0));
	btRigidBody* sphereBody = new btRigidBody(sphereCI);

	sphere->SetMotionState(sphereMotion);
	sphere->SetRigidBody(sphereBody);
	sphereMotion->ConvertbtTransform();
	
	world->AddGameObject(sphere);
	dynamicsWorld->addRigidBody(sphere->GetRigidBody()); //Dynamics world inside of our game world class.
	
	//ground
}

void PhysicsTestScene::UpdateGame(float dt) {

	world->GetMainCamera()->UpdateCamera(dt);
	UpdateKeys();
	renderer->Render();
}

void PhysicsTestScene::UpdateKeys() {

}