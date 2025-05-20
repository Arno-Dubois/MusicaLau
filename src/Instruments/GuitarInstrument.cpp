#include "../../include/Instruments/GuitarInstrument.h"

namespace MusicApp {
namespace Instruments {

// Constructor
GuitarInstrument::GuitarInstrument() {
    setName("Guitar");
}

// Plays a note using the provided audio engine
void GuitarInstrument::playNote(
    const Core::Note& note,
    Audio::AudioEngine& audioEngine) const {
    // The instrument tells the audio engine to play a sound,
    // identifying itself by name and passing the note details.
    audioEngine.playSound(getName(), note);
}

} // namespace Instruments
} // namespace MusicApp
