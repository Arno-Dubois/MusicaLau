#include "../../include/Audio/SDLAudioEngine.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MusicApp {
    namespace Audio {

        // noteFrequencies_ is defined here as it was before.
        const std::map<std::string, float> SDLAudioEngine::noteFrequencies_ = {
            // Octave 0
            {"C0",   16.35f}, {"C#0",  17.32f}, {"Db0",  17.32f}, {"D0",   18.35f}, {"D#0",  19.45f}, {"Eb0",  19.45f}, {"E0",   20.60f}, {"F0",   21.83f}, {"F#0",  23.12f}, {"Gb0",  23.12f}, {"G0",   24.50f}, {"G#0",  25.96f}, {"Ab0",  25.96f}, {"A0",   27.50f}, {"A#0",  29.14f}, {"Bb0",  29.14f}, {"B0",   30.87f},
            // Octave 1
            {"C1",   32.70f}, {"C#1",  34.65f}, {"Db1",  34.65f}, {"D1",   36.71f}, {"D#1",  38.89f}, {"Eb1",  38.89f}, {"E1",   41.20f}, {"F1",   43.65f}, {"F#1",  46.25f}, {"Gb1",  46.25f}, {"G1",   49.00f}, {"G#1",  51.91f}, {"Ab1",  51.91f}, {"A1",   55.00f}, {"A#1",  58.27f}, {"Bb1",  58.27f}, {"B1",   61.74f},
            // Octave 2
            {"C2",   65.41f}, {"C#2",  69.30f}, {"Db2",  69.30f}, {"D2",   73.42f}, {"D#2",  77.78f}, {"Eb2",  77.78f}, {"E2",   82.41f}, {"F2",   87.31f}, {"F#2",  92.50f}, {"Gb2",  92.50f}, {"G2",   98.00f}, {"G#2",  103.83f}, {"Ab2",  103.83f}, {"A2",   110.00f}, {"A#2",  116.54f}, {"Bb2",  116.54f}, {"B2",   123.47f},
            // Octave 3
            {"C3",   130.81f}, {"C#3",  138.59f}, {"Db3",  138.59f}, {"D3",   146.83f}, {"D#3",  155.56f}, {"Eb3",  155.56f}, {"E3",   164.81f}, {"F3",   174.61f}, {"F#3",  185.00f}, {"Gb3",  185.00f}, {"G3",   196.00f}, {"G#3",  207.65f}, {"Ab3",  207.65f}, {"A3",   220.00f}, {"A#3",  233.08f}, {"Bb3",  233.08f}, {"B3",   246.94f},
            // Octave 4 (Middle C = C4)
            {"C4",   261.63f}, {"C#4",  277.18f}, {"Db4",  277.18f}, {"D4",   293.66f}, {"D#4",  311.13f}, {"Eb4",  311.13f}, {"E4",   329.63f}, {"F4",   349.23f}, {"F#4",  369.99f}, {"Gb4",  369.99f}, {"G4",   392.00f}, {"G#4",  415.30f}, {"Ab4",  415.30f}, {"A4",   440.00f}, {"A#4",  466.16f}, {"Bb4",  466.16f}, {"B4",   493.88f},
            // Octave 5
            {"C5",   523.25f}, {"C#5",  554.37f}, {"Db5",  554.37f}, {"D5",   587.33f}, {"D#5",  622.25f}, {"Eb5",  622.25f}, {"E5",   659.25f}, {"F5",   698.46f}, {"F#5",  739.99f}, {"Gb5",  739.99f}, {"G5",   783.99f}, {"G#5",  830.61f}, {"Ab5",  830.61f}, {"A5",   880.00f}, {"A#5",  932.33f}, {"Bb5",  932.33f}, {"B5",   987.77f},
            // Add more octaves if needed e.g. C-2 to B10 as before
        };


        SDLAudioEngine::SDLAudioEngine()
            : isInitialized_(false), audioStream_(nullptr), audioDevice_(0), activeNotesMutex_(nullptr) {
            std::cout << "SDLAudioEngine: Constructor called." << std::endl;
            activeNotesMutex_ = SDL_CreateMutex();
            if (!activeNotesMutex_) {
                std::cerr << "SDLAudioEngine: Failed to create mutex: " << SDL_GetError() << std::endl;
            }
        }

        SDLAudioEngine::~SDLAudioEngine() {
            std::cout << "SDLAudioEngine: Destructor called." << std::endl;
            if (isInitialized_) {
                shutdown();
            }
            if (activeNotesMutex_) {
                SDL_DestroyMutex(activeNotesMutex_);
                activeNotesMutex_ = nullptr;
            }
        }

        float SDLAudioEngine::getFrequencyForNote(const std::string &pitchName) const {
            auto it = noteFrequencies_.find(pitchName);
            if (it != noteFrequencies_.end()) {
                return it->second;
            }
            std::cerr << "SDLAudioEngine: Frequency for note \'" << pitchName
                    << "\' not found. Defaulting to 0 Hz (silence)." << std::endl;
            return 0.0f;
        }

        bool SDLAudioEngine::init() {
            std::cout << "SDLAudioEngine: Initializing with callback..." << std::endl;
            if (SDL_Init(SDL_INIT_AUDIO) < 0) {
                std::cerr << "SDLAudioEngine: Failed to initialize SDL_AUDIO: " << SDL_GetError() << std::endl;
                return false;
            }

            SDL_AudioSpec deviceSpecWant;
            SDL_zero(deviceSpecWant);
            deviceSpecWant.freq = 44100;
            deviceSpecWant.format = SDL_AUDIO_S16LE;
            deviceSpecWant.channels = 2;

            audioDevice_ = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &deviceSpecWant);
            std::cout << audioDevice_;
            if (audioDevice_ == 0) {
                std::cerr << "SDLAudioEngine: Failed to open audio device: " << SDL_GetError() << std::endl;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }
            std::cout << "SDLAudioEngine: Opened audio device ID: " << audioDevice_ << std::endl;


            audioStream_ = SDL_CreateAudioStream(&deviceSpecWant, &deviceSpecWant);
            if (!SDL_ResumeAudioDevice(audioDevice_)) {
                std::cerr << "SDLAudioEngine: Failed to create audio stream: " << SDL_GetError() << std::endl;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }
            std::cout << "SDLAudioEngine: Created audio stream." << std::endl;

            if (!SDL_SetAudioStreamGetCallback(audioStream_, audioCallback, this)) {
                 std::cerr << "SDLAudioEngine: Failed to set stream get data callback: " << SDL_GetError() << std::endl;
                 SDL_DestroyAudioStream(audioStream_);
                 audioStream_ = nullptr;
                 SDL_CloseAudioDevice(audioDevice_);
                 audioDevice_ = 0;
                 SDL_QuitSubSystem(SDL_INIT_AUDIO);
                 return false;
            }
            std::cout << "SDLAudioEngine: Set audio stream get data callback." << std::endl;

            if (!SDL_BindAudioStream(audioDevice_, audioStream_)) {
                std::cerr << "SDLAudioEngine: Failed to bind audio stream to device: " << SDL_GetError() << std::endl;
                // Clean up thoroughly
                SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);
                SDL_DestroyAudioStream(audioStream_);
                audioStream_ = nullptr;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }
            std::cout << "SDLAudioEngine: Bound audio stream to device." << std::endl;

            if (!SDL_ResumeAudioDevice(audioDevice_)) {
                 std::cerr << "SDLAudioEngine: Failed to resume audio device: " << SDL_GetError() << std::endl;
                // Clean up: unbind first if already bound
                SDL_BindAudioStream(audioDevice_, nullptr); // Try to unbind
                SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);
                SDL_DestroyAudioStream(audioStream_);
                audioStream_ = nullptr;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }
            std::cout << "SDLAudioEngine: Resumed audio device." << std::endl;

            isInitialized_ = true;
            std::cout << "SDLAudioEngine: Successfully initialized with callback. Sample Rate: " << deviceSpecWant.freq
                      << " Channels: " << (int)deviceSpecWant.channels << std::endl;
            return true;
        }

        void SDLAudioEngine::shutdown() {
            std::cout << "SDLAudioEngine: Shutting down..." << std::endl;
            if (isInitialized_) {
                if (audioDevice_ != 0) {
                    SDL_PauseAudioDevice(audioDevice_);
                    SDL_BindAudioStream(audioDevice_, nullptr);
                }
                if (audioStream_) {
                    SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);
                    SDL_ClearAudioStream(audioStream_);
                    SDL_DestroyAudioStream(audioStream_);
                    audioStream_ = nullptr;
                }
                if (audioDevice_ != 0) {
                    SDL_CloseAudioDevice(audioDevice_);
                    audioDevice_ = 0;
                }
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                isInitialized_ = false;
                std::cout << "SDLAudioEngine: Shutdown complete." << std::endl;
            }
        }

        void SDLAudioEngine::playSound(const std::string &instrumentName, const Core::Note &note) {
            if (!isInitialized_ || !activeNotesMutex_) {
                std::cerr << "SDLAudioEngine: Cannot play sound, not initialized or mutex missing." << std::endl;
                return;
            }

            float frequency = getFrequencyForNote(note.pitchName);
            if (frequency <= 0.0f) {
                std::cerr << "SDLAudioEngine: Invalid frequency for note \'" << note.pitchName << "\'." << std::endl;
                return;
            }

            std::string noteId = instrumentName + "_" + note.pitchName;

            SDL_LockMutex(activeNotesMutex_);
            auto it = activeNotes_.find(noteId);
            if (it != activeNotes_.end() && it->second.isPlaying && !it->second.needsRelease) {
                SDL_UnlockMutex(activeNotesMutex_);
                return;
            }

            ActiveNote newActiveNote;
            newActiveNote.instrumentName = instrumentName;
            newActiveNote.pitchName = note.pitchName;
            newActiveNote.frequency = frequency;
            newActiveNote.isPlaying = true;
            newActiveNote.needsRelease = false;
            newActiveNote.systemStartTimeMs = SDL_GetTicks();
            newActiveNote.currentTimeInSamples = 0;
            newActiveNote.currentEnvelopeValue = 0.0f;

            activeNotes_[noteId] = newActiveNote;

            std::cout << "SDLAudioEngine: Queued note \'" << note.pitchName << "\' (Freq: " << frequency <<
                    " Hz) for callback." << std::endl;
            SDL_UnlockMutex(activeNotesMutex_);
        }

        void SDLAudioEngine::stopSound(const std::string &instrumentName, const Core::Note &note) {
            if (!isInitialized_ || !activeNotesMutex_) return;

            std::string noteId = instrumentName + "_" + note.pitchName;
            SDL_LockMutex(activeNotesMutex_);
            auto it = activeNotes_.find(noteId);
            if (it != activeNotes_.end() && it->second.isPlaying) {
                it->second.isPlaying = false;
                it->second.needsRelease = true;
                it->second.currentTimeInSamples = 0;
                std::cout << "SDLAudioEngine: Marked note \'" << it->second.pitchName << "\' for release." << std::endl;
            }
            SDL_UnlockMutex(activeNotesMutex_);
        }

        void SDLAudioEngine::generateAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer, int numStereoSampleFrames) {
            const float twoPi = 2.0f * static_cast<float>(M_PI);
            const int totalMonoSamplesNeeded = numStereoSampleFrames * 2;

            for (int i = 0; i < totalMonoSamplesNeeded; i += 2) {
                float sampleValue = 0.0f;
                float envelope = 0.0f;

                if (note.isPlaying && !note.needsRelease) {
                    if (note.currentTimeInSamples < ActiveNote::ATTACK_DURATION_SAMPLES) {
                        note.currentEnvelopeValue = static_cast<float>(note.currentTimeInSamples) / ActiveNote::ATTACK_DURATION_SAMPLES;
                        envelope = note.currentEnvelopeValue;
                    } else if (note.currentTimeInSamples < (
                                   ActiveNote::ATTACK_DURATION_SAMPLES + ActiveNote::DECAY_DURATION_SAMPLES)) {
                        float decayProgress = (note.currentTimeInSamples - ActiveNote::ATTACK_DURATION_SAMPLES) / ActiveNote::DECAY_DURATION_SAMPLES;
                        note.currentEnvelopeValue = 1.0f - (1.0f - ActiveNote::SUSTAIN_LEVEL) * decayProgress;
                        envelope = note.currentEnvelopeValue;
                    } else {
                        note.currentEnvelopeValue = ActiveNote::SUSTAIN_LEVEL;
                        envelope = note.currentEnvelopeValue;
                    }
                    note.currentTimeInSamples++;
                } else if (note.needsRelease) {
                    if (note.currentTimeInSamples < ActiveNote::RELEASE_DURATION_SAMPLES) {
                        envelope = note.currentEnvelopeValue * (
                                       1.0f - (static_cast<float>(note.currentTimeInSamples) /
                                               ActiveNote::RELEASE_DURATION_SAMPLES));
                    } else {
                        envelope = 0.0f;
                        note.needsRelease = false;
                    }
                    note.currentTimeInSamples++;
                }

                if (envelope > 0.0001f) {
                    float time_in_seconds = static_cast<float>(note.currentTimeInSamples) / SAMPLE_RATE;
                    float oscillatorValue = std::sin(twoPi * note.frequency * time_in_seconds);
                    sampleValue = oscillatorValue * envelope;
                }

                int16_t intSample = static_cast<int16_t>(SDL_clamp(sampleValue * 32767.0f * 0.7f, -32768.0f, 32767.0f));
                buffer[i] = intSample;
                buffer[i + 1] = intSample;
            }
        }

        void SDLAudioEngine::audioCallback(void *userdata, SDL_AudioStream *sdlStream, int additional_amount, int total_amount) {
            auto *engine = static_cast<SDLAudioEngine *>(userdata);
            if (!engine || !engine->isInitialized_ || !engine->activeNotesMutex_) {
                std::vector<uint8_t> silence(additional_amount, 0);
                SDL_PutAudioStreamData(sdlStream, silence.data(), additional_amount);
                return;
            }

            int stereoSampleFramesNeeded = additional_amount / (sizeof(int16_t) * 2);
            if (stereoSampleFramesNeeded <= 0) return;

            std::vector<int16_t> mixBuffer(stereoSampleFramesNeeded * 2, 0);

            SDL_LockMutex(engine->activeNotesMutex_);

            std::vector<std::string> notesToRemove;
            for (auto &pair: engine->activeNotes_) {
                ActiveNote &note = pair.second;

                if (note.isPlaying || note.needsRelease) {
                    std::vector<int16_t> noteChunkBuffer(stereoSampleFramesNeeded * 2);
                    engine->generateAudioChunk(note, noteChunkBuffer, stereoSampleFramesNeeded);

                    for (size_t i = 0; i < mixBuffer.size(); ++i) {
                        float mixed_sample = static_cast<float>(mixBuffer[i]) + static_cast<float>(noteChunkBuffer[i]);
                        mixBuffer[i] = static_cast<int16_t>(SDL_clamp(mixed_sample, -32768.0f, 32767.0f));
                    }

                    if (!note.isPlaying && !note.needsRelease) {
                        notesToRemove.push_back(pair.first);
                    }
                }
            }

            for (const auto& noteId : notesToRemove) {
                engine->activeNotes_.erase(noteId);
                std::cout << "SDLAudioEngine: Removed note \'" << noteId << "\' from active list after release." <<
                        std::endl;
            }

            SDL_UnlockMutex(engine->activeNotesMutex_);

            if (SDL_PutAudioStreamData(sdlStream, mixBuffer.data(), additional_amount) < 0) {
                std::cerr << "SDLAudioEngine::audioCallback: Failed to put audio stream data: " << SDL_GetError() << std::endl;
            }
        }

        bool SDLAudioEngine::isNotePlaying(const std::string &instrumentName, const Core::Note &note) {
            if (!isInitialized_ || !activeNotesMutex_) return false;
            std::string noteId = instrumentName + "_" + note.pitchName;

            SDL_LockMutex(activeNotesMutex_);
            auto it = activeNotes_.find(noteId);
            bool isCurrentlyPlaying = false;
            if (it != activeNotes_.end()) {
                isCurrentlyPlaying = (it->second.isPlaying || it->second.needsRelease);
            }
            SDL_UnlockMutex(activeNotesMutex_);
            return isCurrentlyPlaying;
        }

        void SDLAudioEngine::cleanupLongPlayingNotes(Uint32 maxDurationMs) {
            if (!isInitialized_ || !activeNotesMutex_) return;

            Uint32 currentTimeMs = SDL_GetTicks();
            std::vector<std::pair<std::string, Core::Note>> notesToActuallyStop;

            SDL_LockMutex(activeNotesMutex_);
            for (auto const &[noteId, activeNote]: activeNotes_) {
                if (activeNote.isPlaying && !activeNote.needsRelease) {
                    if ((currentTimeMs - activeNote.systemStartTimeMs) > maxDurationMs) {
                        notesToActuallyStop.push_back({activeNote.instrumentName, Core::Note(activeNote.pitchName)});
                    }
                }
            }
            SDL_UnlockMutex(activeNotesMutex_);

            for (const auto &item : notesToActuallyStop) {
                std::cout << "SDLAudioEngine: Note \'" << item.second.pitchName
                        << "\' on instrument \'" << item.first
                        << "\' auto-stopped by cleanup after " << maxDurationMs << "ms" << std::endl;
                stopSound(item.first, item.second);
            }
        }

    } // namespace Audio
} // namespace MusicApp
