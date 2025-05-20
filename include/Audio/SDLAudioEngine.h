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
#include <SDL3/SDL_mutex.h> // For SDL_Mutex

namespace MusicApp {
    namespace Audio {

        struct ActiveNote {
            std::string instrumentName;
            std::string pitchName;
            float frequency;
            bool isPlaying;          // True if note is in Attack, Decay, or Sustain phase
            Uint32 systemStartTimeMs; // SDL_GetTicks() when playSound was called
            float velocity;          // Between 0.0 and 1.0, representing the strength of the note

            float currentTimeInSamples; // Current sample position in the note's lifecycle (for ADSR or release phase)
            bool needsRelease;       // Flag to indicate if release envelope should be played
            float currentEnvelopeValue; // To allow smooth transition to release from any point

            // Variables pour éviter les bruits parasites
            float phase;             // Phase continue pour la génération d'onde sonore
            float prevSampleLeft;    // Échantillon gauche précédent pour le crossfading
            float prevSampleRight;   // Échantillon droit précédent pour le crossfading

            // ADSR parameters (durations in samples at a fixed sample rate)
            // These assume SAMPLE_RATE is 44100. Define them with that assumption.
            static constexpr float ATTACK_DURATION_SAMPLES = 44100 * 0.01f; // 10ms
            static constexpr float DECAY_DURATION_SAMPLES = 44100 * 0.1f;   // 100ms
            static constexpr float SUSTAIN_LEVEL = 0.7f;
            static constexpr float RELEASE_DURATION_SAMPLES = 44100 * 0.2f; // 200ms

            ActiveNote() : frequency(0.0f), isPlaying(false), systemStartTimeMs(0), velocity(1.0f),
                           currentTimeInSamples(0.0f), needsRelease(false), currentEnvelopeValue(0.0f),
                           phase(0.0f), prevSampleLeft(0.0f), prevSampleRight(0.0f) {}
        };

        class SDLAudioEngine : public AudioEngine {
        public:
            SDLAudioEngine();

            ~SDLAudioEngine() override;

            bool init() override;

            void shutdown() override;

            void playSound(const std::string &instrumentName, const Core::Note &note) override;

            void playSound(const std::string &instrumentName, const Core::Note &note, float velocity);

            void stopSound(const std::string &instrumentName, const Core::Note &note);

            bool isNotePlaying(const std::string &instrumentName, const Core::Note &note);

            void cleanupLongPlayingNotes(Uint32 maxDurationMs = 5000);

        private:
            // Static audio callback function
            static void audioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);

            // Generates a chunk of waveform data for a single note
            void generateAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer, int numStereoSampleFrames);

            // Generates xylophone sound specifically (brighter, shorter decay)
            void generateXylophoneAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer, int numStereoSampleFrames);

            // Generates 8-bit chiptune sound for video game console
            void generate8BitAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer, int numStereoSampleFrames);

            float getFrequencyForNote(const std::string &pitchName) const;

            bool isInitialized_;
            SDL_AudioStream *audioStream_;      // Audio stream for the callback
            SDL_AudioDeviceID audioDevice_;     // Audio device ID

            std::unordered_map<std::string, ActiveNote> activeNotes_; // Protected by activeNotesMutex_
            SDL_Mutex *activeNotesMutex_;       // Mutex for thread-safe access to activeNotes

            static const std::map<std::string, float> noteFrequencies_;
            static const unsigned int SAMPLE_RATE = 44100; // Make sample rate a known constant for ADSR calculations
        };

    } // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_SDLAUDIOENGINE_H