#include "PhysicsTestScene.h"
#include "../Physics/btNClmotionState.h"
#include "../OpenGLRendering/OGLMesh.h"
#include "../OpenGLRendering/OGLTexture.h"
#include "../OpenGLRendering/OGLShader.h"

//using namespace NCL;
//using namespace CSC8503;

PhysicsTestScene::PhysicsTestScene() {

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
	InitScene();
}

PhysicsTestScene::~PhysicsTestScene() {
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;

}

void PhysicsTestScene::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &sphereMesh);

}

void PhysicsTestScene::InitScene() {
	//sphere
	sphere = new GameEntity("Sphere");

	btVector3 position = { 0, 50, 0 };
	btQuaternion orientation = { 0, 0, 0, 1 };
	btNCLMotionState* sphereMotion = new btNCLMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btCollisionShape* sphereShape = new btSphereShape(1);
	btRigidBody::btRigidBodyConstructionInfo sphereCI(0, sphereMotion, sphereShape, btVector3(0, 0, 0));
	btRigidBody* sphereBody = new btRigidBody(sphereCI);


	sphere->SetMotionState(sphereMotion);
	sphere->SetCollisionShape(sphereShape);
	sphere->SetRigidBody(sphereBody);
	//sphere->SetRenderObject(new RenderObject(&sphere->ConvertbtTransform(), sphereMesh, basicTex, basicShader));
	
	dynamicsWorld->addRigidBody(sphere->GetRigidBody());
	
	//ground
}

void PhysicsTestScene::UpdateGame(float dt) {

}