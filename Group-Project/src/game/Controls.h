#pragma once
#include "ControlsCommand.h"
#include "../Audio/AudioManager.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//Needs on Ground Bool
		player.GetRigidBody()->applyCentralImpulse({ 0, 1000, 0});
		audioManager.SetJump();
	}
};

class moveForwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//while (player.GetRigidBody()->getLinearVelocity().x() > -20) {
			player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * -100);
		//}
		
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//while (player.GetRigidBody()->getLinearVelocity().x() > 20) {
			player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * 100);
		//}
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//while (player.GetRigidBody()->getLinearVelocity().z() > 20) {
			player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -100);
		//}
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//while (player.GetRigidBody()->getLinearVelocity().z() > 20) {
			player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * 100);
		//}
	}
};

class MouseHorizontal : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		player.GetRigidBody()->setAngularVelocity({ 0, -(Window::GetMouse()->GetRelativePosition().x) , 0 });		
	}	
};


class leftMouseCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		Vector3 shootPos({ 0, 0, player.GetTransform().GetPosition().z }); //Local Position;
		btVector3 force(shootPos.x, shootPos.y, shootPos.z);
		player.GetBulletPool()->Create(*player.GetRigidBody(), force, 1.0f, camera);
	}
};