#pragma once

#include "../CSC8503/Transform.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class TransformConverter {
public:
	TransformConverter();
	~TransformConverter();

protected:
	Transform* NCLBTConvert(btTransform& newTransform) {

		btTransform = newTransform;
		btRot = btTransform.getRotation();
		btPos = btTransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		nclTransform.SetOrientation(nclRot);
		nclTransform.SetPosition(nclPos);

		return &nclTransform;
	}
	btTransform* BTNCLConvert(Transform& newTransform) {
		nclTransform = newTransform;
		nclRot = nclTransform.GetOrientation();
		nclPos = nclTransform.GetPosition();
		
		btRot = { nclRot.x, nclRot.y, nclRot.z, nclRot.w };
		btPos = { nclPos.x, nclPos.y, nclPos.z };

		btTransform.setRotation(btRot);
		btTransform.setOrigin(btPos);

		return &btTransform;
	}

	btTransform& btTransform;
	Transform& nclTransform;

	btVector3 btPos;
	btQuaternion btRot;

	Vector3 nclPos;
	Quaternion nclRot;
};
