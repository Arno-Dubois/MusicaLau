#ifndef MUSICAPP_AUDIO_SDLAUDIOENGINE_H
#define MUSICAPP_AUDIO_SDLAUDIOENGINE_H

#include "AudioEngine.h"
#include "../Core/Note.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <SDL3/SDL.h>

namespace MusicApp {
    namespace Audio {

        struct ActiveNote {
            std::string instrumentName;
            std::string pitchName;
            float frequency;
            bool isPlaying;
            Uint32 startTime;  // Timestamp du début de la note
        };

        class SDLAudioEngine : public AudioEngine {
        public:
            SDLAudioEngine();

            ~SDLAudioEngine() override;

            bool init() override;

            void shutdown() override;

            void playSound(const std::string &instrumentName, const Core::Note &note) override;

            void stopSound(const std::string &instrumentName, const Core::Note &note);

            bool isNotePlaying(const std::string &instrumentName, const Core::Note &note);

            // Nettoie les notes qui jouent depuis trop longtemps
            void cleanupLongPlayingNotes(Uint32 maxDurationMs = 100);

        private:
            float getFrequencyForNote(const std::string &pitchName) const;

            std::vector<int16_t> generateWaveform(float frequency, float durationSeconds, bool sustainMode = true);

            bool isInitialized_;
            std::unordered_map<std::string, ActiveNote> activeNotes;

            static const std::map<std::string, float> noteFrequencies_;
        };

    } // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_SDLAUDIOENGINE_H