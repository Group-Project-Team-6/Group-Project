#include "GameEntity.h"


GameEntity::GameEntity(string objectName) {
	name = "test";
	//name = objectName;
	renderObject = nullptr;
	rigidBody = nullptr;
	ghost = nullptr;
	isActive = true;
	isTrigger = false;

}

GameEntity::~GameEntity() {
	delete renderObject;
	delete rigidBody;
}
