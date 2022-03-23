#include "DebugMode.h"

void DebugMode::ToggleDebugMode() {
    isDebug = !isDebug;
}

void DebugMode::DebugUpdate() {
    if (!isDebug) {
        return;
    }
}