#ifndef MUSICAPP_VIEW_CONSOLEVIEW_H
#define MUSICAPP_VIEW_CONSOLEVIEW_H

#include "../Controller/MusicController.h" // To interact with the controller
#include "../Core/Note.h"                 // For creating Note objects
#include <iostream>
#include <string>
#include <vector>
#include <limits> // Required for std::numeric_limits

namespace MusicApp {
namespace View {

/**
 * @brief A simple console-based view for the Music Application.
 *
 * Allows users to interact with the MusicController through text commands.
 */
class ConsoleView {
public:
    /**
     * @brief Constructs a ConsoleView.
     *
     * @param controller A reference to the MusicController.
     */
    explicit ConsoleView(Controller::MusicController& controller);

    /**
     * @brief Starts the main interaction loop for the console view.
     *
     * Displays a menu and processes user input.
     */
    void run();

private:
    /**
     * @brief Displays the main menu options to the user.
     */
    void displayMenu() const;

    /**
     * @brief Prompts the user to select an instrument.
     */
    void selectInstrument();

    /**
     * @brief Prompts the user to enter a note and plays it.
     */
    void playNote();

    Controller::MusicController& controller_; // Reference to the controller
};

} // namespace View
} // namespace MusicApp

#endif // MUSICAPP_VIEW_CONSOLEVIEW_H
