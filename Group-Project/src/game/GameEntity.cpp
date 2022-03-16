#include "GameEntity.h"


GameEntity::GameEntity(string objectName) {
	name = objectName;
	renderObject = nullptr;
	rigidBody = nullptr;
	isActive = true;
	isTrigger = false;

}

GameEntity::~GameEntity() {
	delete renderObject;
	delete rigidBody;
}
