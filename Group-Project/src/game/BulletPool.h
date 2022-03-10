#pragma once
#include "Bullet.h"
#include "../common/RendererBase.h"

class BulletPool {
public:
	BulletPool() {};
	~BulletPool();

	void Create(Transform shootingPos, int lifeTime, RendererBase& renderer);
	void Animate();

private:
	//Bullet* firstAvailable;
	static const int poolSize = 100;
	Bullet bullets[poolSize];
};
