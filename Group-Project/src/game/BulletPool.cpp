#include "BulletPool.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <assert.h>


BulletPool::~BulletPool(){}

void BulletPool::Create(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera) {
	for (int i = 0; i < poolSize; i++) {
		if (!bullets[i]->inUse()) {
			bullets[i]->Init(player, force, lifeTime, camera, !((bool)(i % 10)));
			return;
		}
	}
}

void BulletPool::Animate(btRigidBody& player, float dt) {

	for (int i = 0; i < poolSize; i++) {
		bullets[i]->Animate(player, dt);
	}
}