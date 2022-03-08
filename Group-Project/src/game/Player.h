#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../CSC8503/GameTechRenderer.h"
#include "BulletPool.h"

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

	virtual Transform& GetTransform() override {
		return transform;
	}

	virtual void SetTransform(Transform newtransform) override {
		transform = newtransform;
	}

	BulletPool GetBulletPool() const {
		return bullets;
	}

	Transform GetShootingPosition() const {
		return *shootingPos;
	}

	Transform GetCameraPosition() const {
		return *cameraPos;
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

	/*void Shoot() {
		Bullet* bullet = new Bullet();
		bullet->Init(*shootingPos, 5);
	}*/

	void InitAssets(); //Temp

protected:
	//Temp
	OGLMesh* playerMesh;
	OGLTexture* playerTex;
	OGLShader* playerShader;

	//general
	string name;
	TransformConverter transformConverter;
	Transform transform;
	Transform* shootingPos;
	Transform* cameraPos;
	btTransform bttransform;
	BulletPool bullets;

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
