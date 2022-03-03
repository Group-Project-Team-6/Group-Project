#pragma once
#include "../game/GameEntity.h"

class ControlsCommand {
public:
	virtual ~ControlsCommand() {}

	virtual void execute(GameEntity& player) = 0;
};