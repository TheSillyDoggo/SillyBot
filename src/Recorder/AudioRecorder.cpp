#include "AudioRecorder.hpp"

void AudioRecorder::startRecording()
{
    isRecording = true;

    FMOD_CREATESOUNDEXINFO exinfo = {};
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 1;
    exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = 44100;
    exinfo.length = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 10; // 10 seconds buffer

    FMODAudioEngine::sharedEngine()->m_system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);

    FMOD_RESULT result = FMODAudioEngine::sharedEngine()->m_system->recordStart(0, FMODAudioEngine::sharedEngine()->m_sound, true);
}

void AudioRecorder::endRecording()
{
    isRecording = false;

    FMODAudioEngine::sharedEngine()->m_system->recordStop(0);

    std::string filename = "sound.wav";

    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    void* ptr1 = nullptr;
    void* ptr2 = nullptr;
    unsigned int len1 = 0;
    unsigned int len2 = 0;

    unsigned int len = 0;

    sound->getLength(&len, FMOD_TIMEUNIT_RAWBYTES);

    FMOD_RESULT result = sound->lock(0, len, &ptr1, &ptr2, &len1, &len2);
    if (result != FMOD_OK) {
        //std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
        log::info("Error: {}", as<int>(result));
        fclose(file);
        return;
    }

    fwrite(ptr1, len1, 1, file);
    if (ptr2) fwrite(ptr2, len2, 1, file);

    sound->unlock(ptr1, ptr2, len1, len2);
    fclose(file);
}

void AudioRecorder::toggleRecording()
{
    if (isRecording)
        endRecording();
    else
        startRecording();
}