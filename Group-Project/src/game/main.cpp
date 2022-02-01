#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include <iostream>

int main() {
	std::cout << "Hello World" << std::endl;

	int maxproxies = 1024;
	btVector3 worldAabbMin = { -100, -100 , 100 };
	btVector3 worldAabbMax = { -100, -100 , 100 };

	btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxproxies);

	return 0;
}