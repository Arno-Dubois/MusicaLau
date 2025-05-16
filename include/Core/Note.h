#ifndef MUSICAPP_CORE_NOTE_H
#define MUSICAPP_CORE_NOTE_H

#include <string> // For pitch name, or use int for MIDI value

namespace MusicApp {
namespace Core {

/**
 * @brief Represents a musical note.
 *
 * This could be expanded to include duration, velocity, etc.
 * For now, it's a simple representation of pitch.
 */
struct Note {
    // Example: "C4", "G#5", or a MIDI note number
    std::string pitchName;
    // Or use an integer for MIDI note value:
    // int midiValue;

    // Optional: duration in milliseconds
    // int durationMs;

    // Constructor
    explicit Note(std::string pName) : pitchName(std::move(pName)) {}
    // explicit Note(int mValue) : midiValue(mValue) {}
};

} // namespace Core
} // namespace MusicApp

#endif // MUSICAPP_CORE_NOTE_H
