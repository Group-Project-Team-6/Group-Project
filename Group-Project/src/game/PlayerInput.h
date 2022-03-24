#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"
#ifdef __ORBIS__
#include "../PlayStation4/PS4Input.h"
using namespace NCL::PS4;
#endif

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

#ifdef _WIN64

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
#elif __ORBIS__

		PS4Input input = PS4Input();
		input.Poll();

		if (input.GetButton(0)) { // Test inputs
			std::cout << "Button 0" << std::endl;
			return leftMouse;
		}


#endif
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