#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "Tasks.h"

/*
namespace Debug{
    #define Debug_UNUSED(_VAR)             ((void)(_VAR))

    typedef void*   (*DebugMemAllocFunc)(size_t sz, void* user_data);               // Function signature for ImGui::SetAllocatorFunctions()
    typedef void    (*DebugMemFreeFunc)(void* ptr, void* user_data);                // Function signature for ImGui::SetAllocatorFunctions()

    void          SetAllocatorFunctions(DebugMemAllocFunc alloc_func, DebugMemFreeFunc free_func, void* user_data = NULL);
    void          GetAllocatorFunctions(DebugMemAllocFunc* p_alloc_func, DebugMemFreeFunc* p_free_func, void** p_user_data);
    void*         MemAlloc(size_t size);
    void          MemFree(void* ptr);

    struct DebugNewWrapper {
        template<class T>
        inline void GetMemoryAllocationSize(T&& t) {
            size_t MemorySize = sizeof(t);
            const char* name = typeid(t).name();
            std::cout << "Memory Size for " << name << ": " << MemorySize << " Byte." << std::endl;
            std::cout << "Memory Location for " << name << ": " << &t << std::endl;
        }
    };
    
    #define Debug_ALLOC(_SIZE)                     MemAlloc(_SIZE)
    #define Debug_FREE(_PTR)                       MemFree(_PTR)
    #define Debug_PLACEMENT_NEW(_PTR)              new(Debug::DebugNewWrapper(), _PTR)
    #define Debug_NEW(_TYPE)                       new(Debug::DebugNewWrapper(), Debug::MemAlloc(sizeof(_TYPE))) _TYPE
    template<typename T> void Debug_DELETE(T* p)   { if (p) { p->~T(); Debug::MemFree(p); } }
}

inline void* operator new(size_t, Debug::DebugNewWrapper d, void* ptr) {
        d.GetMemoryAllocationSize(ptr);
        return ptr;
    }
inline void  operator delete(void*, Debug::DebugNewWrapper, void*)   {} // This is only required so we can use the symmetrical new()

*/


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

        void* operator new(size_t size) {
            void* ptr = malloc(size);

            return ptr;
        }
       
    private:
        size_t MemorySize;
        int frames = 0;
        bool isDebug = false;

        Tasks tasks;

        
};