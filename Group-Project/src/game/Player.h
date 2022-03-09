#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../common/RendererBase.h"

class Player : public GameEntity {
public:
	Player(Vector3 position, string name, RendererBase& r);
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
		//transform.UpdateMatrix();

	}

	void IntitAssets(RendererBase& r); //Temp

protected:
	//Temp
	MeshGeometry* playerMesh;
	TextureBase* playerTex;
	ShaderBase* playerShader;

	//general
	string name;
	TransformConverter transformConverter;
	//Transform transform;
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
