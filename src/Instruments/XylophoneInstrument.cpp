#include "XylophoneInstrument.h"

namespace MusicApp {
namespace Instruments {

// Constructor
XylophoneInstrument::XylophoneInstrument()
    : name_("Xylophone") {}

// Gets the name of the instrument
std::string XylophoneInstrument::getName() const {
    return name_;
}

// Plays a note using the provided audio engine
void XylophoneInstrument::playNote(
    const Core::Note& note,
    Audio::AudioEngine& audioEngine) const {
    // The instrument tells the audio engine to play a sound,
    // identifying itself by name and passing the note details.
    audioEngine.playSound(getName(), note);
}

} // namespace Instruments
} // namespace MusicApp
