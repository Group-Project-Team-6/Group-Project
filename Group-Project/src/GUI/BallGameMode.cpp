#include "BallGameMode.h"
#include "../GUI/GameUI.h"
#include "../Physics/PhysicsTestScene.h"

using namespace NCL;
using namespace CSC8503;

BallGameMode::BallGameMode(PhysicsTestScene* game)
	: GameMode(game)
{
	menu.reset(new BallLevelMenu());
	gameInst->gameUI->PushMenu(menu);
}

BallGameMode::~BallGameMode()
{
	gameInst->gameUI->RemoveMenu(menu);
}

void BallGameMode::SetupScene()
{
	//gameInst->camInitPos = Vector3(0, 100, 200);
	//gameInst->camInitRot = Vector3(0, 0, -30);

	////Add floor
	////GameObject* floor = gameInst->AddFloorToWorld(Vector3(0, -2, 0), Vector3(100,2,100), Vector4(1,1,1,0), "DefaultFloor");
	////floor->GetPhysicsObject()->SetFriction(0.6);
	//GameObject* floor_01 = gameInst->AddFloorToWorld(Vector3(80, -2, 0), Vector3(20, 2, 100), Vector4(1, 1, 1, 0), "Floor_01");
	//floor_01->GetPhysicsObject()->SetFriction(0.6);
	//floor_01->GetPhysicsObject()->SetElasticity(0.6);



}

void BallGameMode::Update(float dt)
{
	/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::E))
	{
		if (gameInst->selectionObject)
		{
			ShootableObject* sObj = dynamic_cast<ShootableObject*>(gameInst->selectionObject);
			if (sObj)
			{
				sObj->TriggerShoot(gameInst->forceMagnitude);
			}
		}
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R))
	{
		mainBall->RestorePosition();
	}*/

	menu->Update(dt);
}
