#ifndef MUSICAPP_AUDIO_SDLAUDIOENGINE_H
#define MUSICAPP_AUDIO_SDLAUDIOENGINE_H

#include "AudioEngine.h"
#include "../Core/Note.h"
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <SDL3/SDL.h>

namespace MusicApp {
    namespace Audio {

        class SDLAudioEngine : public AudioEngine {
        public:
            SDLAudioEngine();

            ~SDLAudioEngine() override;

            bool init() override;

            void shutdown() override;

            void playSound(const std::string &instrumentName, const Core::Note &note) override;

        private:
            float getFrequencyForNote(const std::string &pitchName) const;

            bool isInitialized_;

            static const std::map<std::string, float> noteFrequencies_;
        };

    } // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_SDLAUDIOENGINE_H