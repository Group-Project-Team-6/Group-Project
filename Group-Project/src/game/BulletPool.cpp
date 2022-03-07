#include "BulletPool.h"
#include <assert.h>

BulletPool::BulletPool() {
	firstAvailable = &bullets[0];
	for (int i = 0; i < poolSize - 1; i++) {
		bullets[i].SetNext(&bullets[i + 1]);
	}

	bullets[poolSize - 1].SetNext(NULL);
}

void BulletPool::Create(int lifeTime) {
	for (int i = 0; i < poolSize; i++) {
		assert(firstAvailable != NULL); //Make more regular use of?

		Bullet* newbullet = firstAvailable;
		firstAvailable = newbullet->getNext();

		//newbullet->Init(lifeTime);
	}
}

void BulletPool::Animate() {

	for (int i = 0; i < poolSize; i++) {
		if (bullets[i].Animate()) {
			bullets[i].SetNext(firstAvailable);
			firstAvailable = &bullets[i];
		}
	}
}