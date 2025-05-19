#include "../../include/Audio/SDLAudioEngine.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <SDL3/SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MusicApp {
    namespace Audio {
        static SDL_AudioStream *stream = nullptr;
        static SDLAudioEngine *currentEngine = nullptr;

        const std::map<std::string, float> SDLAudioEngine::noteFrequencies_ = {
                // Octave -2 (ultra-basse)
                {"C-2",  4.0879f},
                {"C#-2", 4.3304f},
                {"Db-2", 4.3304f},
                {"D-2",  4.5886f},
                {"D#-2", 4.8630f},
                {"Eb-2", 4.8630f},
                {"E-2",  5.1523f},
                {"F-2",  5.4577f},
                {"F#-2", 5.7805f},
                {"Gb-2", 5.7805f},
                {"G-2",  6.1249f},
                {"G#-2", 6.4891f},
                {"Ab-2", 6.4891f},
                {"A-2",  6.8750f},
                {"A#-2", 7.2838f},
                {"Bb-2", 7.2838f},
                {"B-2",  7.7167f},

                // Octave -1 (très basse)
                {"C-1",  8.1758f},
                {"C#-1", 8.6607f},
                {"Db-1", 8.6607f},
                {"D-1",  9.1771f},
                {"D#-1", 9.7261f},
                {"Eb-1", 9.7261f},
                {"E-1",  10.3013f},
                {"F-1",  10.9134f},
                {"F#-1", 11.5623f},
                {"Gb-1", 11.5623f},
                {"G-1",  12.2499f},
                {"G#-1", 12.9783f},
                {"Ab-1", 12.9783f},
                {"A-1",  13.7500f},
                {"A#-1", 14.5677f},
                {"Bb-1", 14.5677f},
                {"B-1",  15.4334f},

                // Octave 0
                {"C0",   16.35f},
                {"C#0",  17.32f},
                {"Db0",  17.32f},
                {"D0",   18.35f},
                {"D#0",  19.45f},
                {"Eb0",  19.45f},
                {"E0",   20.60f},
                {"F0",   21.83f},
                {"F#0",  23.12f},
                {"Gb0",  23.12f},
                {"G0",   24.50f},
                {"G#0",  25.96f},
                {"Ab0",  25.96f},
                {"A0",   27.50f},
                {"A#0",  29.14f},
                {"Bb0",  29.14f},
                {"B0",   30.87f},

                // Octave 1
                {"C1",   32.70f},
                {"C#1",  34.65f},
                {"Db1",  34.65f},
                {"D1",   36.71f},
                {"D#1",  38.89f},
                {"Eb1",  38.89f},
                {"E1",   41.20f},
                {"F1",   43.65f},
                {"F#1",  46.25f},
                {"Gb1",  46.25f},
                {"G1",   49.00f},
                {"G#1",  51.91f},
                {"Ab1",  51.91f},
                {"A1",   55.00f},
                {"A#1",  58.27f},
                {"Bb1",  58.27f},
                {"B1",   61.74f},

                // Octave 2
                {"C2",   65.41f},
                {"C#2",  69.30f},
                {"Db2",  69.30f},
                {"D2",   73.42f},
                {"D#2",  77.78f},
                {"Eb2",  77.78f},
                {"E2",   82.41f},
                {"F2",   87.31f},
                {"F#2",  92.50f},
                {"Gb2",  92.50f},
                {"G2",   98.00f},
                {"G#2",  103.83f},
                {"Ab2",  103.83f},
                {"A2",   110.00f},
                {"A#2",  116.54f},
                {"Bb2",  116.54f},
                {"B2",   123.47f},

                // Octave 3
                {"C3",   130.81f},
                {"C#3",  138.59f},
                {"Db3",  138.59f},
                {"D3",   146.83f},
                {"D#3",  155.56f},
                {"Eb3",  155.56f},
                {"E3",   164.81f},
                {"F3",   174.61f},
                {"F#3",  185.00f},
                {"Gb3",  185.00f},
                {"G3",   196.00f},
                {"G#3",  207.65f},
                {"Ab3",  207.65f},
                {"A3",   220.00f},
                {"A#3",  233.08f},
                {"Bb3",  233.08f},
                {"B3",   246.94f},

                // Octave 4
                {"C4",   261.63f},
                {"C#4",  277.18f},
                {"Db4",  277.18f},
                {"D4",   293.66f},
                {"D#4",  311.13f},
                {"Eb4",  311.13f},
                {"E4",   329.63f},
                {"F4",   349.23f},
                {"F#4",  369.99f},
                {"Gb4",  369.99f},
                {"G4",   392.00f},
                {"G#4",  415.30f},
                {"Ab4",  415.30f},
                {"A4",   440.00f},
                {"A#4",  466.16f},
                {"Bb4",  466.16f},
                {"B4",   493.88f},

                // Octave 5
                {"C5",   523.25f},
                {"C#5",  554.37f},
                {"Db5",  554.37f},
                {"D5",   587.33f},
                {"D#5",  622.25f},
                {"Eb5",  622.25f},
                {"E5",   659.25f},
                {"F5",   698.46f},
                {"F#5",  739.99f},
                {"Gb5",  739.99f},
                {"G5",   783.99f},
                {"G#5",  830.61f},
                {"Ab5",  830.61f},
                {"A5",   880.00f},
                {"A#5",  932.33f},
                {"Bb5",  932.33f},
                {"B5",   987.77f},

                // Octave 6
                {"C6",   1046.50f},
                {"C#6",  1108.73f},
                {"Db6",  1108.73f},
                {"D6",   1174.66f},
                {"D#6",  1244.51f},
                {"Eb6",  1244.51f},
                {"E6",   1318.51f},
                {"F6",   1396.91f},
                {"F#6",  1479.98f},
                {"Gb6",  1479.98f},
                {"G6",   1567.98f},
                {"G#6",  1661.22f},
                {"Ab6",  1661.22f},
                {"A6",   1760.00f},
                {"A#6",  1864.66f},
                {"Bb6",  1864.66f},
                {"B6",   1975.53f},

                // Octave 7
                {"C7",   2093.00f},
                {"C#7",  2217.46f},
                {"Db7",  2217.46f},
                {"D7",   2349.32f},
                {"D#7",  2489.02f},
                {"Eb7",  2489.02f},
                {"E7",   2637.02f},
                {"F7",   2793.83f},
                {"F#7",  2959.96f},
                {"Gb7",  2959.96f},
                {"G7",   3135.96f},
                {"G#7",  3322.44f},
                {"Ab7",  3322.44f},
                {"A7",   3520.00f},
                {"A#7",  3729.31f},
                {"Bb7",  3729.31f},
                {"B7",   3951.07f},

                // Octave 8
                {"C8",   4186.01f},
                {"C#8",  4434.92f},
                {"Db8",  4434.92f},
                {"D8",   4698.63f},
                {"D#8",  4978.03f},
                {"Eb8",  4978.03f},
                {"E8",   5274.04f},
                {"F8",   5587.65f},
                {"F#8",  5919.91f},
                {"Gb8",  5919.91f},
                {"G8",   6271.93f},
                {"G#8",  6644.88f},
                {"Ab8",  6644.88f},
                {"A8",   7040.00f},
                {"A#8",  7458.62f},
                {"Bb8",  7458.62f},
                {"B8",   7902.13f},

                // Octave 9
                {"C9",   8372.02f},
                {"C#9",  8869.84f},
                {"Db9",  8869.84f},
                {"D9",   9397.27f},
                {"D#9",  9956.06f},
                {"Eb9",  9956.06f},
                {"E9",   10548.08f},
                {"F9",   11175.30f},
                {"F#9",  11839.82f},
                {"Gb9",  11839.82f},
                {"G9",   12543.85f},
                {"G#9",  13289.75f},
                {"Ab9",  13289.75f},
                {"A9",   14080.00f},
                {"A#9",  14917.24f},
                {"Bb9",  14917.24f},
                {"B9",   15804.26f},

                // Octave 10
                {"C10",  16744.04f},
                {"C#10", 17739.69f},
                {"Db10", 17739.69f},
                {"D10",  18794.54f},
                {"D#10", 19912.13f},
                {"Eb10", 19912.13f},
                {"E10",  21096.16f},
                {"F10",  22350.61f},
                {"F#10", 23679.64f},
                {"Gb10", 23679.64f},
                {"G10",  25087.71f},
                {"G#10", 26579.50f},
                {"Ab10", 26579.50f},
                {"A10",  28160.00f},
                {"A#10", 29834.48f},
                {"Bb10", 29834.48f},
                {"B10",  31608.52f},
        };

        SDLAudioEngine::SDLAudioEngine()
                : isInitialized_(false) {
            std::cout << "SDLAudioEngine: Constructor called." << std::endl;
            currentEngine = this; // Stocker l'instance pour le callback
        }

        SDLAudioEngine::~SDLAudioEngine() {
            std::cout << "SDLAudioEngine: Destructor called." << std::endl;
            if (isInitialized_) {
                shutdown();
            }
            if (currentEngine == this) {
                currentEngine = nullptr;
            }
        }

        float SDLAudioEngine::getFrequencyForNote(const std::string &pitchName) const {
            auto it = noteFrequencies_.find(pitchName);
            if (it != noteFrequencies_.end()) {
                return it->second;
            }

            try {
                size_t processedChars = 0;
                float freq = std::stof(pitchName, &processedChars);
                if (processedChars == pitchName.length()) {
                    return freq;
                }
            } catch (...) {
                // Ignore exceptions
            }

            std::cerr << "SDLAudioEngine: Frequency for note '" << pitchName
                      << "' not found. Defaulting to 0 Hz (silence)." << std::endl;
            return 0.0f;
        }

        bool SDLAudioEngine::init() {
            std::cout << "SDLAudioEngine: Initializing..." << std::endl;
            if (SDL_Init(SDL_INIT_AUDIO) < 0) {
                std::cerr << "SDLAudioEngine: Failed to initialize SDL_AUDIO: " << SDL_GetError() << std::endl;
                return false;
            }

            SDL_AudioSpec want;
            SDL_zero(want);
            want.freq = 44100;
            want.format = SDL_AUDIO_S16LE;
            want.channels = 2;

            // Créer un stream audio sans callback
            stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, nullptr, nullptr);
            if (!stream) {
                std::cerr << "SDLAudioEngine: Failed to open audio stream: " << SDL_GetError() << std::endl;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            if (SDL_ResumeAudioStreamDevice(stream) < 0) {
                std::cerr << "SDLAudioEngine: Failed to resume audio stream: " << SDL_GetError() << std::endl;
                SDL_DestroyAudioStream(stream);
                stream = nullptr;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            isInitialized_ = true;
            std::cout << "SDLAudioEngine: Successfully initialized." << std::endl;
            return true;
        }

        void SDLAudioEngine::shutdown() {
            std::cout << "SDLAudioEngine: Shutting down..." << std::endl;
            if (isInitialized_) {
                if (stream) {
                    SDL_FlushAudioStream(stream);
                    SDL_DestroyAudioStream(stream);
                    stream = nullptr;
                }

                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                isInitialized_ = false;
                std::cout << "SDLAudioEngine: Shutdown complete." << std::endl;
            }
        }

        std::vector<int16_t>
        SDLAudioEngine::generateWaveform(float frequency, float durationSeconds, bool sustainMode) {
            unsigned int sampleRate = 44100;
            // Pour les notes maintenues, générer une durée plus longue pour le sustain
            float actualDuration = sustainMode ? 10.0f : durationSeconds;

            const size_t sampleCount = static_cast<size_t>(sampleRate * actualDuration);
            std::vector<int16_t> samples(sampleCount);
            const float twoPi = 2.0f * static_cast<float>(M_PI);

            // Paramètres d'enveloppe ADSR pour le piano
            float attack = 0.005f;   // 5ms - attaque rapide
            float decay = 0.1f;      // 100ms - déclin rapide après le pic initial
            float sustain = 0.7f;    // niveau de sustain à 70% du maximum
            float release = sustainMode ? 0.0f : 0.2f;    // relâchement uniquement en mode non-sustain

            // Coefficients d'harmoniques pour le piano
            // Ces coefficients modulent l'amplitude des harmoniques pour imiter le timbre du piano
            std::vector<float> harmonicAmps = {
                    1.0f,    // Fondamentale
                    0.5f,    // 2ème harmonique
                    0.33f,   // 3ème harmonique
                    0.25f,   // 4ème harmonique
                    0.15f,   // 5ème harmonique
                    0.12f,   // 6ème harmonique
                    0.08f    // 7ème harmonique
            };

            // Léger désaccord pour les harmoniques supérieures (simulation d'inharmonicité)
            std::vector<float> detuneFactors = {
                    1.0f,     // Fondamentale
                    2.0f,     // 2ème harmonique
                    3.001f,   // 3ème harmonique légèrement désaccordée
                    4.002f,   // 4ème harmonique légèrement désaccordée
                    5.004f,   // etc
                    6.006f,
                    7.009f
            };

            for (size_t i = 0; i < sampleCount; ++i) {
                float time = static_cast<float>(i) / sampleRate;
                float value = 0.0f;

                // Synthèse additive - Mélanger les harmoniques
                for (size_t h = 0; h < harmonicAmps.size(); ++h) {
                    float harmonicFreq = frequency * detuneFactors[h];

                    // Réduire l'amplitude des harmoniques pour les notes basses
                    float ampFactor = harmonicAmps[h];
                    if (frequency < 100.0f) {
                        // Réduit progressivement les harmoniques hautes pour les notes graves
                        ampFactor *= (h == 0) ? 1.0f : (0.9f - 0.1f * h) * (frequency / 100.0f);
                    }

                    value += ampFactor * std::sin(twoPi * harmonicFreq * time);
                }

                // Normaliser le mélange des harmoniques
                value /= 1.5f;  // Facteur pour éviter l'écrêtage

                // Appliquer l'enveloppe ADSR
                float envelope;
                if (time < attack) {
                    // Phase d'attaque (progressive)
                    envelope = time / attack;
                } else if (time < attack + decay) {
                    // Phase de decay
                    float decayProgress = (time - attack) / decay;
                    envelope = 1.0f - (1.0f - sustain) * decayProgress;
                } else if (sustainMode || time < actualDuration - release) {
                    // Phase de sustain - pour les notes maintenues, cela peut durer longtemps
                    envelope = sustain;

                    // Pour le piano : déclin naturel durant le sustain (seulement pour les notes à durée fixe)
                    if (!sustainMode) {
                        float sustainProgress =
                                (time - (attack + decay)) / (actualDuration - release - (attack + decay));
                        envelope *= 1.0f - 0.3f * sustainProgress;
                    } else {
                        // En mode sustain, on applique un très léger déclin pour un son plus naturel
                        // mais qui reste essentiellement constant jusqu'au relâchement
                        float sustainProgress = time / actualDuration;
                        envelope *= 1.0f - 0.05f * sustainProgress;
                    }
                } else {
                    // Phase de relâchement
                    envelope = sustain * (actualDuration - time) / release;
                }

                // Réduction du maintien pour les notes aiguës (simulation des cordes courtes)
                if (frequency > 1000.0f) {
                    float reductionFactor = std::min(1.0f, 2.0f - frequency / 2000.0f);
                    envelope *= reductionFactor;
                }

                // Appliquer l'enveloppe et limiter à [-1,1]
                value *= envelope;
                value = std::max(-1.0f, std::min(1.0f, value));

                samples[i] = static_cast<int16_t>(value * 32767);
            }

            return samples;
        }

        void SDLAudioEngine::playSound(const std::string &instrumentName, const Core::Note &note) {
            if (!isInitialized_ || !stream) {
                std::cerr << "SDLAudioEngine: Cannot play sound, audio engine not initialized." << std::endl;
                return;
            }

            float frequency = getFrequencyForNote(note.pitchName);
            if (frequency <= 0.0f) {
                std::cerr << "SDLAudioEngine: Invalid frequency for note '" << note.pitchName << "'." << std::endl;
                return;
            }

            std::cout << "SDLAudioEngine: Playing note '" << note.pitchName
                      << "' (Freq: " << frequency << " Hz)" << std::endl;

            // Identifiant unique pour cette note
            std::string noteId = instrumentName + "_" + note.pitchName;

            // Si la note est déjà jouée, l'arrêter d'abord pour éviter les clics
            auto it = activeNotes.find(noteId);
            if (it != activeNotes.end()) {
                stopSound(instrumentName, note);
            }

            // Générer une forme d'onde très courte (0.5 secondes) pour éviter les notes qui sonnent trop longtemps
            float noteDuration = 0.5f;
            if (frequency < 100.0f) {
                noteDuration = 0.7f;  // Notes graves un peu plus longues
            } else if (frequency > 1000.0f) {
                noteDuration = 0.3f;  // Notes aiguës un peu plus courtes
            }

            // Générer le son
            std::vector<int16_t> samples = generateWaveform(frequency, noteDuration, false);

            // Envoyer directement les données audio au stream
            int byteSize = static_cast<int>(samples.size() * sizeof(int16_t));
            if (SDL_PutAudioStreamData(stream, samples.data(), byteSize) < 0) {
                std::cerr << "SDL_PutAudioStreamData error: " << SDL_GetError() << std::endl;
            }

            // Enregistrer cette note comme active
            ActiveNote activeNote;
            activeNote.instrumentName = instrumentName;
            activeNote.pitchName = note.pitchName;
            activeNote.frequency = frequency;
            activeNote.isPlaying = true;
            activeNote.startTime = SDL_GetTicks();  // Enregistrer le moment de début de la note
            activeNotes[noteId] = std::move(activeNote);
        }

        void SDLAudioEngine::stopSound(const std::string &instrumentName, const Core::Note &note) {
            std::string noteId = instrumentName + "_" + note.pitchName;

            // Si la note est en train de jouer, générer le relâchement
            auto it = activeNotes.find(noteId);
            if (it != activeNotes.end() && it->second.isPlaying) {
                // Générer une petite forme d'onde de relâchement pour éviter les clics
                std::vector<int16_t> releaseSamples = generateWaveform(it->second.frequency, 0.2f, false);

                // Envoyer les données de relâchement au stream
                int byteSize = static_cast<int>(releaseSamples.size() * sizeof(int16_t));
                if (SDL_PutAudioStreamData(stream, releaseSamples.data(), byteSize) < 0) {
                    std::cerr << "SDL_PutAudioStreamData error for release: " << SDL_GetError() << std::endl;
                }

                activeNotes.erase(noteId);
                std::cout << "SDLAudioEngine: Stopped note '" << note.pitchName << "'" << std::endl;
            }
        }

        bool SDLAudioEngine::isNotePlaying(const std::string &instrumentName, const Core::Note &note) {
            std::string noteId = instrumentName + "_" + note.pitchName;
            auto it = activeNotes.find(noteId);
            return (it != activeNotes.end() && it->second.isPlaying);
        }

        void SDLAudioEngine::cleanupLongPlayingNotes(Uint32 maxDurationMs) {
            Uint32 currentTime = SDL_GetTicks();
            std::vector<std::string> notesToStop;

            // Identifier les notes qui jouent depuis trop longtemps
            for (const auto &pair: activeNotes) {
                Uint32 elapsedTime = currentTime - pair.second.startTime;
                if (elapsedTime > maxDurationMs) {
                    notesToStop.push_back(pair.first);
                }
            }

            // Arrêter ces notes
            for (const auto &noteId: notesToStop) {
                auto it = activeNotes.find(noteId);
                if (it != activeNotes.end()) {
                    // Créer une note temporaire pour l'arrêter
                    Core::Note note(it->second.pitchName);
                    stopSound(it->second.instrumentName, note);
                    std::cout << "SDLAudioEngine: Note " << it->second.pitchName
                              << " auto-stopped after " << maxDurationMs << "ms" << std::endl;
                }
            }
        }
    } // namespace Audio
} // namespace MusicApp