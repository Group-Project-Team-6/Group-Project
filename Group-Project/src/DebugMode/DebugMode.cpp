#include "DebugMode.h"

DebugMode::DebugMode(int num) {
    InitTasks(num);
}

void DebugMode::ToggleDebugMode() {
    isDebug = !isDebug;
}

void DebugMode::UpdateDebug(float dt) {
    if (!isDebug) {
        return;
    }
    GetMemoryInfo();
	GetPhysicsInfo();
	GetFPS(dt);
    GetRunTime();
	std::cout << std::endl;
}