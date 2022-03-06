#pragma once
#include "ControlsCommand.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		//Needs on Ground Bool
		player.GetRigidBody()->applyCentralImpulse({ 0, 1000, 0});
	}
};

class moveForwardCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		player.GetRigidBody()->applyCentralImpulse({ 0, 0, 100 });
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		player.GetRigidBody()->applyCentralImpulse({ 0, 0, -100 });
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
	}
};

class leftMouse : ControlsCommand {
public:
	virtual void execute(GameEntity& player) override {
		//Shoot
	}
};