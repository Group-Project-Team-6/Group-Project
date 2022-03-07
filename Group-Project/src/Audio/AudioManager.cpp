#include "AudioManager.h"

void AudioManager::InitSystem() {
    Common_Init(&extradriverdata);

    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);

    result = system->set3DSettings(1.0, DISTANCEFACTOR, 0.1f);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("wave.mp3"), FMOD_3D, 0, &sound);
    ERRCHECK(result);

    result = sound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 10000.0f * DISTANCEFACTOR);
    ERRCHECK(result);
}

void AudioManager::AudioUpdate(NCL::CSC8503::GameWorld* world, float dt) {
    Common_Update();

    FMOD_VECTOR pos = { 0.0f, 25.0f, -200.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    
    if (NCL::Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::P)) {
        result = system->playSound(sound, 0, false, &channel);
        ERRCHECK(result);
        result = channel->setVolume(5.0f);
        ERRCHECK(result);
        result = channel->set3DAttributes(&pos, &vel);
        ERRCHECK(result);
    }

    {       
            static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
            FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
            FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
            FMOD_VECTOR vel;

            NCL::Vector3 cameraPos = world->GetMainCamera()->GetPosition();
            float yaw = world->GetMainCamera()->GetYaw();
            
            forward.z = yaw;

            listenerpos = {cameraPos.x, cameraPos.y, cameraPos.z};

            vel.x = (listenerpos.x - lastpos.x) * dt;
            vel.y = (listenerpos.y - lastpos.y) * dt;
            vel.z = (listenerpos.z - lastpos.z) * dt;

            lastpos = listenerpos;

            result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
            ERRCHECK(result);
        }

    result = system->update();
    ERRCHECK(result);
}

void AudioManager::CacheRelease() {
    result = sound->release();
    ERRCHECK(result);

    Common_Close();
}