#pragma once
#include "../game/Player.h"
#include "../CSC8503/GameWorld.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class ControlsCommand {
public:
	virtual ~ControlsCommand() {}

	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) = 0;
};