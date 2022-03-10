#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"

class PlayerInput {
public:
	//will need deconstructor
	//Check Memory Leaks!
	~PlayerInput() {
		delete spaceBar;
		delete wKey;
		delete sKey;
		delete aKey;
		delete dKey;
	}

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
		if (Window::GetMouse()->ButtonDown(MouseButtons::LEFT)) {
			return leftMouse;
		}
		if (Window::GetMouse()->GetRelativePosition().x) {
			return mouseHorizontal;
		}
			
		return NULL;
	}

private:
	ControlsCommand* spaceBar = (ControlsCommand*) new JumpCommand();
	ControlsCommand* wKey = (ControlsCommand*) new moveForwardCommand();
	ControlsCommand* sKey = (ControlsCommand*) new moveBackwardCommand();
	ControlsCommand* aKey = (ControlsCommand*) new moveLeftCommand();
	ControlsCommand* dKey = (ControlsCommand*) new moveRightCommand();
	ControlsCommand* leftMouse = (ControlsCommand*) new leftMouseCommand();
	ControlsCommand* mouseHorizontal = (ControlsCommand*) new MouseHorizontal();

};