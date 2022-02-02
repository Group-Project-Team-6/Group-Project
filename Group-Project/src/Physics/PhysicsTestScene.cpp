#include "PhysicsTestScene.h"

using namespace NCL;
using namespace CSC8503;

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

	InitScene();
}

PhysicsTestScene::~PhysicsTestScene() {
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	delete dynamicsWorld;

}

void PhysicsTestScene::InitScene() {
	//sphere
	//sphere = new GameObject();

	//ground
}

void PhysicsTestScene::UpdateGame(float dt) {

}