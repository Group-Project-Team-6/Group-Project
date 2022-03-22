#pragma once

#include "GameEntity.h"
#include "AssetsManager.h"
//#include "../CSC8503/GameTechRenderer.h"
//#include "../common/TextureLoader.h"

class Wall : public GameEntity {
public:
	Wall(Transform buildTransform);
	~Wall();

	void InitAssets(); //Temp

	/*virtual btRigidBody* GetRigidBody() const override {
		return wallRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		wallRigidBody = newRigidBody;
	}

	virtual void UpdateRenderPositions() override {

		return;
	}*/

private:
	//TransformConverter transformConverter;
	//Transform transform;
	//btTransform bttransform;

	btDefaultMotionState* wallMotion;
	btCollisionShape* wallShape;
	//btRigidBody* wallRigidBody;
	btCollisionObject* wallObject;

	//graphics
	MeshPtr wallMesh = nullptr;
	TexturePtr wallTex = nullptr;
	ShaderPtr wallShader = nullptr;
};
