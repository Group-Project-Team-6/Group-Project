#include "BulletPool.h"

void BulletPool::Create(int lifeTime) {
	for (int i = 0; i < poolSize; i++) {
		if (!bullets[i].inUse()) {
			bullets->Init(lifeTime);
			return;
		}
	}
}

/*void BulletPool::Animate() {
	for (auto i : bullets) {
		i.Animate();
	}
}*/