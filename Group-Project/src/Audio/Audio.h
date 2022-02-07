#include "fmod.hpp"
#include "common.h"

namespace AUDIO{ 
    class Audio{
        public:
        Audio();
        ~Audio();
        void Init();
        void Loop();

        private:
        FMOD::System *system;
        
    };

}
