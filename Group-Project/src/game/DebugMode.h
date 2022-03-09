#pragma once

#include <iostream>
#include <string>
#include <memory>

class DebugMode {
    public:
        template<class T>
        void GetMemoryAllocationSize(T&& t) {
            MemorySize = sizeof(t);
            const char* name = typeid(t).name();
            std::cout << "Memory Size for " << name << ": " << MemorySize << " Byte." << std::endl;
            std::cout << "Memory Location for " << name << ": " << &t << std::endl;
        }

    private:
        size_t MemorySize;
};