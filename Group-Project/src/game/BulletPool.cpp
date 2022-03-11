#include "BulletPool.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <assert.h>


BulletPool::~BulletPool(){}

void BulletPool::Create(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {
	for (int i = 0; i < poolSize; i++) {
		if (!bullets[i]->inUse()) {
			bullets[i]->Init(player, force, lifeTime, world, physicsWorld);
			return;
		}
	}
}

void BulletPool::Animate() {

	for (int i = 0; i < poolSize; i++) {
		bullets[i]->Animate();
	}
}