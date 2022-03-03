#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"

class PlayerInput {
public:
	//will need deconstructor
	//Check Memory Leaks!


	ControlsCommand* handleInput() {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			return spaceBar;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::W)) {
			return wKey;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::S)) {
			return sKey;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::A)) {
			return aKey;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::D)) {
			return dKey;
		}

		return NULL;
	}

private:
	ControlsCommand* spaceBar = (ControlsCommand*) new JumpCommand();
	ControlsCommand* wKey = (ControlsCommand*) new moveForwardCommand();
	ControlsCommand* sKey = (ControlsCommand*) new moveBackwardCommand();
	ControlsCommand* aKey = (ControlsCommand*) new moveLeftCommand();
	ControlsCommand* dKey = (ControlsCommand*) new moveRightCommand();
};