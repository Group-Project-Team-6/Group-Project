#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../Common/RenderObject.h"
#include "../CSC8503/Transform.h"

#include <vector>

using std::vector;
using std::string;

using namespace NCL;
using namespace CSC8503;

class GameEntity {
public:
	GameEntity(string name = "");
	~GameEntity();

	RenderObject* GetRenderObject() const {
		return renderObject;
	}

	void SetRenderObject(RenderObject* newObject) {
		renderObject = newObject;
	}

	btCollisionObject* GetCollisionObject() const {
		return collisionObject;
	}

	void SetCollisionObject(btCollisionObject* newCollisionObject) {
		collisionObject = newCollisionObject;
	}

	btDefaultMotionState* GetMotionState() const {
		return motionState;
	}

	void SetMotionState(btDefaultMotionState* newMotionState) {
		motionState = newMotionState;
	}

	btRigidBody* GetRigidBody() const {
		return rigidBody;
	}

	void SetRigidBody(btRigidBody* newRigidBody) {
		rigidBody = newRigidBody;
	}

	btCollisionShape* GetCollisionShape() {
		return collisionShape;
	}

	void SetCollisionShape(btCollisionShape* newCollisionShape) {
		collisionShape = newCollisionShape;
	}

	bool IsActive() const {
		return isActive;
	}

	void SetWorldID(int newID) {
		worldID = newID;
	}

	int	GetWorldID() const {
		return worldID;
	}

	//Transform& ConvertbtTransform(btTransform);

protected:
	RenderObject* renderObject;
	btCollisionObject* collisionObject;

	btCollisionShape* collisionShape;
	btDefaultMotionState* motionState;
	btRigidBody* rigidBody;
	
	Transform physicsTransform;

	string name;

	bool isActive;
	int	worldID;
};


