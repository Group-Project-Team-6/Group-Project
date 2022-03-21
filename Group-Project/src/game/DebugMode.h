#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>

#include "Tasks.h"

class DebugMode {
    public:
        template<class T>
        inline void GetMemoryAllocationSize(T&& t) {
            MemorySize = sizeof(t);
            const char* name = typeid(t).name();
            std::cout << "Memory Size for " << name << ": " << MemorySize << " Byte." << std::endl;
            std::cout << "Memory Location for " << name << ": " << &t << std::endl;
        }

        inline void GetFPS(float dt) {
            std::cout << "Average FPS: " << (1.0f / dt) << std::endl;
        }

        inline void InitTasks() {
            tasks.start(4);
        }

        void ToggleDebugMode();
        bool getDebugMode() { return isDebug; }
        void DebugUpdate();
       
    private:
        size_t MemorySize;
        int frames = 0;
        bool isDebug = false;

        Tasks tasks;
};