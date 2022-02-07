#pragma once
#include "LinearMath/btMotionState.h"
#include "../CSC8503/Transform.h"

using namespace NCL;
using namespace CSC8503;

class btNCLMotionState : public btMotionState {
public:
	btNCLMotionState(const btTransform& startTrans = btTransform::getIdentity(), const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: m_graphicsWorldTrans(startTrans),
		m_centerOfMassOffset(centerOfMassOffset),
		m_startWorldTrans(startTrans),
		m_userPointer(0)

	{
	}

	//~btNCLMotionState();

	 btTransform getWorldTransform() {
		return btTransform;
	}

	void setWorldTransform(btTransform newbtTransform) {
		btTransform = newbtTransform;
	}

	void ConvertbtTransform(const btTransform intialBTTransform, Transform intialNCLTransform) {

		m_graphicsWorldTrans = intialBTTransform;
		nclTransform = intialNCLTransform;

		btQuaternion btRot = intialBTTransform.getRotation();
		btVector3 btPos = intialBTTransform.getOrigin();

		Quaternion nclRot = intialNCLTransform.GetOrientation();
		Vector3 nclPos = intialNCLTransform.GetPosition();

		nclRot.x = btRot.x;
		nclRot.y = btRot.y;
		nclRot.z = btRot.z;
		nclRot.w = btRot.w;

		nclPos.x = btPos.x;
		nclPos.y = btPos.y;
		nclPos.z = btPos.z;

		nclTransform.SetOrientation(nclRot);
		nclTransform.SetPosition(nclPos);
	}
		
protected:

	btTransform m_graphicsWorldTrans;
	btTransform m_centerOfMassOffset;
	btTransform m_startWorldTrans;
	void* m_userPointer;

	btTransform btTransform;
	Transform nclTransform;
};