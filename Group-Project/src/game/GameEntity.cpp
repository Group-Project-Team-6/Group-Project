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

void GameEntity::UpdateRenderPositions()
{

	bttransform = rigidBody->getWorldTransform();

	btRot = bttransform.getRotation();
	btPos = bttransform.getOrigin();

	nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
	nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

	transform.SetOrientation(nclRot);
	transform.SetPosition(nclPos);

}

