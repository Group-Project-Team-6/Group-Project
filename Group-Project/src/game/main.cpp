#include "../common/Window.h"
#include "Game.h"
//#include "../Physics/VkTechRenderer.h"
#include "../common/Assets.h"
#include "../DebugMode/DebugMode.h"

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <thread>

void* operator new(size_t size, const char* name, std::string& Info) {
	std::stringstream ss;
    void* ptr;
    ptr = malloc(size);

    if (!ptr) // if no memory is allocated, then generate an exception
    {
        std::bad_alloc ba;
        std::cout<<"Memory allocation error for " << name << "." << std::endl;
        throw ba;
    }
    else {
		ss << "Memory Size for " << name << ": " << size << " Byte.\nMemory Location for " << name << ": " << &size << "\n";
		Info = ss.str();

        return ptr;
    }
}

using namespace NCL;

int main() {
	std::string info;
	Assets::FetchDirConfig("dir.txt");
	Window* w = Window::CreateGameWindow("Physics Test Scene", 1920, 1080, false);
	if (!w->HasInitialised()) {
		return -1;
	}

	std::shared_ptr<DebugMode> d(new(typeid(DebugMode).name(), info) DebugMode());
	d->SetMemoryInfo(info);

	Tasks* tasks = d->GetTasks();
	d->InitTasks(2);

	std::shared_ptr<Game> g(new(typeid(Game).name(), info) Game(tasks));
	d->SetMemoryInfo(info);
	
	srand(time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);
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

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
			d->ToggleDebugMode();
		}
		if (d->getDebugMode()) {
			tasks->queue(
				[d, w, g, dt]
				{
					std::cout << "Current Thread ID: " << std::this_thread::get_id() << "\n" << std::endl; //For tracking current thread
					//std::cout << "\x1B[2J\x1B[H";
					//d->GetMemoryAllocationSize(*w);
					//d->GetMemoryAllocationSize(*d);
					//d->GetMemoryAllocationSize(*g);
					//g->GetPhysicsTestSceneDebugData(d);
					d->GetMemoryInfo();
					d->GetPhysicsInfo();
					d->GetFPS(dt);
					std::cout << std::endl;
				}
			);
			tasks->waitFinished();
		}
		g->UpdateGame(dt, d);
	}
	
	Window::DestroyGameWindow();

	return 0;
}