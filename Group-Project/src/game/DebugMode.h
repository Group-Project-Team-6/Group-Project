#pragma once

#include <iostream>
#include <string>
#include <memory>

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
            //if (frames > 60) {
            //    std::cout << "Average FPS: " << (1.0f / dt) << std::endl;
            //    frames = 0;
            //}
            //frames++;

            std::cout << "Average FPS: " << (1.0f / dt) << std::endl;
        }

    private:
        size_t MemorySize;
        int frames = 0;
};