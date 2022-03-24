#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

#include "Tasks.h"

struct MemoryInformations {
    const char* name;
    std::string info;
};

typedef  std::chrono::time_point<std::chrono::high_resolution_clock>  Timepoint;

class DebugMode {
    public:
        DebugMode(int num);
        ~DebugMode() { std::cout << "delete DebugMode" << std::endl; };

        //template<class T>
        //inline void GetMemoryAllocationSize(T&& t) {
        //    MemorySize = sizeof(t);
        //    const char* name = typeid(t).name();
        //    std::cout << "Memory Size for " << name << ": " << MemorySize << " Byte." << std::endl;
        //    std::cout << "Memory Location for " << name << ": " << &t << std::endl;
        //}

        inline void GetFPS(float dt) {
            std::cout << "Average FPS: " << (1.0f / dt) << std::endl;
        }

        inline void InitTasks(int num) {
            tasks.start(num);
        }

        Tasks* GetTasks() { 
            return &tasks; 
        }

        void AddMemoryInfo(MemoryInformations info) {
            memoryInformations.push_back(info);
        }

        inline void GetMemoryInfo() {
            for (int i = 0; i < memoryInformations.size(); ++i) {
                std::cout << memoryInformations.at(i).info << std::endl;
            }
        }

        void RemoveMemoryInfo(const char* name) {
            std::vector<MemoryInformations>::iterator it;
            it = std::find_if(memoryInformations.begin(), memoryInformations.end(), [name](MemoryInformations& info) {
                return info.name == name;
            });
            memoryInformations.erase(it);
            std::cout << "Free Memory From " << name << "\n" << std::endl;
        }

        void SetPhysicsInfo(int t) {
            ManifoldsInfo = t;
        }

        inline void GetPhysicsInfo() {
            std::cout << "Number of Manifold(s): " << ManifoldsInfo << "\n" << std::endl;
        }

        void ToggleDebugMode();
        bool getDebugMode() { return isDebug; }

        void UpdateDebug(float dt);
       
        void GetStartTime() {
            start = std::chrono::high_resolution_clock::now();
        }

        void GetEndTime() {
            end = std::chrono::high_resolution_clock::now();
        }

        void GetRunTime() {
            std::chrono::duration<double> elapsed = (end - start) * 1000;
            std::cout << "Run Time for Main Loop: " << elapsed.count() << "ms\n" << std::endl;
        }

    private:
        Tasks tasks;

        bool isDebug = false;
        size_t MemorySize;
        int ManifoldsInfo;

        Timepoint start, end;

        std::vector<MemoryInformations> memoryInformations;
};