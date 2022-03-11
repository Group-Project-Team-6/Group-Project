#pragma once

#include "GameEntity.h"
//#include "../CSC8503/GameTechRenderer.h"
//#include "../common/TextureLoader.h"

class Wall : public GameEntity {
public:
	Wall(Transform buildTransform);
	~Wall();

	void InitAssets(); //Temp

	virtual btRigidBody* GetRigidBody() const override {
		return wallRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		wallRigidBody = newRigidBody;
	}

	void UpdateCollShape(float x, float y, float z) { wallShape->setLocalScaling({(x / 2.0f), (y / 2.0f), (z / 2.0f)}); }

	virtual void UpdateRenderPositions() override {

		bttransform = wallRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}
private:
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;

	btDefaultMotionState* wallMotion;
	btCollisionShape* wallShape;
	btRigidBody* wallRigidBody;

	//graphics
	OGLMesh* wallMesh;
	OGLTexture* wallTex;
	OGLShader* wallShader;
};
