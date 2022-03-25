#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>

#include "Tasks.h"

struct MemoryInformations {
    const char* name;
    std::string info;
    size_t size;
};



typedef  std::chrono::time_point<std::chrono::high_resolution_clock>  Timepoint;

class DebugMode {
    public:
        DebugMode(int num);
        DebugMode() {};
        ~DebugMode() { std::cout << "delete DebugMode" << std::endl; };

        //template<class T>
        //inline void GetMemoryAllocationSize(T&& t) {
        //    MemorySize = sizeof(t);
        //    const char* name = typeid(t).name();
        //    std::cout << "Memory Size for " << name << ": " << MemorySize << " Byte." << std::endl;
        //    std::cout << "Memory Location for " << name << ": " << &t << std::endl;
        //}

        static inline void GetFPS(float dt) {
            std::cout << "Average FPS: " << (1.0f / dt) << std::endl;
        }

        static inline void InitTasks(int num) {
            tasks.start(num);
        }

        static Tasks* GetTasks() { 
            return &tasks; 
        }

        static void AddMemoryInfo(MemoryInformations info) {
            memoryInformations.push_back(info);
            memQueue.push(info);
            memUsed += (float)info.size;
        }

        static float GetMemUsed() {
            return memUsed;
        }
/*
        void GetMemoryInfo() {
            for (int i = 0; i < memoryInformations.size(); ++i) {
                std::cout << memoryInformations.at(i).info << std::endl;
            }   
        }
*/
        static std::vector<MemoryInformations> GetMemoryInfo() {
            return memoryInformations;
        }

        static void RemoveMemoryInfo(const char* name) {
            std::vector<MemoryInformations>::iterator it;
            it = std::find_if(memoryInformations.begin(), memoryInformations.end(), [name](MemoryInformations& info) {
                return info.name == name;
            });
            if (it != memoryInformations.end()) {
                memUsed -= (float)it->size;
                memoryInformations.erase(it);
                std::cout << "Free Memory From " << name << "\n" << std::endl;
            }
        }

        static void SetPhysicsInfo(int t) {
            ManifoldsInfo = t;
        }

        static inline std::string GetPhysicsInfo() {
            return ("Number of Manifold(s): " + std::to_string(ManifoldsInfo) + "\n");
        }

        static void SetDebugMode(bool debugMode) { isDebug = debugMode; }
        static bool getDebugMode() { return isDebug; }

        static void UpdateDebug(float dt);
       
        static void GetStartTime() {
            start = std::chrono::high_resolution_clock::now();
        }

        static void GetEndTime() {
            end = std::chrono::high_resolution_clock::now();
        }

        static void GetRunTime() {
            std::chrono::duration<double> elapsed = (end - start) * 1000;
            std::cout << "Run Time for Main Loop: " << elapsed.count() << "ms\n" << std::endl;
        }
        static std::queue<MemoryInformations> memQueue;

    private:
        static Tasks tasks;

        static bool isDebug;
        static int ManifoldsInfo;
        static float memUsed;
        static Timepoint start, end;

        static std::vector<MemoryInformations> memoryInformations;
};

template<typename T>
const std::type_info& Ty() {
    return typeid(T);
}

inline void* operator new(size_t size, const std::type_info& t) {
    std::stringstream ss;
    void* ptr;
    ptr = malloc(size);

    if (!ptr) // if no memory is allocated, then generate an exception
    {
        std::bad_alloc ba;
        std::cout << "Memory allocation error for " << t.name() << "." << std::endl;
        throw ba;
    }
    else {
        ss << "Memory Size for " << t.name() << ": " << size << " Byte.\nMemory Location for " << t.name() << ": " << &size << "\n";
        MemoryInformations Info;
        Info.name = t.name();
        Info.info = ss.str();
        Info.size = size;
        DebugMode::AddMemoryInfo(Info);
        return ptr;
    }
}

inline void operator delete(void* p)
{
    if (p) {
        DebugMode::RemoveMemoryInfo(typeid(p).name());
        free(p);
        p = nullptr;
    }
}