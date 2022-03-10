#include "BulletPool.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <assert.h>


BulletPool::~BulletPool(){}

void BulletPool::Create(Transform& shootingPos, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {
	for (int i = 0; i < poolSize; i++) {
		if (!bullets[i].inUse()) {
			bullets[i].Init(shootingPos, force, lifeTime, world, physicsWorld);
			//std::cout << &world << std::endl;
			//std::cout << &physicsWorld << std::endl;
			return;
		}
	}
}

void BulletPool::Animate() {

	for (int i = 0; i < poolSize; i++) {
		bullets[i].Animate();
	}
}