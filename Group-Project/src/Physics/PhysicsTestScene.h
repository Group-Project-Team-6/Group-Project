#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "../CSC8503/GameTechRenderer.h"
//#include "../Physics/btNClmotionState.h"
#include "../game/GameEntity.h"
#include "../Physics/btNClmotionState.h"

#include "../Audio/AudioManager.h"

class PhysicsTestScene {
	public:
		PhysicsTestScene();

		~PhysicsTestScene();

		void UpdateGame(float dt);

	private:
		void InitAssets();
		void InitScene();
		void InitCamera();
		void UpdateKeys();

		int maxProxies;
		btVector3 worldAabbMin;
		btVector3 worldAabbMax;
		btAxisSweep3* broadphase;

		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;

		btDefaultMotionState* sphereMotion;

		GameEntity* sphere;
		GameEntity* ground;

		OGLMesh* sphereMesh = nullptr;
		OGLMesh* cubeMesh = nullptr;

		OGLTexture* basicTex = nullptr;
		OGLShader* basicShader = nullptr;

		GameTechRenderer* renderer;
		GameWorld* world;
		AudioManager* audioManager;
};

