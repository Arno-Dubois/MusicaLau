#include "../../include/Audio/SDLAudioEngine.h"
#include <iostream> // For std::cout, std::cerr
#include <vector>
#include <cmath>    // For M_PI, sin
#include <cstdint>

// Define M_PI if not already defined (often in <cmath> but can be non-standard)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MusicApp {
    namespace Audio {
        static SDL_AudioStream *stream = NULL;

        enum class Instrument {
            SINE,
            SQUARE,
            SAWTOOTH
        };
        // Initialize static member
        const std::map<std::string, float> SDLAudioEngine::noteFrequencies_ = {
            // Octave 4
            {"C4", 261.63f}, {"C#4", 277.18f}, {"Db4", 277.18f},
            {"D4", 293.66f}, {"D#4", 311.13f}, {"Eb4", 311.13f},
            {"E4", 329.63f},
            {"F4", 349.23f}, {"F#4", 369.99f}, {"Gb4", 369.99f},
            {"G4", 392.00f}, {"G#4", 415.30f}, {"Ab4", 415.30f},
            {"A4", 440.00f}, // A440
            {"A#4", 466.16f}, {"Bb4", 466.16f},
            {"B4", 493.88f},
            // Octave 5
            {"C5", 523.25f}, {"D5", 587.33f}, {"E5", 659.25f}, {"F5", 698.46f},
            {"G5", 783.99f}, {"A5", 880.00f}, {"B5", 987.77f}
            // ...add more notes as needed
        };

        SDLAudioEngine::SDLAudioEngine()
        : audioDevice_(0), isInitialized_(false) {
            // SDL_AudioSpec is initialized in init()
            std::cout << "SDLAudioEngine: Constructor called." << std::endl;
            // In a real SDL app, you wouldn't typically init SDL in the constructor directly,
            // but rather in the init() method to allow for error checking and controlled startup.
        }

        SDLAudioEngine::~SDLAudioEngine() {
            std::cout << "SDLAudioEngine: Destructor called." << std::endl;
            // Ensure shutdown is called if it hasn't been already.
            if (isInitialized_) {
                shutdown();
            }
        }

        float SDLAudioEngine::getFrequencyForNote(const std::string& pitchName) const {
            auto it = noteFrequencies_.find(pitchName);
            if (it != noteFrequencies_.end()) {
                return it->second;
            }
            // A simple way to handle unknown notes: try to parse if it's just a number (frequency)
            try {
                size_t processedChars = 0;
                float freq = std::stof(pitchName, &processedChars);
                // Check if the whole string was consumed by stof
                if (processedChars == pitchName.length()) {
                    return freq;
                }
            } catch (const std::invalid_argument& ia) {
                // Not a number, fall through
            } catch (const std::out_of_range& oor) {
                // Number out of range for float, fall through
            }
            std::cerr << "SDLAudioEngine: Frequency for note '" << pitchName << "' not found. Defaulting to 0 Hz (silence)." << std::endl;
            return 0.0f; // Return 0 or throw an error for unknown notes
        }

        bool SDLAudioEngine::init() {
            std::cout << "SDLAudioEngine: Initializing..." << std::endl;
            if (SDL_Init(SDL_INIT_AUDIO) == false) {
                std::cerr << "SDLAudioEngine: Failed to initialize SDL_AUDIO: " << SDL_GetError() << std::endl;
                return false;
            }

            SDL_AudioSpec want;
            SDL_zero(want); // Initialize want structure to zeros

            want.freq = 44100;          // Desired frequency (samples per second)
            want.format = SDL_AUDIO_S16; // Desired format (float, system endianness)
            want.channels = 2;          // Number of channels (1=mono, 2=stereo)

            // SDL_OpenAudioDevice opens a specific audio device.
            // Pass nullptr for the device name to use the most reasonable default.
            // The last parameter (allowed_changes) can be 0 if you want SDL to fail if it can't get exactly what you want.
            // Or use SDL_AUDIO_ALLOW_ANY_CHANGE, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE, etc.
            stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, NULL, NULL);
            audioDevice_ = SDL_GetAudioStreamDevice(stream);

            if (audioDevice_ == 0) {
                std::cerr << "SDLAudioEngine: Failed to open audio device: " << SDL_GetError() << std::endl;
                SDL_QuitSubSystem(SDL_INIT_AUDIO); // Clean up SDL_Init(SDL_INIT_AUDIO)
                return false;
            }

            // Check if we got what we wanted (or something compatible)
            std::cout << "SDLAudioEngine: Audio device opened." << std::endl;
            std::cout << "  Frequency: " << audioSpec_.freq << " Hz" << std::endl;
            std::cout << "  Format:    " << SDL_GetAudioFormatName(audioSpec_.format) << " (Value: " << audioSpec_.format << ")" << std::endl;
            std::cout << "  Channels:  " << (int)audioSpec_.channels << std::endl;

            SDL_ResumeAudioStreamDevice(stream);


            isInitialized_ = true;
            std::cout << "SDLAudioEngine: Successfully initialized." << std::endl;
    return true;
        }

        void SDLAudioEngine::shutdown() {
            std::cout << "SDLAudioEngine: Shutting down..." << std::endl;
            if (isInitialized_) {
                if (audioDevice_ != 0) {
                    SDL_PauseAudioDevice(audioDevice_); // Pause before closing
                    SDL_CloseAudioDevice(audioDevice_);
                    audioDevice_ = 0;
                }
                SDL_QuitSubSystem(SDL_INIT_AUDIO); // Quit the audio subsystem initiated with SDL_Init
                // If other SDL subsystems were initialized with SDL_Init(SDL_INIT_EVERYTHING),
                // then use SDL_Quit() here instead of SDL_QuitSubSystem.

                isInitialized_ = false;
                std::cout << "SDLAudioEngine: Shutdown complete (placeholder)." << std::endl;
            } else {
                std::cout << "SDLAudioEngine: Shutdown complete." << std::endl;
    }
        }

        // Function to generate a waveform sample for a given instrument using int16_t
        std::vector<int16_t> generateWaveform(Instrument ins, float frequency, float durationSeconds, unsigned int sampleRate = 44100) {
            const std::size_t sampleCount = static_cast<std::size_t>(sampleRate * durationSeconds);
            std::vector<int16_t> samples(sampleCount);
            const float twoPi = 6.283185307179586476925286766559f;

            for (std::size_t i = 0; i < sampleCount; ++i) {
                float time = static_cast<float>(i) / sampleRate;  // time in seconds
                float value = 0.0f;

                // Calculate phase for current sample
                float phase = twoPi * frequency * time;

                // Select waveform based on the instrument type
                switch (ins) {
                    case Instrument::SINE:
                        value = std::sin(phase);
                        break;
                    case Instrument::SQUARE:
                        value = (std::sin(phase) >= 0) ? 1.0f : -1.0f;
                        break;
                    case Instrument::SAWTOOTH:
                        // Sawtooth ranges from -1 to 1 linearly
                        value = 2.0f * (time * frequency - std::floor(0.5f + time * frequency));
                        break;
                    default:
                        value = std::sin(phase);
                        break;
                }

                // Scale to 16-bit integer range
                samples[i] = static_cast<int16_t>(value * 32767);
            }
            return samples;
        }

        void SDLAudioEngine::playSound(const std::string &instrumentName, const Core::Note &note) {
            if (!isInitialized_ || audioDevice_ == 0) {
                std::cerr << "SDLAudioEngine: Cannot play sound, audio engine not initialized or device not open." << std::endl;
                return;
            }
            float frequency = getFrequencyForNote(note.pitchName);
            if (frequency <= 0.0f) { // 0 Hz or negative is silence or error
                std::cerr << "SDLAudioEngine: Invalid frequency for note '" << note.pitchName << "'. Cannot play sound." << std::endl;
                return;
            }

            std::cout << "SDLAudioEngine: Playing note '" << note.pitchName
            << "' (Freq: " << frequency << " Hz) with instrument '" << instrumentName << "'." << std::endl;

            Instrument instrument;
            if (instrumentName == "Piano") {
                instrument = Instrument::SINE;
            } else if (instrumentName == "Xylophone") {
                instrument = Instrument::SINE;
            } else if (instrumentName == "Guitar") {
                instrument = Instrument::SAWTOOTH;
            } else {
                instrument = Instrument::SINE;
            }

            std::cout << "generating wave";
            // Generate waveform samples
            std::vector<int16_t> samples = generateWaveform(instrument, frequency, 1);
            std::cout << "wave :check: puting stream";

            // Queue the audio data
            int byteSize = samples.size() * sizeof(int16_t);
            if (SDL_PutAudioStreamData(stream, samples.data(), byteSize) == false) {
                std::cerr << "SDL_QueueAudio error: " << SDL_GetError() << std::endl;
            }

            // Wait for the duration of the audio plus a little extra time
            SDL_Delay(static_cast<Uint32>(1 * 1000) + 100);
        }
    } // namespace Audio
} // namespace MusicApp
