#pragma once

#include "../game/GameEntity.h"
#include "../common/RendererBase.h"
#include "../CSC8503/GameTechRenderer.h"
#include "../Physics/VkTechRenderer.h"
#include "../game/TransformConverter.h"
#include "ControlsCommand.h"
#include "PlayerInput.h"
#include "Player.h"
#include "Items.h"
#include "Wall.h"

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
	void InitItems();
	//Build State Machine?
	//void LevelGeneration();
	void InitCharacter();
	//void InitHUD
	//InitNetworking?

	//Tools
	TransformConverter transformConverter;

	//World
	RendererBase* renderer;//GameTechRenderer* renderer;
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
	Player* players[4];
	Item* items[36];
	Wall* walls[100]; //Exact number data driven;
	GameEntity* ground;

	//Game Assets? Temp
	MeshGeometry* sphereMesh;
	MeshGeometry* cubeMesh;
	MeshGeometry* capsuleMesh;

	TextureBase* basicTex;
	ShaderBase* basicShader;

	//Controls
	PlayerInput playerInput;
	ControlsCommand* command;
};