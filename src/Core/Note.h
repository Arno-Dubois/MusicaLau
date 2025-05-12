#ifndef MUSICAPP_CORE_NOTE_H
#define MUSICAPP_CORE_NOTE_H

#include <string> // For pitch name
#include <utility> // For std::move

namespace MusicApp {
namespace Core {

/**
 * @brief Represents a musical note with pitch and duration.
 */
struct Note {
    std::string pitchName;  // Example: "C4", "G#5", or "0" for silence
    float durationSeconds;  // Duration of the note in seconds

    /**
     * @brief Default constructor.
     */
    Note() : pitchName("0"), durationSeconds(0.0f) {}

    /**
     * @brief Constructs a Note with specified pitch name and duration.
     *
     * @param pName The pitch name (e.g., "C4", "G#5"). Can be "0" or "Unknown" for silence.
     * @param durSeconds The duration of the note in seconds.
     */
    explicit Note(std::string pName, float durSeconds)
        : pitchName(std::move(pName)), durationSeconds(durSeconds) {
        // Treat "Unknown" from file as silence (pitchName "0")
        if (this->pitchName == "Unknown") {
            this->pitchName = "0";
        }
    }

    // Optional: A constructor for only pitch name, with a default duration (though less used now)
    explicit Note(std::string pName) 
        : pitchName(std::move(pName)), durationSeconds(0.5f) { // Default duration if not specified
        if (this->pitchName == "Unknown") {
            this->pitchName = "0";
        }
    }
};

} // namespace Core
} // namespace MusicApp

#endif // MUSICAPP_CORE_NOTE_H
