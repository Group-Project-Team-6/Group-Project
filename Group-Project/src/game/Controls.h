#pragma once
#include "ControlsCommand.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//Needs on Ground Bool
		player.GetRigidBody()->applyCentralImpulse({ 0, 1000, 0});
	}
};

class moveForwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		player.GetRigidBody()->applyCentralImpulse({ 0, 0, 100 });
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		player.GetRigidBody()->applyCentralImpulse({ 0, 0, -100 });
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
	}
};

class leftMouseCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//Shoot with freelist
		player.GetBulletPool().Create(player.GetShootingPosition(), 5);
		//world.AddGameObject();
		//physicsWorld.addRigidBody();
	}
};