#pragma once

#include "GameEntity.h"
#include "../CSC8503/GameTechRenderer.h"

class Item : public GameEntity {
public:
	Item(Vector3 position, int score);
	~Item();
private:
	int score;
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;

	int itemMass;
	btVector3 itemInertia;

	btDefaultMotionState* itemMotion;
	btCollisionShape* itemShape;
	btGeneric6DofConstraint* itemConstraint;
	btRigidBody* itemRigidBody;

	//Anim
};
