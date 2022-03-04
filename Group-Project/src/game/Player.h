#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../CSC8503/GameTechRenderer.h"

class Player : public GameEntity {
public:
	Player();
	~Player();

	void AddPlayer(const Vector3& position, string name = "");

	btRigidBody* GetRigidBody() const {
		return rigidBody;
	}

	void SetRigidBody(btRigidBody* newRigidBody) {
		rigidBody = newRigidBody;
	}

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
	int playerFriction;
	btVector3 playerInertia;

	btDefaultMotionState* playerMotion;
	btCollisionShape* playerShape;
	btGeneric6DofConstraint* playerConstraint;
	btRigidBody* playerRigidBody;
};
