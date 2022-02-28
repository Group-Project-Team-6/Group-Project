#pragma once
#include "../game/GameEntity.h"

class PlayerInput {
public:
	virtual ~PlayerInput();

	virtual void execute(GameEntity& player) = 0;
};