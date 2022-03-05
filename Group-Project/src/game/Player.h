#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../CSC8503/GameTechRenderer.h"

class Player : public GameEntity {
public:
	Player(Vector3 position, string name);
	~Player();

	virtual btRigidBody* GetRigidBody() const override{
		return playerRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		playerRigidBody = newRigidBody;
	}

	btGeneric6DofConstraint* GetPlayerConstraints() const {
		return playerConstraint;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = playerRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}

	void IntitAssets(); //Temp

protected:
	//Temp
	OGLMesh* playerMesh;
	OGLTexture* playerTex;
	OGLShader* playerShader;

	//general
	string name;
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;

	//player Physics
	int playerMass;
	float playerFriction;
	float playerRestitution;
	btVector3 playerInertia;

	btDefaultMotionState* playerMotion;
	btCollisionShape* playerShape;
	btGeneric6DofConstraint* playerConstraint;
	btRigidBody* playerRigidBody;
};
