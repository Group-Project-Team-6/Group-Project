#include <iostream>
#include <string>

class DebugMode {
    public:
        template<class T>
        void GetMemoryAllocationSize(T&& t) {
            MemorySize = sizeof(t);
            const char* name = typeid(t).name();
            std::cout << "Memory Size for " << name << ": " << MemorySize << " Btye." << std::endl;
        }

    private:
        size_t MemorySize;
};