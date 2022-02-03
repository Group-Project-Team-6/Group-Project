#include "fmod.hpp"
#include "common.h"

namespace AUDIO{ 
    class Audio{
        public:
        void Init();

        private:
        FMOD::System     *system;
        FMOD::Sound      *sound1, *sound2, *sound3;
        FMOD::Channel    *channel = 0;
        FMOD_RESULT       result;
        void             *extradriverdata = 0;
    };

}
