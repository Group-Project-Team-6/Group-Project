#include "Bullet.h"

void Bullet::Init(int lifeTime) {
	//Pos from parameter
	frameLeft = lifeTime;
}

void Bullet::Animate() {
	if (!inUse()) return;

	frameLeft--;
	//increment position by velocity
}