#pragma once

#include "../FMOD/api/core/inc/fmod.hpp"
#include "common.h"
#include "../Common/Window.h"
#include "../Common/Camera.h"
#include "../CSC8503/GameWorld.h"
#include "../Common/Vector3.h"

#include <Vector>

class AudioManager {
    public:
        void InitSystem();

        void AudioUpdate(NCL::CSC8503::GameWorld* world, float dt);

        void CacheRelease();
    private:
        const float     DISTANCEFACTOR = 1.0f;
        FMOD::System   *system;
        FMOD::Sound    *sound;
        FMOD::Channel  *channel = 0;
        FMOD_RESULT     result;
        void           *extradriverdata = 0;
        FMOD_VECTOR     listenerpos  = { 0.0f, 0.0f, -1.0f * DISTANCEFACTOR };
};