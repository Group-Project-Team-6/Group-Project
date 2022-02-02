#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class PhysicsTestScene {
		public:
			PhysicsTestScene();

			~PhysicsTestScene();

			void UpdateGame(float dt);

		private:
			void InitScene();

			int maxProxies;
			btVector3 worldAabbMin;
			btVector3 worldAabbMax;
			btAxisSweep3* broadphase;

			btDefaultCollisionConfiguration* collisionConfiguration;
			btCollisionDispatcher* dispatcher;
			btSequentialImpulseConstraintSolver* solver;
			btDiscreteDynamicsWorld* dynamicsWorld;

			//GameObject* sphere;
			//GameObject* ground;

		};
	}
}

