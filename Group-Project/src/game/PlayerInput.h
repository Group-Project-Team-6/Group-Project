#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"
#include <queue>

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

	std::queue<ControlsCommand*>& handleInput() {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			ControlQueue.push(spaceBar);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::W)) {
			ControlQueue.push(wKey);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::S)) {
			ControlQueue.push(sKey);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::A)) {
			ControlQueue.push(aKey);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::D)) {
			ControlQueue.push(dKey);
		}
		if (Window::GetMouse()->ButtonDown(MouseButtons::LEFT)) {
			ControlQueue.push(leftMouse);
		}
		if (Window::GetMouse()->GetRelativePosition().x) {
			ControlQueue.push(mouseHorizontal);
		}


			
		return ControlQueue;
	}

private:
	ControlsCommand* spaceBar = (ControlsCommand*) new JumpCommand();
	ControlsCommand* wKey = (ControlsCommand*) new moveForwardCommand();
	ControlsCommand* sKey = (ControlsCommand*) new moveBackwardCommand();
	ControlsCommand* aKey = (ControlsCommand*) new moveLeftCommand();
	ControlsCommand* dKey = (ControlsCommand*) new moveRightCommand();
	ControlsCommand* leftMouse = (ControlsCommand*) new leftMouseCommand();
	ControlsCommand* mouseHorizontal = (ControlsCommand*) new MouseHorizontal();
	std::queue<ControlsCommand*> ControlQueue;
};