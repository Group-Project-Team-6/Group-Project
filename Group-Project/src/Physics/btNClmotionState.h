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

	 virtual btTransform getWorldTransform() const{
		return m_graphicsWorldTrans;
	}

	virtual void setWorldTransform(btTransform newbtTransform) {
		m_graphicsWorldTrans = newbtTransform;
	}

	void ConvertbtTransform(const btTransform intialBTTransform) {

		m_graphicsWorldTrans = intialBTTransform;

		btQuaternion btRot = intialBTTransform.getRotation();
		btVector3 btPos = intialBTTransform.getOrigin();

		Quaternion nclRot = nclTransform.GetOrientation();
		Vector3 nclPos = nclTransform.GetPosition();

		nclRot.x = btRot.x();
		nclRot.y = btRot.y();
		nclRot.z = btRot.z();
		nclRot.w = btRot.w();

		nclPos.x = btPos.x();
		nclPos.y = btPos.y();
		nclPos.z = btPos.z();

		nclTransform.SetOrientation(nclRot);
		nclTransform.SetPosition(nclPos);
	}
		
protected:

	btTransform m_graphicsWorldTrans;
	btTransform m_centerOfMassOffset;
	btTransform m_startWorldTrans;
	void* m_userPointer;

	//btTransform btTransform;
	Transform nclTransform;
};