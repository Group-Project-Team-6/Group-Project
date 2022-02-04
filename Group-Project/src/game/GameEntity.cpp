#include "GameEntity.h"


GameEntity::GameEntity(string objectName) {
	name = objectName;
	renderObject = nullptr;
	collisionObject = nullptr;
	motionState = nullptr;
	rigidBody = nullptr;
	isActive = true;

}

GameEntity::~GameEntity() {
	delete renderObject;
	delete collisionObject;
	delete motionState;
	delete rigidBody;
}
