#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "../CSC8503/GameTechRenderer.h"
#include "../game/GameEntity.h"
#include "../common/RendererBase.h"

class PhysicsTestScene {
	public:
		PhysicsTestScene(RendererBase* renderer);

		~PhysicsTestScene();

		void UpdateGame(float dt);

	private:
		void InitAssets();
		void InitScene();

		int maxProxies;
		btVector3 worldAabbMin;
		btVector3 worldAabbMax;
		btAxisSweep3* broadphase;

		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;

		GameEntity* sphere;
		GameEntity* ground;

		MeshGeometry* sphereMesh = nullptr;
		TextureBase* basicTex = nullptr;
		ShaderBase* basicShader = nullptr;

		RendererBase* renderer = nullptr;
};

