#pragma once

#include "GameEntity.h"
//#include "../CSC8503/GameTechRenderer.h"
//#include "../common/TextureLoader.h"

class Wall : public GameEntity {
public:
	Wall(Transform buildTransform, RendererBase&  r);
	~Wall();

	void InitAssets(RendererBase& b); //Temp

	virtual btRigidBody* GetRigidBody() const override {
		return wallRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		wallRigidBody = newRigidBody;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = wallRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);
		renderObject->GetTransform()->SetOrientation(nclRot);
		renderObject->GetTransform()->SetPosition(nclPos);
		renderObject->GetTransform()->SetScale(transform.GetScale());
		renderObject->GetTransform()->UpdateMatrix();

	}
private:
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;

	btDefaultMotionState* wallMotion;
	btCollisionShape* wallShape;
	btRigidBody* wallRigidBody;

	//graphics
	MeshGeometry* wallMesh;
	TextureBase* wallTex;
	ShaderBase* wallShader;
};
