#include "BulletPool.h"
#include <assert.h>

BulletPool::~BulletPool(){}

void BulletPool::Create(Transform shootingPos, int lifeTime) {
	for (int i = 0; i < poolSize; i++) {

		if (!bullets[i].inUse()) {
			bullets[i].Init(shootingPos, lifeTime);
		}
		
	}
}

void BulletPool::Animate() {

	for (int i = 0; i < poolSize; i++) {
		bullets[i].Animate();
	}
}