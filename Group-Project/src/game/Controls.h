#pragma once
#include "ControlsCommand.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(GameEntity& player) {
		player.Jump();
	}
};