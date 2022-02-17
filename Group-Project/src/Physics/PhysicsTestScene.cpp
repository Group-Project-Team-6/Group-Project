#include "PhysicsTestScene.h"

#include "../OpenGLRendering/OGLMesh.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLShader.h"
#include "../common/TextureLoader.h"

#include "../CSC8503/GameWorld.h"
#include "../Common/Camera.h"
//#include "../CSC8503/Transform.h"

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
	world->GetMainCamera()->SetPosition(Vector3(0, 0, 0));
}

void PhysicsTestScene::InitScene() {
	world->ClearAndErase();
	dynamicsWorld->clearForces();

	//sphere
	//Set position
	sphere = new GameEntity("Sphere");
	sphere->GetTransform()
		.SetPosition(Vector3(0, 25, -200))
		.SetScale(Vector3(1, 1, 1))
		.SetOrientation({ 0,0,0,1 });

	//Set Renderer
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));

	//Set Physics
	sphere->ConvertTobtTransform();

	int sphereMass = 10;
	btVector3 sphereInertia(0, 0, 0);

	btDefaultMotionState* sphereMotion = new btDefaultMotionState(sphere->GetbtTransform());
	btCollisionShape* sphereShape = new btSphereShape(0.01);

	sphereShape->calculateLocalInertia(sphereMass, sphereInertia);

	btRigidBody::btRigidBodyConstructionInfo sphereCI(sphereMass, sphereMotion, sphereShape, sphereInertia);
	sphere->SetRigidBody(new btRigidBody(sphereCI));

	world->AddGameObject(sphere);
	dynamicsWorld->addRigidBody(sphere->GetRigidBody());

	//ground
	ground = new GameEntity("Ground");
	ground->GetTransform()
		.SetPosition(Vector3(0, 0, -200))
		.SetScale(Vector3(100, 1, 100))
		.SetOrientation(Quaternion(0, 0, 0, 1));

	ground->SetRenderObject(new RenderObject(&ground->GetTransform(), cubeMesh, basicTex, basicShader));

	ground->ConvertTobtTransform();

	int groundMass = 0;

	btDefaultMotionState* groundMotion = new btDefaultMotionState(ground->GetbtTransform());
	//btCollisionShape* groundShape = new btBoxShape({ 50, 1, 50 });
	//btCollisionShape* groundShape = new btBoxShape({ 50, 50, 1 });
	//btCollisionShape* groundShape = new btBoxShape({ 1, 50, 50 });
	btCollisionShape* groundShape = new btStaticPlaneShape({ 0, 1, 0 }, 1);
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, btVector3(0, 0, 0));
	ground->SetRigidBody(new btRigidBody(groundCI));

	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());

}

void PhysicsTestScene::UpdateGame(float dt) {

	dynamicsWorld->stepSimulation(1 / 60.f, 10);

	world->GetMainCamera()->UpdateCamera(dt);
	UpdateKeys();
	renderer->Render();

	
	btTransform test;
	sphere->GetRigidBody()->getMotionState()->getWorldTransform(test);
	//std::cout << "Sphere Height " << test.getOrigin().getY() << std::endl;
}

void PhysicsTestScene::UpdateKeys() {

}