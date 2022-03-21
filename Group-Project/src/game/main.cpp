#include "../common/Window.h"
#include "Game.h"
//#include "../Physics/VkTechRenderer.h"
#include "../common/Assets.h"
#include "DebugMode.h"

#include <iostream>
#include <memory>

void* operator new(size_t size, const char* name) {
    void* ptr;
    ptr = malloc(size);

    if (!ptr) // if no memory is allocated, then generate an exception
    {
        std::bad_alloc ba;
        std::cout<<"Memory allocation error." << std::endl;
        throw ba;
    }
    else {
        std::cout<<"Memory is allocated successfully!" << std::endl;
        std::cout << "Memory Size for " << name << ": " << size << " Byte." << std::endl;
        std::cout << "Memory Location for " << name << ": " << &size << std::endl;

        return ptr;
    }
    
}

void operator delete(void* ptr) {
    const char* name = typeid(ptr).name();
    //std::cout << "Free Memory From " << name << std::endl;
    free(ptr);
}

using namespace NCL;
//using namespace CSC8503;

int main() {
	Assets::FetchDirConfig("dir.txt");

	Window* w = Window::CreateGameWindow("Physics Test Scene", 1920, 1080, false);
	std::shared_ptr<DebugMode> d(new(typeid(DebugMode).name()) DebugMode());

	//DebugMode* d = new (typeid(DebugMode).name()) DebugMode();
	
	if (!w->HasInitialised()) {
		return -1;
	}
	srand(time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
	//VkTechRenderer* renderer = new VkTechRenderer();
	//PhysicsTestScene* g = new PhysicsTestScene(renderer);

	Game* g = new Game();
	//std::shared_ptr<Game> g(Debug_NEW(Game));

	w->GetTimer()->GetTimeDeltaSeconds();
	bool toggleDebug = false;
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

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
			toggleDebug = !toggleDebug;
		}
		if (toggleDebug) {
			//std::cout << "\x1B[2J\x1B[H";
			d->GetMemoryAllocationSize(*w);
			d->GetMemoryAllocationSize(*d);
			d->GetMemoryAllocationSize(*g);
			g->GetPhysicsTestSceneDebugData(d);
			d->GetFPS(dt);
		}
		g->UpdateGame(dt);
	}

	Window::DestroyGameWindow();

	return 0;
}