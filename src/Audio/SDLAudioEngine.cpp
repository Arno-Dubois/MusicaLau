#include "SDLAudioEngine.h"
#include <iostream> // For std::cout, std::cerr
#include <vector>
#include <cmath>    // For M_PI, sin
#include <algorithm> // for std::max, std::min

// Define M_PI if not already defined (often in <cmath> but can be non-standard)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MusicApp {
namespace Audio {


// Initialize static member with the extended frequency list
const std::map<std::string, float> SDLAudioEngine::noteFrequencies_ = {
    {"B0", 31.0f}, {"C1", 33.0f}, {"C#1", 35.0f}, {"Db1", 35.0f},
    {"D1", 37.0f}, {"D#1", 39.0f}, {"Eb1", 39.0f}, {"E1", 41.0f},
    {"F1", 44.0f}, {"F#1", 46.0f}, {"Gb1", 46.0f}, {"G1", 49.0f},
    {"G#1", 52.0f}, {"Ab1", 52.0f}, {"A1", 55.0f}, {"A#1", 58.0f}, {"Bb1", 58.0f},
    {"B1", 62.0f}, {"C2", 65.0f}, {"C#2", 69.0f}, {"Db2", 69.0f},
    {"D2", 73.0f}, {"D#2", 78.0f}, {"Eb2", 78.0f}, {"E2", 82.0f},
    {"F2", 87.0f}, {"F#2", 93.0f}, {"Gb2", 93.0f}, {"G2", 98.0f},
    {"G#2", 104.0f}, {"Ab2", 104.0f}, {"A2", 110.0f}, {"A#2", 117.0f}, {"Bb2", 117.0f},
    {"B2", 123.0f}, {"C3", 131.0f}, {"C#3", 139.0f}, {"Db3", 139.0f},
    {"D3", 147.0f}, {"D#3", 156.0f}, {"Eb3", 156.0f}, {"E3", 165.0f},
    {"F3", 175.0f}, {"F#3", 185.0f}, {"Gb3", 185.0f}, {"G3", 196.0f},
    {"G#3", 208.0f}, {"Ab3", 208.0f}, {"A3", 220.0f}, {"A#3", 233.0f}, {"Bb3", 233.0f},
    {"B3", 247.0f}, {"C4", 262.0f}, {"C#4", 277.0f}, {"Db4", 277.0f}, // Adjusted C4 to 262 from 261.63 for consistency
    {"D4", 294.0f}, {"D#4", 311.0f}, {"Eb4", 311.0f}, {"E4", 330.0f},
    {"F4", 349.0f}, {"F#4", 370.0f}, {"Gb4", 370.0f}, {"G4", 392.0f},
    {"G#4", 415.0f}, {"Ab4", 415.0f}, {"A4", 440.0f}, {"A#4", 466.0f}, {"Bb4", 466.0f},
    {"B4", 494.0f}, {"C5", 523.0f}, {"C#5", 554.0f}, {"Db5", 554.0f}, // Adjusted C5 to 523 from 523.25
    {"D5", 587.0f}, {"D#5", 622.0f}, {"Eb5", 622.0f}, {"E5", 659.0f},
    {"F5", 698.0f}, {"F#5", 740.0f}, {"Gb5", 740.0f}, {"G5", 784.0f},
    {"G#5", 831.0f}, {"Ab5", 831.0f}, {"A5", 880.0f}, {"A#5", 932.0f}, {"Bb5", 932.0f},
    {"B5", 988.0f}, {"C6", 1047.0f}, {"C#6", 1109.0f}, {"Db6", 1109.0f},
    {"D6", 1175.0f}, {"D#6", 1245.0f}, {"Eb6", 1245.0f}, {"E6", 1319.0f},
    {"F6", 1397.0f}, {"F#6", 1480.0f}, {"Gb6", 1480.0f}, {"G6", 1568.0f},
    {"G#6", 1661.0f}, {"Ab6", 1661.0f}, {"A6", 1760.0f}, {"A#6", 1865.0f}, {"Bb6", 1865.0f},
    {"B6", 1976.0f}, {"C7", 2093.0f}, {"C#7", 2217.0f}, {"Db7", 2217.0f},
    {"D7", 2349.0f}, {"D#7", 2489.0f}, {"Eb7", 2489.0f}, {"E7", 2637.0f},
    {"F7", 2794.0f}, {"F#7", 2960.0f}, {"Gb7", 2960.0f}, {"G7", 3136.0f},
    {"G#7", 3322.0f}, {"Ab7", 3322.0f}, {"A7", 3520.0f}, {"A#7", 3729.0f}, {"Bb7", 3729.0f},
    {"B7", 3951.0f}, {"C8", 4186.0f}, {"C#8", 4435.0f}, {"Db8", 4435.0f},
    {"D8", 4699.0f}, {"D#8", 4978.0f}, {"Eb8", 4978.0f},
    {"0", 0.0f} // Explicitly define "0" as 0 Hz for silence
};

SDLAudioEngine::SDLAudioEngine()
    : audioDevice_(0), isInitialized_(false) {
    SDL_zero(audioSpec_); // Initialize audioSpec_ member
    std::cout << "SDLAudioEngine: Constructor called." << std::endl;
}

SDLAudioEngine::~SDLAudioEngine() {
    std::cout << "SDLAudioEngine: Destructor called." << std::endl;
    if (isInitialized_) {
        shutdown();
    }
}

float SDLAudioEngine::getFrequencyForNote(const std::string& pitchName) const {
    if (pitchName == "0" || pitchName == "Unknown") { // Handle common silence indicators
        return 0.0f;
    }
    auto it = noteFrequencies_.find(pitchName);
    if (it != noteFrequencies_.end()) {
        return it->second;
    }
    try {
        size_t processedChars = 0;
        float freq = std::stof(pitchName, &processedChars);
        if (processedChars == pitchName.length()) {
            return freq; // Allow direct frequency input
        }
    } catch (const std::invalid_argument&) {
        // Not a number, fall through
    } catch (const std::out_of_range&) {
        // Number out of range for float, fall through
    }
    std::cerr << "SDLAudioEngine: Frequency for note '" << pitchName << "' not found. Defaulting to 0 Hz (silence)." << std::endl;
    return 0.0f;
}

bool SDLAudioEngine::init() {
    std::cout << "SDLAudioEngine: Initializing..." << std::endl;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) { // Use < 0 for SDL error checks
        std::cerr << "SDLAudioEngine: Failed to initialize SDL_AUDIO: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_AudioSpec wantSpec;
    SDL_zero(wantSpec);

    wantSpec.freq = 44100;
    wantSpec.format = SDL_AUDIO_F32SYS; // SDL_AUDIO_F32 was in your snippet, ensure this matches your SDL3 version. SDL_AUDIO_F32SYS is common.
    wantSpec.channels = 2;
    // wantSpec.samples: For SDL_OpenAudioDeviceStream, samples is not configured in wantSpec directly,
    // but rather influences how much data you should put into the stream at a time.
    // The stream has its own internal buffering. Let's use a reasonable buffer size for playSound.

    // Copy `wantSpec` to our member `audioSpec_` so it's available for calculations.
    // The actual stream might have slightly different properties, but for generation, we use what we requested.
    audioSpec_ = wantSpec;

    audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec_, nullptr, nullptr);

    if (!audioStream) {
        std::cerr << "SDLAudioEngine: Failed to open audio stream: " << SDL_GetError() << std::endl;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    audioDevice_ = SDL_GetAudioStreamDevice(audioStream); // Get the device ID associated with the stream
    if (audioDevice_ == 0) {
        // Should not happen if stream opened, but good check
        std::cerr << "SDLAudioEngine: Failed to get audio device from stream: " << SDL_GetError() << std::endl;
        SDL_CloseAudioStream(audioStream);
        audioStream = nullptr;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    std::cout << "SDLAudioEngine: Audio stream opened." << std::endl;
    std::cout << "  Frequency: " << audioSpec_.freq << " Hz" << std::endl;
    std::cout << "  Format:    " << SDL_GetAudioFormatName(audioSpec_.format) << " (Value: " << audioSpec_.format << ")"
            << std::endl;
    std::cout << "  Channels:  " << (int) audioSpec_.channels << std::endl;
    // SDL_AudioSpec for streams doesn't have a 'samples' field that reflects a hardware buffer size in the same way as SDL_OpenAudioDevice's obtained spec.
    // std::cout << "  Samples (Device): " << audioSpec_.samples << std::endl; // This would be from obtainedSpec with SDL_OpenAudioDevice

    if (SDL_ResumeAudioStreamDevice(audioStream) != 0) {
        // Resume the stream's device
        std::cerr << "SDLAudioEngine: Failed to resume audio device for stream: " << SDL_GetError() << std::endl;
        SDL_CloseAudioStream(audioStream);
        audioStream = nullptr;
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
        if (audioStream) {
            SDL_PauseAudioStreamDevice(audioStream); // Pause the device associated with the stream
            SDL_ClearAudioStream(audioStream); // Clear any pending data
            SDL_CloseAudioStream(audioStream);
            audioStream = nullptr;
            audioDevice_ = 0; // Reset device ID
        }
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        isInitialized_ = false;
        std::cout << "SDLAudioEngine: Shutdown complete." << std::endl;
    } else {
        std::cout << "SDLAudioEngine: Already shut down or was not initialized." << std::endl;
    }
}

void SDLAudioEngine::playSound(const std::string &instrumentName, const Core::Note &note) {
    if (!isInitialized_ || !audioStream) {
        std::cerr << "SDLAudioEngine: Cannot play sound, audio engine not initialized or stream not open." << std::endl;
        return;
    }

    float frequency = getFrequencyForNote(note.pitchName);
    // For silence (freq = 0 or duration = 0), we don't need to generate/queue audio.
    // The delay for silence will be handled by the MusicController.
    if (frequency <= 0.0f || note.durationSeconds <= 0.0f) {
        if (note.durationSeconds > 0.0f) {
            // Only print if it was supposed to be a sound
            std::cout << "SDLAudioEngine: Playing silence for note '" << note.pitchName
                    << "' for " << note.durationSeconds << "s." << std::endl;
        }
        return;
    }

    std::cout << "SDLAudioEngine: Playing note '" << note.pitchName
            << "' (Freq: " << frequency << " Hz, Dur: " << note.durationSeconds
            << "s) with instrument '" << instrumentName << "'." << std::endl;

    const float amplitude = 0.25f;

    int numFrames = static_cast<int>(static_cast<float>(audioSpec_.freq) * note.durationSeconds);
    int totalSamples = numFrames * audioSpec_.channels;

    if (totalSamples == 0) {
        // Avoid generating empty buffer
        return;
    }

    std::vector<float> buffer(totalSamples);

    for (int i = 0; i < numFrames; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(audioSpec_.freq);
        float sampleValue = amplitude * std::sin(2.0f * M_PI * frequency * t);

        float envelope = 1.0f;
        // Apply a simple linear fade-out for the last 10% or last 50ms, whichever is shorter, to reduce clicks.
        float fadeOutDuration = std::min(note.durationSeconds * 0.10f, 0.05f);
        if (note.durationSeconds > fadeOutDuration) {
            // ensure fadeOutStartTime is valid
            float fadeOutStartTime = note.durationSeconds - fadeOutDuration;
            if (t > fadeOutStartTime) {
                envelope = (note.durationSeconds - t) / fadeOutDuration;
                envelope = std::max(0.0f, std::min(1.0f, envelope));
            }
        } else {
            // Very short note, maybe a quick ramp down
            envelope = (note.durationSeconds - t) / note.durationSeconds;
            envelope = std::max(0.0f, std::min(1.0f, envelope));
        }
        sampleValue *= envelope;

        for (int ch = 0; ch < audioSpec_.channels; ++ch) {
            buffer[i * audioSpec_.channels + ch] = sampleValue;
        }
    }

    // The SDL_GetAudioStreamQueued check from your snippet was:
    // const int minimum_audio = (8000 * sizeof (float)) / 2; 
    // if (SDL_GetAudioStreamQueued(stream) < minimum_audio) { ... }
    // This implies not overfilling. For now, we put the whole note.
    // If issues arise (like audio cutting off), this might need to be revisited with smaller chunks.

    if (SDL_PutAudioStreamData(audioStream, buffer.data(), totalSamples * sizeof(float)) != 0) {
        std::cerr << "SDLAudioEngine: Failed to put audio data to stream: " << SDL_GetError() << std::endl;
    }
}
} // namespace Audio
} // namespace MusicApp
