#pragma once

#include "../game/GameEntity.h"
#include "../common/RendererBase.h"
#include "../CSC8503/GameTechRenderer.h"
#include "../CSC8503/GameLoadingRenderer.h"
#include "../Physics/VkTechRenderer.h"
#include "../game/TransformConverter.h"
#include "ControlsCommand.h"
#include "PlayerInput.h"
#include "Player.h"
#include "Items.h"
#include "Wall.h"
#include "DebugMode.h"
#include "../Audio/AudioManager.h"
#include "AssetsManager.h"
#include <atomic>


//Encapsulate in namespace?

class Game {

public:
	Game();
	~Game();

	void UpdateGame(float dt);
	void GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d);

protected:
	void InitWorld();
	void RenderLoading();
	void Init();
	void InitPhysics();
	void InitAudio();
	void InitAssets();
	void InitScene();
	void InitItems();
	//Build State Machine?
	void LevelGeneration();
	void InitCharacter();
	
	//void InitHUD
	//InitNetworking?

	//Tools
	TransformConverter transformConverter;

	//World
	RendererPtr renderer = nullptr;//GameTechRenderer* renderer;
	GameWorld* world;

	//Audio
	AudioManager* audioManager;

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
	MeshPtr sphereMesh = nullptr;
	MeshPtr cubeMesh = nullptr;
	MeshPtr capsuleMesh = nullptr;

	TexturePtr basicTex = nullptr;
	ShaderPtr basicShader = nullptr;

	//Controls
	PlayerInput playerInput[4];
	std::atomic<bool> loading;
};