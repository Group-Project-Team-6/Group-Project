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
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, 100 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * -1000);
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, -100 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * 1000);
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -1000);
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) override {
		//player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
		player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * 1000);
	}
};

class MouseHorizontal : ControlsCommand {
public:
	virtual void execute(Player& player, GameWorld& world, btDiscreteDynamicsWorld& physicsworld) override {
		player.GetRigidBody()->setAngularVelocity({ 0, -(Window::GetMouse()->GetRelativePosition().x) , 0 });		
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