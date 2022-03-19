#include "GameEntity.h"


GameEntity::GameEntity(string objectName) {
	name = "";
	renderObject = nullptr;
	rigidBody = nullptr;
	ghost = nullptr;
	isActive = true;
	isTrigger = false;

}

GameEntity::~GameEntity() {
	delete renderObject;
	delete rigidBody;
	delete ghost;
}
