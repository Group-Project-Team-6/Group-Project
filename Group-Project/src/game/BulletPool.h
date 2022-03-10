#pragma once
#include "Bullet.h"

class  btDiscreteDynamicsWorld;

class BulletPool {
public:
	BulletPool() {};
	~BulletPool();

	void Create(Transform& shootingPos, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld);
	void Animate();

private:
	//Bullet* firstAvailable;
	static const int poolSize = 1;
	Bullet bullets[poolSize];
};
