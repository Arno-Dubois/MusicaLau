#include "../../include/Audio/SDLAudioEngine.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <algorithm>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MusicApp {
    namespace Audio {

        // noteFrequencies_ is defined here as it was before.
        const std::map<std::string, float> SDLAudioEngine::noteFrequencies_ = {
                // Octave 0
                {"C0",  16.35f},
                {"C#0", 17.32f},
                {"Db0", 17.32f},
                {"D0",  18.35f},
                {"D#0", 19.45f},
                {"Eb0", 19.45f},
                {"E0",  20.60f},
                {"F0",  21.83f},
                {"F#0", 23.12f},
                {"Gb0", 23.12f},
                {"G0",  24.50f},
                {"G#0", 25.96f},
                {"Ab0", 25.96f},
                {"A0",  27.50f},
                {"A#0", 29.14f},
                {"Bb0", 29.14f},
                {"B0",  30.87f},
                // Octave 1
                {"C1",  32.70f},
                {"C#1", 34.65f},
                {"Db1", 34.65f},
                {"D1",  36.71f},
                {"D#1", 38.89f},
                {"Eb1", 38.89f},
                {"E1",  41.20f},
                {"F1",  43.65f},
                {"F#1", 46.25f},
                {"Gb1", 46.25f},
                {"G1",  49.00f},
                {"G#1", 51.91f},
                {"Ab1", 51.91f},
                {"A1",  55.00f},
                {"A#1", 58.27f},
                {"Bb1", 58.27f},
                {"B1",  61.74f},
                // Octave 2
                {"C2",  65.41f},
                {"C#2", 69.30f},
                {"Db2", 69.30f},
                {"D2",  73.42f},
                {"D#2", 77.78f},
                {"Eb2", 77.78f},
                {"E2",  82.41f},
                {"F2",  87.31f},
                {"F#2", 92.50f},
                {"Gb2", 92.50f},
                {"G2",  98.00f},
                {"G#2", 103.83f},
                {"Ab2", 103.83f},
                {"A2",  110.00f},
                {"A#2", 116.54f},
                {"Bb2", 116.54f},
                {"B2",  123.47f},
                // Octave 3
                {"C3",  130.81f},
                {"C#3", 138.59f},
                {"Db3", 138.59f},
                {"D3",  146.83f},
                {"D#3", 155.56f},
                {"Eb3", 155.56f},
                {"E3",  164.81f},
                {"F3",  174.61f},
                {"F#3", 185.00f},
                {"Gb3", 185.00f},
                {"G3",  196.00f},
                {"G#3", 207.65f},
                {"Ab3", 207.65f},
                {"A3",  220.00f},
                {"A#3", 233.08f},
                {"Bb3", 233.08f},
                {"B3",  246.94f},
                // Octave 4 (Middle C = C4)
                {"C4",  261.63f},
                {"C#4", 277.18f},
                {"Db4", 277.18f},
                {"D4",  293.66f},
                {"D#4", 311.13f},
                {"Eb4", 311.13f},
                {"E4",  329.63f},
                {"F4",  349.23f},
                {"F#4", 369.99f},
                {"Gb4", 369.99f},
                {"G4",  392.00f},
                {"G#4", 415.30f},
                {"Ab4", 415.30f},
                {"A4",  440.00f},
                {"A#4", 466.16f},
                {"Bb4", 466.16f},
                {"B4",  493.88f},
                // Octave 5
                {"C5",  523.25f},
                {"C#5", 554.37f},
                {"Db5", 554.37f},
                {"D5",  587.33f},
                {"D#5", 622.25f},
                {"Eb5", 622.25f},
                {"E5",  659.25f},
                {"F5",  698.46f},
                {"F#5", 739.99f},
                {"Gb5", 739.99f},
                {"G5",  783.99f},
                {"G#5", 830.61f},
                {"Ab5", 830.61f},
                {"A5",  880.00f},
                {"A#5", 932.33f},
                {"Bb5", 932.33f},
                {"B5",  987.77f},
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

            // Support pour les notes spéciales de la console 8-bit (format 8bit_X)
            if (pitchName.substr(0, 5) == "8bit_") {
                // Extraire l'index du bouton à partir du nom de la note
                try {
                    int buttonIndex = std::stoi(pitchName.substr(5));

                    // Calcul de la fréquence pour une gamme chromatique complète 
                    // à partir de C4 (Do4) pour deux octaves
                    const float C4_FREQUENCY = 261.63f;  // Fréquence de base pour C4 (Do4)

                    // Association des boutons aux demi-tons
                    int semitoneOffset = 0;

                    // Les indices pairs représentent les notes blanches (Do, Ré, Mi, etc.)
                    // Les indices impairs représentent les notes noires (Do#, Ré#, etc.)
                    if (buttonIndex < 12) {
                        // Première octave - notes naturelles et dièses/bémols
                        switch (buttonIndex) {
                            case 0:
                                semitoneOffset = 0;
                                break; // C4 (Do4)
                            case 1:
                                semitoneOffset = 2;
                                break; // D4 (Ré4)
                            case 2:
                                semitoneOffset = 4;
                                break; // E4 (Mi4)
                            case 3:
                                semitoneOffset = 5;
                                break; // F4 (Fa4)
                            case 4:
                                semitoneOffset = 7;
                                break; // G4 (Sol4)
                            case 5:
                                semitoneOffset = 9;
                                break; // A4 (La4)
                            case 6:
                                semitoneOffset = 11;
                                break; // B4 (Si4)
                            case 7:
                                semitoneOffset = 1;
                                break; // C#4/Db4
                            case 8:
                                semitoneOffset = 3;
                                break; // D#4/Eb4
                            case 9:
                                semitoneOffset = 6;
                                break; // F#4/Gb4
                            case 10:
                                semitoneOffset = 8;
                                break; // G#4/Ab4
                            case 11:
                                semitoneOffset = 10;
                                break; // A#4/Bb4
                        }
                    } else {
                        // Deuxième octave (ajouter 12 demi-tons)
                        int idx = buttonIndex - 12;
                        switch (idx) {
                            case 0:
                                semitoneOffset = 12;
                                break; // C5 (Do5)
                            case 1:
                                semitoneOffset = 14;
                                break; // D5 (Ré5)
                            case 2:
                                semitoneOffset = 16;
                                break; // E5 (Mi5)
                            case 3:
                                semitoneOffset = 17;
                                break; // F5 (Fa5)
                            case 4:
                                semitoneOffset = 19;
                                break; // G5 (Sol5)
                            case 5:
                                semitoneOffset = 21;
                                break; // A5 (La5)
                            case 6:
                                semitoneOffset = 23;
                                break; // B5 (Si5)
                            case 7:
                                semitoneOffset = 13;
                                break; // C#5/Db5
                            case 8:
                                semitoneOffset = 15;
                                break; // D#5/Eb5
                            case 9:
                                semitoneOffset = 18;
                                break; // F#5/Gb5
                            case 10:
                                semitoneOffset = 20;
                                break; // G#5/Ab5
                            case 11:
                                semitoneOffset = 22;
                                break; // A#5/Bb5
                        }
                    }

                    // Calcul de la fréquence avec l'échelle tempérée
                    return C4_FREQUENCY * std::pow(2.0f, semitoneOffset / 12.0f);
                }
                catch (const std::exception &e) {
                    std::cerr << "SDLAudioEngine: Error parsing 8-bit note: " << pitchName << ", error: " << e.what()
                              << std::endl;
                }
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
            if (audioDevice_ == 0) {
                std::cerr << "SDLAudioEngine: Failed to open audio device: " << SDL_GetError() << std::endl;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            audioStream_ = SDL_CreateAudioStream(&deviceSpecWant, &deviceSpecWant);
            if (!SDL_ResumeAudioDevice(audioDevice_)) {
                std::cerr << "SDLAudioEngine: Failed to create audio stream: " << SDL_GetError() << std::endl;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            if (!SDL_SetAudioStreamGetCallback(audioStream_, audioCallback, this)) {
                std::cerr << "SDLAudioEngine: Failed to set stream get data callback: " << SDL_GetError() << std::endl;
                SDL_DestroyAudioStream(audioStream_);
                audioStream_ = nullptr;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            if (!SDL_BindAudioStream(audioDevice_, audioStream_)) {
                std::cerr << "SDLAudioEngine: Failed to bind audio stream to device: " << SDL_GetError() << std::endl;
                SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);
                SDL_DestroyAudioStream(audioStream_);
                audioStream_ = nullptr;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            if (!SDL_ResumeAudioDevice(audioDevice_)) {
                std::cerr << "SDLAudioEngine: Failed to resume audio device: " << SDL_GetError() << std::endl;
                SDL_BindAudioStream(audioDevice_, nullptr);
                SDL_SetAudioStreamGetCallback(audioStream_, nullptr, nullptr);
                SDL_DestroyAudioStream(audioStream_);
                audioStream_ = nullptr;
                SDL_CloseAudioDevice(audioDevice_);
                audioDevice_ = 0;
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
                return false;
            }

            isInitialized_ = true;
            std::cout << "SDLAudioEngine: Successfully initialized with callback. Sample Rate: " << deviceSpecWant.freq
                      << " Channels: " << (int) deviceSpecWant.channels << std::endl;
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
            playSound(instrumentName, note, 1.0f);
        }

        void SDLAudioEngine::playSound(const std::string &instrumentName, const Core::Note &note, float velocity) {
            if (!isInitialized_ || !activeNotesMutex_) {
                std::cerr << "SDLAudioEngine: Cannot play sound, not initialized or mutex missing." << std::endl;
                return;
            }

            velocity = std::max(0.1f, std::min(velocity, 1.0f));

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
            newActiveNote.velocity = velocity;
            newActiveNote.prevSampleLeft = 0.0f;
            newActiveNote.prevSampleRight = 0.0f;
            newActiveNote.phase = 0.0f;

            activeNotes_[noteId] = newActiveNote;

            std::cout << "SDLAudioEngine: Queued note \'" << note.pitchName << "\' (Freq: " << frequency <<
                      " Hz, Vel: "
                      << velocity
                      << ") for callback."
                      << std::endl;
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

                if (it->second.currentEnvelopeValue < 0.05f) {
                    it->second.currentEnvelopeValue = 0.05f;
                }

                std::cout << "SDLAudioEngine: Marked note \'" << it->second.pitchName
                          << "\' for release with envelope value: "
                          << it->second.currentEnvelopeValue << std::endl;
            }
            SDL_UnlockMutex(activeNotesMutex_);
        }

        void
        SDLAudioEngine::generateAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer, int numStereoSampleFrames) {
            const float twoPi = 2.0f * static_cast<float>(M_PI);
            const int totalMonoSamplesNeeded = numStereoSampleFrames * 2;

            // Ajuster les paramètres ADSR en fonction de la vélocité
            // Attaque plus courte pour les notes fortes, plus longue pour les notes douces
            float attackDuration =
                    ActiveNote::ATTACK_DURATION_SAMPLES * (1.2f - note.velocity * 0.6f); // 6ms à 14ms selon la vélocité
            float decayDuration = ActiveNote::DECAY_DURATION_SAMPLES *
                                  (0.8f + note.velocity * 0.4f);   // 80ms à 120ms selon la vélocité
            float sustainLevel = ActiveNote::SUSTAIN_LEVEL *
                                 (0.6f + note.velocity * 0.4f);             // 42% à 70% selon la vélocité

            // Calculer les paramètres de filtre basés sur la vélocité pour les harmoniques
            // Une vélocité plus élevée donne des harmoniques plus brillantes
            float harmonic_factor = note.velocity * 1.3f; // Plus de brillance pour les notes fortes

            for (int i = 0; i < totalMonoSamplesNeeded; i += 2) {
                float sampleValue = 0.0f;
                float envelope = 0.0f;

                // Calculer l'enveloppe ADSR
                if (note.isPlaying && !note.needsRelease) {
                    if (note.currentTimeInSamples < attackDuration) {
                        // Courbe d'attaque plus douce pour éviter les clics
                        // Utiliser une fonction sinusoïdale pour l'attaque (commence doucement, s'accélère puis ralentit)
                        float normalizedTime = static_cast<float>(note.currentTimeInSamples) / attackDuration;
                        note.currentEnvelopeValue = (1.0f - std::cos(normalizedTime * M_PI)) * 0.5f;
                        envelope = note.currentEnvelopeValue;
                    } else if (note.currentTimeInSamples < (attackDuration + decayDuration)) {
                        float decayProgress = (note.currentTimeInSamples - attackDuration) / decayDuration;
                        note.currentEnvelopeValue = 1.0f - (1.0f - sustainLevel) * decayProgress;
                        envelope = note.currentEnvelopeValue;
                    } else {
                        note.currentEnvelopeValue = sustainLevel;
                        envelope = note.currentEnvelopeValue;
                    }
                    note.currentTimeInSamples++;
                } else if (note.needsRelease) {
                    // Release plus long pour les notes fortes et avec une courbe plus douce
                    float releaseDuration = ActiveNote::RELEASE_DURATION_SAMPLES * (1.0f + note.velocity * 0.5f);
                    if (note.currentTimeInSamples < releaseDuration) {
                        // Courbe de relâchement douce pour éviter les clics
                        float releaseProgress = static_cast<float>(note.currentTimeInSamples) / releaseDuration;
                        // Combinaison de courbe exponentielle et sinusoïdale pour une atténuation naturelle
                        envelope = note.currentEnvelopeValue * (0.5f + 0.5f * std::cos(releaseProgress * M_PI)) *
                                   std::exp(-3.0f * releaseProgress);
                    } else {
                        envelope = 0.0f;
                        note.needsRelease = false;
                    }
                    note.currentTimeInSamples++;
                }

                if (envelope > 0.0001f) {
                    float time_in_seconds = static_cast<float>(note.currentTimeInSamples) / 44100.0f;

                    // Utiliser une approche basée sur la phase plutôt que le temps pour éviter les discontinuités
                    // Calculer l'incrément de phase en fonction de la fréquence
                    float phase_increment = twoPi * note.frequency / 44100.0f;

                    // Modèle de synthèse de piano amélioré avec plus d'harmoniques
                    // Forme d'onde fondamentale avec phase continue
                    float fundamental = std::sin(note.phase);

                    // Harmoniques avec intensités variables selon la vélocité
                    // Plus la vélocité est forte, plus les harmoniques supérieures sont présentes
                    float harmonic1 = 0.5f * std::sin(note.phase * 2.0f) * harmonic_factor;
                    float harmonic2 = 0.3f * std::sin(note.phase * 3.0f) * harmonic_factor;
                    float harmonic3 = 0.2f * std::sin(note.phase * 4.0f) * harmonic_factor * note.velocity;
                    float harmonic4 =
                            0.1f * std::sin(note.phase * 5.0f) * harmonic_factor * note.velocity * note.velocity;

                    // Effet d'attaque subtil (bruit de marteau) au début de la note
                    // Réduit pour minimiser les bruits parasites
                    float hammerNoise = 0.0f;
                    if (note.currentTimeInSamples < attackDuration * 2) {
                        // Atténuer le bruit et l'envelopper plus doucement
                        float noiseEnvelope =
                                (1.0f - std::cos(note.currentTimeInSamples / (attackDuration * 2) * M_PI)) * 0.5f;
                        hammerNoise = ((std::rand() / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) *
                                      0.03f * note.velocity * noiseEnvelope;
                    }

                    // Effet de résonance des cordes - plus prononcé pour les notes fortes
                    // Simuler les vibrations sympathiques des cordes
                    float resonance = 0.02f * note.velocity *
                                      std::sin(twoPi * note.frequency * 1.01f * time_in_seconds) *
                                      std::exp(-0.5f * time_in_seconds);

                    // Combiner tous les éléments du son
                    float oscillatorValue = fundamental + harmonic1 + harmonic2 + harmonic3 + harmonic4 +
                                            hammerNoise + resonance;

                    // Normaliser le volume
                    oscillatorValue = oscillatorValue / (1.0f + harmonic_factor * 0.5f);

                    // Appliquer l'enveloppe et la vélocité
                    sampleValue = oscillatorValue * envelope * note.velocity;

                    // Mettre à jour la phase pour le prochain échantillon
                    note.phase += phase_increment;
                    // Gardez la phase dans la plage [0, 2π] pour éviter les problèmes de précision
                    while (note.phase >= twoPi) {
                        note.phase -= twoPi;
                    }
                } else {
                    // Faire un fondu vers zéro pour les échantillons très faibles
                    // afin d'éviter les discontinuités
                    sampleValue = 0.0f;
                    // Ne pas réinitialiser la phase à 0 pour éviter les discontinuités si la note reprend
                }

                // Ajouter un léger effet de stéréo en fonction de la fréquence (graves à gauche, aigus à droite)
                float stereoPan = 0.5f + (note.frequency - 440.0f) / 2000.0f; // Centrée autour de La4 (440Hz)
                stereoPan = SDL_clamp(stereoPan, 0.2f, 0.8f); // Limiter le panning pour éviter les extrêmes

                float leftPan = 1.0f - stereoPan * 0.5f;  // 0.6 à 0.9
                float rightPan = 0.5f + stereoPan * 0.5f; // 0.6 à 0.9

                // Appliquer un fondu pour éviter les discontinuités
                // Transition douce entre échantillons précédents et actuels
                float left_sample = sampleValue * leftPan;
                float right_sample = sampleValue * rightPan;

                // Crossfade pour minimiser les clics/pops
                const float crossfade_factor = 0.1f; // Facteur de fondu entre échantillons
                left_sample = left_sample * (1.0f - crossfade_factor) + note.prevSampleLeft * crossfade_factor;
                right_sample = right_sample * (1.0f - crossfade_factor) + note.prevSampleRight * crossfade_factor;

                // Sauvegarder pour le prochain cycle
                note.prevSampleLeft = left_sample;
                note.prevSampleRight = right_sample;

                // Convertir en échantillon 16 bits avec un gain adapté à la vélocité
                // Utilisation de SDL_clamp pour éviter les saturations brutes qui créent des clics
                int16_t left_int_sample = static_cast<int16_t>(SDL_clamp(left_sample * 32767.0f * 0.8f, -32767.0f,
                                                                         32767.0f));
                int16_t right_int_sample = static_cast<int16_t>(SDL_clamp(right_sample * 32767.0f * 0.8f, -32767.0f,
                                                                          32767.0f));

                buffer[i] = left_int_sample;
                buffer[i + 1] = right_int_sample;
            }
        }

        void SDLAudioEngine::generateXylophoneAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer,
                                                         int numStereoSampleFrames) {
            const float twoPi = 2.0f * static_cast<float>(M_PI);
            const int totalMonoSamplesNeeded = numStereoSampleFrames * 2;

            // Paramètres spécifiques au xylophone, ajustés selon la vélocité
            const float base_attack_duration = 0.005f; // 5ms (très court pour le xylophone)
            const float base_decay_duration = 0.5f;    // 500ms (plus court que le piano)
            const float base_release_duration = 0.1f;  // 100ms release

            // Ajuster les paramètres en fonction de la vélocité
            const float XYLOPHONE_ATTACK_DURATION =
                    base_attack_duration * (1.2f - note.velocity * 0.4f); // Plus court pour les notes fortes
            const float XYLOPHONE_DECAY_DURATION =
                    base_decay_duration * (0.7f + note.velocity * 0.6f);   // Plus long pour les notes fortes
            const float XYLOPHONE_RELEASE_DURATION = base_release_duration * (0.8f + note.velocity * 0.4f);
            const float XYLOPHONE_SUSTAIN_LEVEL = 0.0f;    // Pas de sustain pour le xylophone

            // Convertir en échantillons
            const float XYLOPHONE_ATTACK_SAMPLES = 44100 * XYLOPHONE_ATTACK_DURATION;
            const float XYLOPHONE_DECAY_SAMPLES = 44100 * XYLOPHONE_DECAY_DURATION;
            const float XYLOPHONE_RELEASE_SAMPLES = 44100 * XYLOPHONE_RELEASE_DURATION;

            // Facteur de brillance des harmoniques basé sur la vélocité
            const float brightness_factor = 0.8f + note.velocity * 0.4f;

            for (int i = 0; i < totalMonoSamplesNeeded; i += 2) {
                float sampleValue = 0.0f;
                float envelope = 0.0f;

                if (note.isPlaying && !note.needsRelease) {
                    if (note.currentTimeInSamples < XYLOPHONE_ATTACK_SAMPLES) {
                        // Attaque très rapide pour le xylophone, mais avec une courbe plus douce
                        float normalizedTime = static_cast<float>(note.currentTimeInSamples) / XYLOPHONE_ATTACK_SAMPLES;
                        note.currentEnvelopeValue = (1.0f - std::cos(normalizedTime * M_PI)) * 0.5f;
                        envelope = note.currentEnvelopeValue;
                    } else if (note.currentTimeInSamples < (XYLOPHONE_ATTACK_SAMPLES + XYLOPHONE_DECAY_SAMPLES)) {
                        // Décroissance plus rapide pour le xylophone
                        float decayProgress =
                                (note.currentTimeInSamples - XYLOPHONE_ATTACK_SAMPLES) / XYLOPHONE_DECAY_SAMPLES;
                        // Utiliser une courbe exponentielle pour une décroissance plus naturelle
                        note.currentEnvelopeValue = std::exp(-decayProgress * 2.5f);
                        envelope = note.currentEnvelopeValue;
                    } else {
                        // Pas de sustain pour le xylophone
                        note.currentEnvelopeValue = XYLOPHONE_SUSTAIN_LEVEL;
                        envelope = note.currentEnvelopeValue;
                    }
                    note.currentTimeInSamples++;
                } else if (note.needsRelease) {
                    // Relâchement rapide mais progressif
                    if (note.currentTimeInSamples < XYLOPHONE_RELEASE_SAMPLES) {
                        float releaseProgress =
                                static_cast<float>(note.currentTimeInSamples) / XYLOPHONE_RELEASE_SAMPLES;
                        // Courbe de relâchement douce pour éviter les clics
                        envelope = note.currentEnvelopeValue * (0.5f + 0.5f * std::cos(releaseProgress * M_PI)) *
                                   std::exp(-3.0f * releaseProgress);
                    } else {
                        envelope = 0.0f;
                        note.needsRelease = false;
                    }
                    note.currentTimeInSamples++;
                }

                if (envelope > 0.0001f) {
                    // Utiliser la phase continue pour éviter les discontinuités
                    float phase_increment = twoPi * note.frequency / 44100.0f;

                    // Mélange des harmoniques pour obtenir un son plus brillant de xylophone
                    float fundamental = std::sin(note.phase);
                    float harmonic1 = 0.5f * std::sin(note.phase * 2.0f) * brightness_factor;
                    float harmonic2 = 0.33f * std::sin(note.phase * 4.0f) * brightness_factor;
                    float harmonic3 = 0.25f * std::sin(note.phase * 6.0f) * brightness_factor * note.velocity;

                    // Effet de résonance métallique caractéristique du xylophone
                    float time_in_seconds = static_cast<float>(note.currentTimeInSamples) / 44100.0f;
                    float resonance = 0.1f * std::sin(twoPi * note.frequency * 12.0f * time_in_seconds) *
                                      std::exp(-time_in_seconds * 8.0f) * note.velocity; // Décroît rapidement

                    // Léger bruit d'impact au début (mallet hit), modulé par la vélocité
                    float impactNoise = 0.0f;
                    if (note.currentTimeInSamples < XYLOPHONE_ATTACK_SAMPLES * 2) {
                        // Atténuer le bruit et l'envelopper plus doucement
                        float noiseEnvelope =
                                (1.0f - std::cos(note.currentTimeInSamples / (XYLOPHONE_ATTACK_SAMPLES * 2) * M_PI)) *
                                0.5f;
                        impactNoise = ((std::rand() / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) *
                                      0.1f * note.velocity * noiseEnvelope;
                    }

                    // Mélange avec plus d'harmoniques hautes pour le son brillant du xylophone
                    float oscillatorValue = fundamental + harmonic1 + harmonic2 + harmonic3 + resonance + impactNoise;
                    oscillatorValue = oscillatorValue / (1.5f + brightness_factor * 0.5f); // Normaliser le volume

                    // Appliquer l'enveloppe et la vélocité
                    sampleValue = oscillatorValue * envelope * note.velocity;

                    // Mettre à jour la phase pour le prochain échantillon
                    note.phase += phase_increment;
                    // Gardez la phase dans la plage [0, 2π] pour éviter les problèmes de précision
                    while (note.phase >= twoPi) {
                        note.phase -= twoPi;
                    }
                } else {
                    sampleValue = 0.0f;
                    // Ne pas réinitialiser la phase à 0 pour éviter les discontinuités si la note reprend
                }

                // Ajouter un léger effet stéréo (panning) basé sur la fréquence
                // Les notes basses plus à gauche, les notes hautes plus à droite
                float normalizedFreq = (note.frequency - 200.0f) / 800.0f;  // Normaliser entre 0-1
                normalizedFreq = SDL_clamp(normalizedFreq, 0.0f, 1.0f);

                // Calculer les volumes des canaux gauche et droit
                float leftVolume = 1.0f - (normalizedFreq * 0.5f);   // 1.0 à 0.5
                float rightVolume = 0.5f + (normalizedFreq * 0.5f);  // 0.5 à 1.0

                // Appliquer un fondu pour éviter les discontinuités
                float left_sample = sampleValue * leftVolume;
                float right_sample = sampleValue * rightVolume;

                // Crossfade pour minimiser les clics/pops
                const float crossfade_factor = 0.08f; // Légèrement moins de crossfade que le piano (son plus percussif)
                left_sample = left_sample * (1.0f - crossfade_factor) + note.prevSampleLeft * crossfade_factor;
                right_sample = right_sample * (1.0f - crossfade_factor) + note.prevSampleRight * crossfade_factor;

                // Sauvegarder pour le prochain cycle
                note.prevSampleLeft = left_sample;
                note.prevSampleRight = right_sample;

                // Convertir en échantillon 16 bits avec un gain adapté à la vélocité
                int16_t left_int_sample = static_cast<int16_t>(SDL_clamp(left_sample * 32767.0f * 0.85f, -32767.0f,
                                                                         32767.0f));
                int16_t right_int_sample = static_cast<int16_t>(SDL_clamp(right_sample * 32767.0f * 0.85f, -32767.0f,
                                                                          32767.0f));

                buffer[i] = left_int_sample;
                buffer[i + 1] = right_int_sample;
            }
        }

        void SDLAudioEngine::generate8BitAudioChunk(ActiveNote &note, std::vector<int16_t> &buffer,
                                                    int numStereoSampleFrames) {
            const float twoPi = 2.0f * static_cast<float>(M_PI);
            const int totalMonoSamplesNeeded = numStereoSampleFrames * 2;

            // Paramètres spécifiques au son 8-bit, ajustés selon la vélocité
            const float base_attack = 0.01f;   // 10ms
            const float base_decay = 0.05f;    // 50ms
            const float base_release = 0.05f;  // 50ms

            // Ajuster les paramètres en fonction de la vélocité
            const float CHIPTUNE_ATTACK_DURATION =
                    base_attack * (1.1f - note.velocity * 0.5f); // Plus court pour les notes fortes
            const float CHIPTUNE_DECAY_DURATION = base_decay * (0.9f + note.velocity * 0.2f);
            const float CHIPTUNE_RELEASE_DURATION = base_release * (0.8f + note.velocity * 0.4f);
            const float CHIPTUNE_SUSTAIN_LEVEL = 0.6f + (note.velocity * 0.2f);     // 60-80% de volume selon vélocité

            // Convertir en échantillons
            const float CHIPTUNE_ATTACK_SAMPLES = 44100 * CHIPTUNE_ATTACK_DURATION;
            const float CHIPTUNE_DECAY_SAMPLES = 44100 * CHIPTUNE_DECAY_DURATION;
            const float CHIPTUNE_RELEASE_SAMPLES = 44100 * CHIPTUNE_RELEASE_DURATION;

            // Facteur de quantification pour le son 8-bit, peut varier avec la vélocité
            // Pour les notes fortes, moins de bits = son plus saturé et agressif
            const int BIT_DEPTH = std::max(3, static_cast<int>(6 - note.velocity *
                                                                   2));  // Entre 3 et 6 bits selon la vélocité
            const float QUANTIZE_LEVELS = (1 << BIT_DEPTH) - 1;  // Niveaux de quantification

            for (int i = 0; i < totalMonoSamplesNeeded; i += 2) {
                float sampleValue = 0.0f;
                float envelope = 0.0f;

                // Calcul de l'enveloppe ADSR avec courbes douces
                if (note.isPlaying && !note.needsRelease) {
                    if (note.currentTimeInSamples < CHIPTUNE_ATTACK_SAMPLES) {
                        // Attaque douce pour éviter les clics
                        float normalizedTime = static_cast<float>(note.currentTimeInSamples) / CHIPTUNE_ATTACK_SAMPLES;
                        note.currentEnvelopeValue = (1.0f - std::cos(normalizedTime * M_PI)) * 0.5f;
                        envelope = note.currentEnvelopeValue;
                    } else if (note.currentTimeInSamples < (CHIPTUNE_ATTACK_SAMPLES + CHIPTUNE_DECAY_SAMPLES)) {
                        float decayProgress =
                                (note.currentTimeInSamples - CHIPTUNE_ATTACK_SAMPLES) / CHIPTUNE_DECAY_SAMPLES;
                        note.currentEnvelopeValue = 1.0f - (1.0f - CHIPTUNE_SUSTAIN_LEVEL) * decayProgress;
                        envelope = note.currentEnvelopeValue;
                    } else {
                        note.currentEnvelopeValue = CHIPTUNE_SUSTAIN_LEVEL;
                        envelope = note.currentEnvelopeValue;
                    }
                    note.currentTimeInSamples++;
                } else if (note.needsRelease) {
                    if (note.currentTimeInSamples < CHIPTUNE_RELEASE_SAMPLES) {
                        float releaseProgress =
                                static_cast<float>(note.currentTimeInSamples) / CHIPTUNE_RELEASE_SAMPLES;
                        // Transition douce vers le silence pour éviter les pops
                        envelope = note.currentEnvelopeValue * (0.5f + 0.5f * std::cos(releaseProgress * M_PI)) *
                                   std::exp(-2.0f * releaseProgress);
                    } else {
                        envelope = 0.0f;
                        note.needsRelease = false;
                    }
                    note.currentTimeInSamples++;
                }

                if (envelope > 0.0001f) {
                    float time_in_seconds = static_cast<float>(note.currentTimeInSamples) / 44100.0f;

                    // Utiliser la phase pour formes d'ondes continues
                    float phase_increment = twoPi * note.frequency / 44100.0f;

                    // Générer une forme d'onde carrée (caractéristique du son 8-bit)
                    // En utilisant la phase pour continuité
                    float squareWave = std::sin(note.phase) > 0.0f ? 1.0f : -1.0f;

                    // Ajouter une légère modulation (vibrato) pour les effets spéciaux
                    // L'intensité du vibrato dépend de la vélocité
                    float vibrato = std::sin(twoPi * 7.0f * time_in_seconds) * 0.05f * note.velocity;

                    // Ajouter de la distorsion qui varie avec la vélocité
                    float distortion = std::sin(twoPi * note.frequency * 2.0f * time_in_seconds) *
                                       (0.1f + note.velocity * 0.2f);

                    // Ajouter du "bitcrush noise" - caractéristique des consoles rétro
                    float noise = 0.0f;
                    if (note.velocity > 0.7f) { // Uniquement pour les vélocités fortes
                        noise = (std::rand() / static_cast<float>(RAND_MAX) * 2.0f - 1.0f) *
                                0.05f * (note.velocity - 0.7f) / 0.3f;
                    }

                    // Mélanger les effets avec des poids qui varient selon la vélocité
                    float oscillatorValue = squareWave + vibrato + distortion + noise;

                    // Appliquer la quantification (effet de réduction de bit)
                    // Plus la vélocité est forte, plus la quantification est prononcée
                    oscillatorValue = std::floor(oscillatorValue * QUANTIZE_LEVELS + 0.5f) / QUANTIZE_LEVELS;

                    // Appliquer l'enveloppe et la vélocité
                    sampleValue = oscillatorValue * envelope * (0.7f + note.velocity * 0.3f);

                    // Mettre à jour la phase pour le prochain échantillon
                    note.phase += phase_increment;
                    // Maintenir la phase dans la plage [0, 2π]
                    while (note.phase >= twoPi) {
                        note.phase -= twoPi;
                    }
                } else {
                    sampleValue = 0.0f;
                    // Ne pas réinitialiser la phase pour éviter les discontinuités
                }

                // Appliquer un effet stéréo léger pour les sons 8-bit
                // Moduler le panoramique en fonction de la note et de la vélocité
                float stereoPan = 0.5f + ((note.frequency - 440.0f) / 1500.0f) * note.velocity * 0.5f;
                stereoPan = SDL_clamp(stereoPan, 0.3f, 0.7f); // Limiter l'effet stéréo

                float leftVolume = 1.0f - (stereoPan * 0.3f);  // 0.79 à 0.91
                float rightVolume = 0.7f + (stereoPan * 0.3f); // 0.79 à 0.91

                // Appliquer un fondu pour éviter les discontinuités
                float left_sample = sampleValue * leftVolume;
                float right_sample = sampleValue * rightVolume;

                // Crossfade très léger pour les sons 8-bit (pour préserver la netteté)
                const float crossfade_factor = 0.05f;
                left_sample = left_sample * (1.0f - crossfade_factor) + note.prevSampleLeft * crossfade_factor;
                right_sample = right_sample * (1.0f - crossfade_factor) + note.prevSampleRight * crossfade_factor;

                // Sauvegarder pour le prochain cycle
                note.prevSampleLeft = left_sample;
                note.prevSampleRight = right_sample;

                // Convertir en échantillon 16 bits avec un gain adapté
                int16_t left_int_sample = static_cast<int16_t>(SDL_clamp(left_sample * 32767.0f * 0.75f, -32767.0f,
                                                                         32767.0f));
                int16_t right_int_sample = static_cast<int16_t>(SDL_clamp(right_sample * 32767.0f * 0.75f, -32767.0f,
                                                                          32767.0f));

                buffer[i] = left_int_sample;
                buffer[i + 1] = right_int_sample;
            }
        }

        void SDLAudioEngine::audioCallback(void *userdata, SDL_AudioStream *sdlStream, int additional_amount,
                                           int total_amount) {
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

                    if (note.instrumentName == "Xylophone") {
                        engine->generateXylophoneAudioChunk(note, noteChunkBuffer, stereoSampleFramesNeeded);
                    } else if (note.instrumentName == "8BitConsole") {
                        engine->generate8BitAudioChunk(note, noteChunkBuffer, stereoSampleFramesNeeded);
                    } else {
                        engine->generateAudioChunk(note, noteChunkBuffer, stereoSampleFramesNeeded);
                    }

                    for (size_t i = 0; i < mixBuffer.size(); ++i) {
                        float mixed_sample = static_cast<float>(mixBuffer[i]) + static_cast<float>(noteChunkBuffer[i]);
                        mixBuffer[i] = static_cast<int16_t>(SDL_clamp(mixed_sample, -32768.0f, 32767.0f));
                    }

                    if (!note.isPlaying && !note.needsRelease) {
                        notesToRemove.push_back(pair.first);
                    }
                }
            }

            for (const auto &noteId: notesToRemove) {
                engine->activeNotes_.erase(noteId);
                std::cout << "SDLAudioEngine: Removed note \'" << noteId << "\' from active list after release." <<
                          std::endl;
            }

            SDL_UnlockMutex(engine->activeNotesMutex_);

            if (SDL_PutAudioStreamData(sdlStream, mixBuffer.data(), additional_amount) < 0) {
                std::cerr << "SDLAudioEngine::audioCallback: Failed to put audio stream data: " << SDL_GetError()
                          << std::endl;
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

            for (const auto &item: notesToActuallyStop) {
                std::cout << "SDLAudioEngine: Note \'" << item.second.pitchName
                          << "\' on instrument \'" << item.first
                          << "\' auto-stopped by cleanup after " << maxDurationMs << "ms" << std::endl;
                stopSound(item.first, item.second);
            }
        }

    } // namespace Audio
} // namespace MusicApp
