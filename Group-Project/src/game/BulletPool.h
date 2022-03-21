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

	void Create(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera);
	void Animate(btRigidBody& player, float dt);

private:

	static const int poolSize = 100;
	Bullet* bullets[poolSize];
};
