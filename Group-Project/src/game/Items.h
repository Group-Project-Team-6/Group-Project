#pragma once

#include "GameEntity.h"
#include "../common/RendererBase.h"
#include "../common/TextureLoader.h"

class Item : public GameEntity {
public:
	Item(Vector3 position, int score, RendererBase& r);
	~Item();
	void InitAssets(RendererBase& r); //Temp
	void OnPlayerCollide();

	virtual btRigidBody* GetRigidBody() const override {
		return itemRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		itemRigidBody = newRigidBody;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = itemRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}

private:
	int score;
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;
	int itemScore;

	btDefaultMotionState* itemMotion;
	btCollisionShape* itemShape;
	btRigidBody* itemRigidBody;

	//Anim

	//graphics
	MeshGeometry* itemMesh;
	TextureBase* itemTex;
	ShaderBase* itemShader;
};
