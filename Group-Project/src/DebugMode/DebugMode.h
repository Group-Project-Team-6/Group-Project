#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <vector>
#include <string>

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

        void InitTasks(int num) {
            tasks.start(num);
            //GetMemoryAllocationSize(tasks);
        }

        Tasks* GetTasks() { 
            return &tasks; 
        }

        void SetMemoryInfo(std::string info) {
            MemoryInfo.push_back(info);
        }

        void GetMemoryInfo() {
            for (int i = 0; i < MemoryInfo.size(); ++i) {
                std::cout << MemoryInfo.at(i) << std::endl;
            }
        }

        void SetPhysicsInfo(int t) {
            ManifoldsInfo = t;
        }

        void GetPhysicsInfo() {
            std::cout << "Number of Manifold(s): " << ManifoldsInfo << "\n" << std::endl;
        }

        void ToggleDebugMode();
        bool getDebugMode() { return isDebug; }
        void DebugUpdate();
       
    private:
        bool isDebug = false;

        size_t MemorySize;

        std::vector<std::string> MemoryInfo;
        int ManifoldsInfo;

        Tasks tasks;
};