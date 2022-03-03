#pragma once

//#include "btBulletCollisionCommon.h"
//#include "btBulletDynamicsCommon.h"
#include "../game/GameEntity.h"
#include "../CSC8503/GameTechRenderer.h"
#include "../game/TransformConverter.h"
#include "ControlsCommand.h"
#include "PlayerInput.h"

//Encapsulate in namespace?

class Game {

public:
	Game();
	~Game();

	void UpdateGame(float dt);

protected:
	void InitWorld();
	void InitPhysics();
	//void InitAudio();
	void InitAssets();
	void InitScene();
	//Build State Machine?
	//void LevelGeneration();
	void InitCharacter();
	//void InitHUD
	//InitNetworking?

	//Tools
	TransformConverter transformConverter;

	//World
	GameTechRenderer* renderer;
	GameWorld* world;

	//Physics
	int maxProxies;
	btVector3 worldAabbMin;
	btVector3 worldAabbMax;
	btAxisSweep3* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	//Custom motion state?

	//GameEntities
	GameEntity* character;
	GameEntity* ground;

	//Game Assets? Temp
	OGLMesh* sphereMesh;
	OGLMesh* cubeMesh;
	OGLMesh* capsuleMesh;

	OGLTexture* basicTex;
	OGLShader* basicShader;

	//Controls
	PlayerInput playerInput;
	ControlsCommand* command;
};