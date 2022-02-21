#include "AudioManager.h"
#include "../Common/Window.h"


void AudioManager::InitSystem() {
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("Pickup.wav"), FMOD_DEFAULT, 0, &sound);
    ERRCHECK(result);

    //AddSound("wave.mp3"); //0
}
/*
void AudioManager::AddSound(const char *name_or_data) {
    //sounds.push_back(sound);
    //result = system->createSound(Common_MediaPath(name_or_data), FMOD_DEFAULT, 0, &sounds.back());
    //ERRCHECK(result);

}
*/
void AudioManager::AudioUpdate(NCL::Window* w) {
    Common_Update();

    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::P)) {
        result = system->playSound(sound, 0, false, &channel);
        ERRCHECK(result);
    }

    /*
    if (Common_BtnPress(BTN_ACTION1))
    {
            result = system->playSound(sound, 0, false, &channel);
            ERRCHECK(result);
    }
    */
    result = system->update();
    ERRCHECK(result);

    {
        unsigned int ms = 0;
        unsigned int lenms = 0;
        bool         playing = 0;
        bool         paused = 0;
        int          channelsplaying = 0;

        if (channel)
        {
            FMOD::Sound *currentsound = 0;

            result = channel->isPlaying(&playing);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }

            result = channel->getPaused(&paused);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }

            result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }
               
            channel->getCurrentSound(&currentsound);
            if (currentsound)
            {
                result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
                if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
                {
                    ERRCHECK(result);
                }
            }
        }

        system->getChannelsPlaying(&channelsplaying, NULL);
    }
}