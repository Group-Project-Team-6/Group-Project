#include "../common/Window.h"
#include "../Physics/PhysicsTestScene.h"
#include "../Physics/VkTechRenderer.h"

#include <iostream>

using namespace NCL;
//using namespace CSC8503;

int main() {

	Window* w = Window::CreateGameWindow("Physics Test Scene", 1920, 1080, false);

	if (!w->HasInitialised()) {
		return -1;
	}
	srand(time(0));
	w->ShowOSPointer(true);
	w->LockMouseToWindow(false);

	VkTechRenderer* renderer = new VkTechRenderer();

	PhysicsTestScene* g = new PhysicsTestScene(renderer);
	w->GetTimer()->GetTimeDeltaSeconds();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}
		g->UpdateGame(dt);
	}

	Window::DestroyGameWindow();

	return 0;
}