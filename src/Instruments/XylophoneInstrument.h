#ifndef MUSICAPP_INSTRUMENTS_XYLOPHONEINSTRUMENT_H
#define MUSICAPP_INSTRUMENTS_XYLOPHONEINSTRUMENT_H

#include "../Core/Instrument.h"
#include "../Audio/AudioEngine.h" // For AudioEngine parameter in playNote
#include <string>

namespace MusicApp {
namespace Instruments {

/**
 * @brief A Xylophone instrument.
 *
 * This instrument tells the AudioEngine to play a note, identifying itself
 * as "Xylophone". The actual sound generation is handled by the AudioEngine.
 */
class XylophoneInstrument : public Core::Instrument {
public:
    XylophoneInstrument();
    ~XylophoneInstrument() override = default;

    std::string getName() const override;

    void playNote(const Core::Note& note, Audio::AudioEngine& audioEngine) const override;

private:
    std::string name_;
};

} // namespace Instruments
} // namespace MusicApp

#endif // MUSICAPP_INSTRUMENTS_XYLOPHONEINSTRUMENT_H
