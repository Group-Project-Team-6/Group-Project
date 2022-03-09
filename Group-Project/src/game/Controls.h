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
		player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
	}
};

class MouseHorizontal : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsworld) override {
		
		float yaw = (Window::GetMouse()->GetRelativePosition().x);	
		//player.GetTransform().SetOrientation(player.GetTransform().GetOrientation().EulerAnglesToQuaternion(0, yaw, 0));
		player.GetRigidBody()->applyTorque({ 0, 100, 0 });
	}
};


class leftMouseCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//Shoot with freelist
		//player.Shoot();
		Bullet* bullet = new Bullet();
		bullet->Init(player.GetShootingPosition(), 5);
		world.AddGameObject(bullet);
		physicsWorld.addRigidBody(bullet->GetRigidBody());
		float yaw = (player.GetShootingPosition().GetOrientation().x) * 100; //Needs to be euler
		float pitch = (player.GetShootingPosition().GetOrientation().y) * 100;
		float roll = (player.GetShootingPosition().GetOrientation().z) * 100;
		bullet->GetRigidBody()->applyCentralImpulse({ yaw, pitch, roll });
		//no memory deallocation
		//memory fragmentation
		// 
		//player.GetBulletPool().Create(player.GetShootingPosition(), 5);
		//world.AddGameObject();
		//physicsWorld.addRigidBody();
	}
};