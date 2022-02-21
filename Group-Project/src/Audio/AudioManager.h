#pragma once

#include "fmod.hpp"
#include "common.h"
#include "../Common/Window.h"
#include <Vector>

class AudioManager {
    public:
        void InitSystem();
        //void AddSound(const char *name_or_data);

        void AudioUpdate(NCL::Window* w);
    private:
        FMOD::System              *system;
        FMOD::Sound               *sound;
        FMOD::Channel             *channel = 0;
        //std::vector<FMOD::Sound*>  sounds;
        FMOD_RESULT                result;
        void                      *extradriverdata = 0;
};