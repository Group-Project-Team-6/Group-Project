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
		return bttransform;
	}

	void SetbtTransform(btTransform newbtTransform) {
		bttransform = newbtTransform;
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

		bttransform = rigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}

protected:
	RenderObject* renderObject;
	btRigidBody* rigidBody;
	
	Transform transform;
	btTransform bttransform;

	btQuaternion btRot;
	btVector3 btPos;
	Quaternion nclRot;
	Vector3 nclPos;

	string name;

	bool isActive;
	int	worldID;
};


