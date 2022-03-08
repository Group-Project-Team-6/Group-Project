#include <iostream>

class DebugMode {
    public:
        template<class T>
        void GetMemoryAllocationSize(T&& t) {
            MemorySize = sizeof(t);
            std::cout << "Memory Size: " << MemorySize << std::endl;
        }

    private:
        size_t MemorySize;
};