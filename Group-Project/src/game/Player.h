#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../CSC8503/GameTechRenderer.h"

class Player : GameEntity {
public:
	Player();
	~Player();

	void AddPlayer(const Vector3& position, string name = "");
protected:
	//Temp
	OGLMesh* playerMesh;
	OGLTexture* playerTex;
	OGLShader* playerShader;

	//general
	string name;
	TransformConverter transformConverter;

	//player Physics
	btDefaultMotionState* playerMotion;
	btCollisionShape* playerShape;
	int playerMass;
	btVector3 playerInertia;
	btGeneric6DofConstraint* playerConstraint;

};
