#ifndef MUSICAPP_AUDIO_AUDIOENGINE_H
#define MUSICAPP_AUDIO_AUDIOENGINE_H

#include "../Core/Note.h"
// Or more likely, specific sound parameters derived from Note
#include <string> // For instrument names or sound identifiers

namespace MusicApp {
namespace Audio {

/**
 * @brief Abstract base class for an audio engine.
 *
 * This interface defines the core functionalities required for audio playback,
 * such as initializing the audio system, playing a sound (representing a note
 * from an instrument), and shutting down the audio system.
 */
class AudioEngine {
public:
    virtual ~AudioEngine() = default;

    /**
     * @brief Initializes the audio engine and any underlying audio systems.
     * @return True if initialization was successful, false otherwise.
     */
    virtual bool init() = 0;

    /**
     * @brief Shuts down the audio engine and releases any resources.
     */
    virtual void shutdown() = 0;

    /**
     * @brief Plays a sound associated with a note from a specific instrument.
     *
     * The exact mechanism of how a "note" translates to sound is an
     * implementation detail of the concrete AudioEngine and potentially the Instrument.
     * This might involve generating a waveform, triggering a sample, etc.
     *
     * @param instrumentName A string identifier for the instrument (e.g., "Piano", "SimpleSynth").
     *                       This helps the audio engine select the correct sound or synthesis method.
     * @param note The musical note to play.
     */
    virtual void playSound(const std::string& instrumentName, const Core::Note& note) = 0;

    // Potential future additions:
    // virtual void stopSound(int soundId) = 0; // To stop a specific sound instance
    // virtual void setVolume(float volume) = 0; // Global volume or per instrument
    // virtual bool loadSoundSample(const std::string& sampleName, const std::string& filePath) = 0;
};

} // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_AUDIOENGINE_H
