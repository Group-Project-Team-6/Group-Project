#pragma once
#include "Bullet.h"

class BulletPool {
public:
	//BulletPool();
	//~BulletPool();

	void Create(int lifeTime);
	//void Animate();

private:
	Bullet* firstAvailable;
	static const int poolSize = 100;
	Bullet bullets[poolSize];
};
