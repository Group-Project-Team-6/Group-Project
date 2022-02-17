#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "../Common/RenderObject.h"
#include "../Physics/PhysicsObject.h"

#include "../CSC8503/Transform.h"

#include <vector>

using std::vector;
using std::string;


class GameEntity {
public:
	GameEntity(string name = "");
	~GameEntity();

	//Graphics
	RenderObject* GetRenderObject() const {
		return renderObject;
	}

	void SetRenderObject(RenderObject* newObject) {
		renderObject = newObject;
	}

	//Physics
	btRigidBody* GetRigidBody() const {
		return rigidBody;
	}

	void SetRigidBody(btRigidBody* newRigidBody) {
		rigidBody = newRigidBody;
	}

	//General
	Transform& GetTransform(){
		return transform;
	}

	void SetTransform(Transform newtransform) {
		transform = newtransform;
	}

	btTransform& GetbtTransform() {
		return btTransform;
	}

	void SetbtTransform(btTransform newbtTransform) {
		btTransform = newbtTransform;
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

	void UpdateRenderPositions() {



		//Quaternion NCLRot = transform.GetOrientation();
		//Vector3 NCLPos = transform.GetPosition();

		//btQuaternion btRot = { NCLRot.x, NCLRot.y, NCLRot.z };
		//btVector3 btPos = { NCLPos.x, NCLPos.y, NCLPos.z };

		//btTransform.setRotation(btRot);
		//btTransform.setOrigin(btPos);
	}

protected:
	RenderObject* renderObject;
	btRigidBody* rigidBody;
	
	Transform transform;
	btTransform btTransform;

	string name;

	bool isActive;
	int	worldID;
};


