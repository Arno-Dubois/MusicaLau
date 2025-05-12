#ifndef MUSICAPP_AUDIO_SDLAUDIOENGINE_H
#define MUSICAPP_AUDIO_SDLAUDIOENGINE_H

#include "AudioEngine.h"
#include "../Core/Note.h"
#include <string>
#include <vector>
#include <map>
#include <cmath> // For M_PI and sin

// Include SDL3 header
// Make sure your include paths are set correctly in your CMakeLists.txt
// For example, if SDL3 is installed in a standard location, <SDL3/SDL.h> might work.
// If you have it in a custom location, ensure CMake passes that path to the compiler.
#include <SDL3/SDL.h>

namespace MusicApp {
namespace Audio {

class SDLAudioEngine : public AudioEngine {
public:
    SDLAudioEngine();
    ~SDLAudioEngine() override;

    bool init() override;
    void shutdown() override;
    void playSound(const std::string& instrumentName, const Core::Note& note) override;

private:
    // Helper to convert note pitch name to frequency
    // A more robust implementation would parse flats/sharps and octaves correctly.
    float getFrequencyForNote(const std::string& pitchName) const;

    SDL_AudioDeviceID audioDevice_;
    SDL_AudioSpec audioSpec_; // Stores the obtained audio spec
    bool isInitialized_;

    // Basic note to frequency map (Hz) for demonstration
    // This should ideally be more comprehensive or calculated.
    static const std::map<std::string, float> noteFrequencies_;
};

} // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_SDLAUDIOENGINE_H