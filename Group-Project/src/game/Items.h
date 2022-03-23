#pragma once

#include "GameEntity.h"
#include "../common/RendererBase.h"
#include "../common/TextureLoader.h"

class Item : public GameEntity {
public:
	Item(Vector3 position, int score);
	~Item();
	void InitAssets(); //Temp
	void OnPlayerCollide();

	virtual btRigidBody* GetRigidBody() const override {
		return itemRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		itemRigidBody = newRigidBody;
	}

	virtual void UpdateRenderPositions() override {

		return;

	}

private:
	int score;
	TransformConverter transformConverter;
	btTransform bttransform;
	int itemScore;


	btDefaultMotionState* itemMotion = nullptr;
	btCollisionShape* itemShape = nullptr;
	btRigidBody* itemRigidBody = nullptr;

	//Anim

	//graphics
	MeshPtr itemMesh = nullptr;
	TexturePtr itemTex = nullptr;
	ShaderPtr itemShader = nullptr;
};
