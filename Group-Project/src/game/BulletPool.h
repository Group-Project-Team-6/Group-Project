#pragma once
#include "Bullet.h"

class  btDiscreteDynamicsWorld;

class BulletPool {
public:
	BulletPool(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) {
		for (int i = 0; i < poolSize; ++i) {
			bullets[i] = new Bullet(world, dynamicsWorld);
		}
	};
	~BulletPool();

	void Create(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld);
	void Animate();

private:

	static const int poolSize = 100;
	Bullet* bullets[poolSize];
};
