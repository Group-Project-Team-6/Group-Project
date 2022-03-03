#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"

class PlayerInput {
public:
	ControlsCommand* handleInput() {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			return spaceBar;
		}



		return NULL;
	}

private:
	ControlsCommand* spaceBar;
};