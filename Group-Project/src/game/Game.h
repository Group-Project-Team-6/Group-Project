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
#include "DebugMode.h"
#include "../Audio/AudioManager.h"
#include "AssetsManager.h"

#ifdef __ORBIS__

#include "../Bullet/include/bullet/LinearMath/btVector3.h"
#include "../Bullet/include/bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "../Bullet/include/bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "../Bullet/include/bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "../Bullet/include/bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "../PlayStation4/PS4RendererBase.h"
#include "../PlayStation4/PS4Shader.h"
#include "../PlayStation4/PS4Mesh.h"
#include "../PlayStation4/PS4Texture.h"
using namespace NCL::PS4;
using namespace NCL;
using namespace NCL::CSC8503;

#endif

//Encapsulate in namespace?

class Game {

public:
	Game();
	~Game();

	void UpdateGame(float dt);
	void GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d);

protected:
	void InitWorld();
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
#ifdef _WIN64
	RendererPtr renderer = nullptr;//GameTechRenderer* renderer;
#elif __ORBIS__

#endif
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
#ifdef _WIN64

	MeshPtr sphereMesh = nullptr;
	MeshPtr cubeMesh = nullptr;
	MeshPtr capsuleMesh = nullptr;

	TexturePtr basicTex = nullptr;
	ShaderPtr basicShader = nullptr;

#elif __ORBIS__

	PS4Mesh* sphereMesh;
	PS4Mesh* cubeMesh;
	PS4Mesh* capsuleMesh;

	PS4Texture* basicTex;
	PS4Shader* basicShader;


#endif

	//Controls
	PlayerInput playerInput;
	ControlsCommand* command;
};