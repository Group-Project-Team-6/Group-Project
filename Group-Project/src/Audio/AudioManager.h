#pragma once

#include "fmod.hpp"
#include "common.h"
#include "../Common/Window.h"
#include <Vector>

class AudioManager {
    public:
        void InitSystem();

        void AudioUpdate(NCL::Window* w);
    private:
        FMOD::System              *system;
        FMOD::Sound               *sound;
        FMOD::Channel             *channel = 0;
        FMOD_RESULT                result;
        void                      *extradriverdata = 0;
};