#pragma once
#include "ControlsCommand.h"
#include "../Audio/AudioManager.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//Needs on Ground Bool
		player.GetRigidBody()->applyCentralImpulse({ 0, 1000, 0});
		audioManager.SetJump();
	}
};

class moveForwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, 100 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * -1000);
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, -100 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * 1000);
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -1000);
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * 1000);
	}
};

class MouseHorizontal : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsworld, AudioManager& audioManager) override {
		player.GetRigidBody()->setAngularVelocity({ 0, -(Window::GetMouse()->GetRelativePosition().x) , 0 });		
	}	
};


class leftMouseCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, AudioManager& audioManager) override {
		//Shoot with freelist
		Vector3 shootPos({ 0, 0, player.GetTransform().GetPosition().z }); //Local Position;
		btVector3 force(shootPos.x, shootPos.y, shootPos.z);
		player.GetBulletPool()->Create(*player.GetRigidBody(), force, 1.0f, world, physicsWorld);

		//std::cout << &world << std::endl;
		//std::cout << &physicsWorld << std::endl;

	}
};