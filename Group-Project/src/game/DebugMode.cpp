#include "DebugMode.h"

void DebugMode::ToggleDebugMode() {
    isDebug = !isDebug;
}

void DebugMode::DebugUpdate() {
    if (!isDebug) {
        return;
    }
}



/*
#ifndef DISABLE_DEFAULT_ALLOCATORS
static void*   MallocWrapper(size_t size, void* user_data)    { Debug_UNUSED(user_data); return malloc(size); }
static void    FreeWrapper(void* ptr, void* user_data)        { Debug_UNUSED(user_data); free(ptr); }
#else
static void*   MallocWrapper(size_t size, void* user_data)    { Debug_UNUSED(user_data); Debug_UNUSED(size); Debug_ASSERT(0); return NULL; }
static void    FreeWrapper(void* ptr, void* user_data)        { Debug_UNUSED(user_data); Debug_UNUSED(ptr); Debug_ASSERT(0); }
#endif
static Debug::DebugMemAllocFunc    DebugmAllocatorAllocFunc = MallocWrapper;
static Debug::DebugMemFreeFunc     DebugmAllocatorFreeFunc = FreeWrapper;
static void*                       DebugmAllocatorUserData = NULL;

void Debug::SetAllocatorFunctions(DebugMemAllocFunc alloc_func, DebugMemFreeFunc free_func, void* user_data)
{
    DebugmAllocatorAllocFunc = alloc_func;
    DebugmAllocatorFreeFunc = free_func;
    DebugmAllocatorUserData = user_data;
}

// This is provided to facilitate copying allocators from one static/DLL boundary to another (e.g. retrieve default allocator of your executable address space)
void Debug::GetAllocatorFunctions(DebugMemAllocFunc* p_alloc_func, DebugMemFreeFunc* p_free_func, void** p_user_data)
{
    *p_alloc_func = DebugmAllocatorAllocFunc;
    *p_free_func = DebugmAllocatorFreeFunc;
    *p_user_data = DebugmAllocatorUserData;
}

// IM_ALLOC() == ImGui::MemAlloc()
void* Debug::MemAlloc(size_t size)
{
    return (*DebugmAllocatorAllocFunc)(size, DebugmAllocatorUserData);
}

// IM_FREE() == ImGui::MemFree()
void Debug::MemFree(void* ptr)
{
    //if (ptr)
    return (*DebugmAllocatorFreeFunc)(ptr, DebugmAllocatorUserData);
}

*/

