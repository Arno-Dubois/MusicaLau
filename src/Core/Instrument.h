#ifndef MUSICAPP_CORE_INSTRUMENT_H
#define MUSICAPP_CORE_INSTRUMENT_H

#include "Note.h" // Definition of Note
#include <string> // For instrument name

// Forward declaration for AudioEngine to avoid circular dependency if Instrument needs it directly.
// However, it's likely AudioEngine will be passed as a parameter to playNote.
namespace MusicApp {
namespace Audio {
    class AudioEngine;
}
}

namespace MusicApp {
namespace Core {

/**
 * @brief Abstract base class for musical instruments.
 *
 * Concrete instrument classes will derive from this and implement the playNote method.
 */
class Instrument {
protected:
    std::string name_;
public:
    virtual ~Instrument() = default; // Important for proper cleanup of derived classes

    /**
     * @brief Gets the name of the instrument.
     * @return The name of the instrument.
     */
    std::string getName() const {
        return name_;
    }

    void setName(const std::string &name)  {
        name_ = name;
    }


    /**
     * @brief Plays a given note using the specified audio engine.
     *
     * @param note The note to play.
     * @param audioEngine The audio engine to use for playback.
     */
    virtual void playNote(const Note& note, MusicApp::Audio::AudioEngine& audioEngine) const = 0;
};

} // namespace Core
} // namespace MusicApp

#endif // MUSICAPP_CORE_INSTRUMENT_H
