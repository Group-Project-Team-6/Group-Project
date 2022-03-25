#include "../common/Window.h"
#include "Game.h"
//#include "../Physics/VkTechRenderer.h"
#include "../common/Assets.h"
#include "../DebugMode/DebugMode.h"
#include "../DebugMode/Tasks.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

//void* operator new(size_t size, const char* name,int i) {
//	std::stringstream ss;
//    void* ptr;
//    ptr = malloc(size);
//
//    if (!ptr) // if no memory is allocated, then generate an exception
//    {
//        std::bad_alloc ba;
//        std::cout<<"Memory allocation error for " << name << "." << std::endl;
//        throw ba;
//    }
//    else {
//		ss << "Memory Size for " << name << ": " << size << " Byte.\nMemory Location for " << name << ": " << &size << "\n";
//		MemoryInformations Info;
//		Info.name = name;
//		Info.info = ss.str();
//		Info.size = size;
//		DebugMode::AddMemoryInfo(Info);
//        return ptr;
//    }
//}


//void operator delete(void* ptr, const char* name, MemoryInformations& Info) {
//	//Info.name = name;
//	std::cout << "Now deleting " << name << std::endl;
//	free(ptr);
//}
//
//void operator delete(void* p)
//{
//	if (p) {
//		free(p);
//		p = nullptr;
//	}
//}

using namespace NCL;

int main() {
	Assets::FetchDirConfig("dir.txt");
	Window* w = Window::CreateGameWindow("Physics Test Scene", 1920, 1080, false);
	if (!w->HasInitialised()) {
		return -1;
	}

	//std::shared_ptr<DebugMode> d(new(typeid(DebugMode).name(), info) DebugMode());
	DebugMode* d = new(Ty<DebugMode>()) DebugMode(4);

	Tasks* tasks = d->GetTasks();

	std::shared_ptr<Game> g(new(Ty<Game>()) Game(tasks, *d));
	
	srand(time(0));
	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);

	w->GetTimer()->GetTimeDeltaSeconds();
	bool toggleDebug = false;
	while (w->UpdateWindow() && !g->End()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		d->GetStartTime();
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

		//			std::cout << "\x1B[2J\x1B[H";
		//			d->GetMemoryAllocationSize(*w);
		//			d->GetMemoryAllocationSize(*d);
		//			d->GetMemoryAllocationSize(*g);
		//			g->GetPhysicsTestSceneDebugData(d);

		//tasks->queue(
		//	[g, d, dt]
		//	{
		//		g->UpdateGame(dt, d);  //Memory access violation
		//	}
		//);

		g->Update(dt);
		//d->GetEndTime();
		//tasks->queue(
		//	[d, dt]
		//	{
		//		d->UpdateDebug(dt);
		//	}
		//);	
		//tasks->waitFinished();
	}
	
	Window::DestroyGameWindow();

	d->RemoveMemoryInfo(typeid(Game).name());
	d->GetMemoryInfo();
	
	delete d;

	return 0;
}