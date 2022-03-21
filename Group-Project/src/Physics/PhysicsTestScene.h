#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "../CSC8503/GameTechRenderer.h"
//#include "../Physics/btNClmotionState.h"
#include "../game/GameEntity.h"
#include "../Physics/btNClmotionState.h"

#include "../Audio/AudioManager.h"

#include "../game/DebugMode.h"
#include "../GUI/PushdownMachine.h"
#include "../GUI/MainMenu.h"
#include "../GUI/SettingMenu.h"

class GameUI;

class PhysicsTestScene {
	public:
		PhysicsTestScene();

		~PhysicsTestScene();

		void UpdateGame(float dt);

		virtual void UpdateRender(float dt);

		void GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d);

		GameUI* GetUI() const { return gameUI; };
		bool IsFreezed() const { return freezed; };
		void SetFreeze(bool freeze) { freezed = freeze; };

		void InitUI();
		void SetMainLevel();
		void SetSettingLevel();

		GameUI* gameUI;

	private:
		void InitAssets();
		void InitScene();
		void InitCamera();
		void UpdateKeys(float dt);

		int maxProxies;
		btVector3 worldAabbMin;
		btVector3 worldAabbMax;
		btAxisSweep3* broadphase;

		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;

		btDefaultMotionState* sphereMotion;

		GameEntity* sphere;
		GameEntity* ground;

		OGLMesh* sphereMesh = nullptr;
		OGLMesh* cubeMesh = nullptr;

		OGLTexture* basicTex = nullptr;
		OGLShader* basicShader = nullptr;

		GameTechRenderer* renderer;
		GameWorld* world;
		AudioManager* audioManager;

		

	protected:

		class GameMode;
		std::unique_ptr<GameMode> gameMode = nullptr;
		class BallGameMode;
		friend class MainMenu;
		std::shared_ptr<MainMenu> gameMenu = nullptr;

		friend class SettingMenu;
		std::shared_ptr<SettingMenu> SMenu = nullptr;

		

		bool quit;
		bool freezed;
		PushdownMachine* pauseMachine;
};

