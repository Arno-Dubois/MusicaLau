#include "ConsoleView.h"
#include "../Core/Note.h"
#include <iostream>
#include <string>
#include <limits> // Required for std::numeric_limits

namespace MusicApp {
namespace View {

ConsoleView::ConsoleView(Controller::MusicController& controller)
    : controller_(controller) {
    std::cout << "ConsoleView: Initialized." << std::endl;
}

void ConsoleView::displayMenu() const {
    std::cout << "\n--- Music App Menu ---" << std::endl;
    std::cout << "1. Select Instrument" << std::endl;
    std::cout << "2. Play Note" << std::endl;
    std::cout << "3. List Available Instruments" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void ConsoleView::selectInstrument() {
    std::vector<std::string> instrumentNames = controller_.getAvailableInstrumentNames();
    if (instrumentNames.empty()) {
        std::cout << "No instruments available to select." << std::endl;
        return;
    }

    std::cout << "\n--- Select Instrument ---" << std::endl;
    for (size_t i = 0; i < instrumentNames.size(); ++i) {
        std::cout << i + 1 << ". " << instrumentNames[i] << std::endl;
    }
    std::cout << "0. Cancel" << std::endl;
    std::cout << "Enter number of instrument to select: ";

    int choiceNum;
    std::cin >> choiceNum;

    if (std::cin.fail()) {
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input. Please enter a number." << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline

    if (choiceNum > 0 && static_cast<size_t>(choiceNum) <= instrumentNames.size()) {
        if (controller_.selectInstrument(instrumentNames[choiceNum - 1])) {
            std::cout << "Instrument '" << instrumentNames[choiceNum - 1] << "' selected." << std::endl;
        } else {
            std::cout << "Failed to select instrument." << std::endl; // Should ideally not happen if listed
        }
    } else if (choiceNum == 0) {
        std::cout << "Instrument selection cancelled." << std::endl;
    } else {
        std::cout << "Invalid instrument number." << std::endl;
    }
}

void ConsoleView::playNote() {
    std::cout << "Enter note to play (e.g., C4, G#5): ";
    std::string pitchName;
    std::cin >> pitchName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline

    if (pitchName.empty()) {
        std::cout << "Invalid note. Pitch name cannot be empty." << std::endl;
        return;
    }

    Core::Note note(pitchName);
    // In a more advanced version, you might ask for duration, velocity etc.
    // note.durationMs = 500; // example
    controller_.playNote(note);
}

void ConsoleView::run() {
    if (!controller_.init()) {
        std::cerr << "ConsoleView: Failed to initialize MusicController. Exiting." << std::endl;
        return;
    }

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            choice = -1; // Set to an invalid choice to continue loop
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume the rest of the line, including newline

        switch (choice) {
            case 1:
                selectInstrument();
                break;
            case 2:
                playNote();
                break;
            case 3: {
                std::cout << "\n--- Available Instruments ---" << std::endl;
                auto names = controller_.getAvailableInstrumentNames();
                if (names.empty()) {
                    std::cout << "No instruments loaded." << std::endl;
                } else {
                    for (const auto& name : names) {
                        std::cout << "- " << name << std::endl;
                    }
                }
                break;
            }
            case 0:
                std::cout << "Exiting Music App." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

} // namespace View
} // namespace MusicApp
