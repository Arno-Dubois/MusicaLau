#ifndef MUSICAPP_CONTROLLER_MUSICCONTROLLER_H
#define MUSICAPP_CONTROLLER_MUSICCONTROLLER_H

#include "../Audio/AudioEngine.h"
#include "../Core/Instrument.h"
#include "../Core/Note.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept> // For std::runtime_error
#include <iostream> // For C++ style output

namespace MusicApp {
namespace Controller {

/**
 * @brief Manages instruments, the audio engine, and user interactions for playing music.
 *
 * This class acts as the controller in an MVC-like pattern.
 * It holds a reference to an AudioEngine and a collection of available Instruments.
 */
class MusicController {
public:
    /**
     * @brief Constructs a MusicController.
     *
     * @param audioEngine A unique_ptr to an AudioEngine implementation.
     *                    The controller takes ownership of the audio engine.
     */
    explicit MusicController(std::unique_ptr<Audio::AudioEngine> audioEngine);
    ~MusicController();

    /**
     * @brief Initializes the music controller and its audio engine.
     * @return True if initialization was successful, false otherwise.
     */
    bool init();

    /**
     * @brief Adds an instrument to the controller's collection.
     *
     * @param instrument A unique_ptr to an Instrument. The controller takes ownership.
     */
    void addInstrument(std::unique_ptr<Core::Instrument> instrument);

    /**
     * @brief Selects an instrument by its name to be the current instrument.
     *
     * @param name The name of the instrument to select.
     * @return True if an instrument with that name was found and selected, false otherwise.
     */
    bool selectInstrument(const std::string& name);

    /**
     * @brief Plays a note using the currently selected instrument.
     *
     * If no instrument is selected, or if the audio engine is not initialized,
     * this function will output an error and do nothing.
     *
     * @param note The note to play.
     */
    void playNote(const Core::Note& note);

    /**
     * @brief Gets the names of all available instruments.
     * @return A vector of strings containing instrument names.
     */
    std::vector<std::string> getAvailableInstrumentNames() const;

private:
    std::unique_ptr<Audio::AudioEngine> audioEngine_;
    std::vector<std::unique_ptr<Core::Instrument>> instruments_;
    Core::Instrument* currentInstrument_ = nullptr;
    bool isInitialized_ = false;
};

} // namespace Controller
} // namespace MusicApp

#endif // MUSICAPP_CONTROLLER_MUSICCONTROLLER_H
