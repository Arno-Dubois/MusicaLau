#include "View/ConsoleView.h"
#include "Core/Note.h"
#include <iostream>
#include <limits> // Required for std::numeric_limits
#include <string>

namespace MusicApp {
namespace View {

ConsoleView::ConsoleView(Controller::MusicController &controller)
    : controller_(controller) {
  MUSICALAU_LOG("ConsoleView: Initialized.");
}

void ConsoleView::displayMenu() const {
  MUSICALAU_LOG("\n--- Music App Menu ---");
  MUSICALAU_LOG("1. Select Instrument");
  MUSICALAU_LOG("2. Play Note");
  MUSICALAU_LOG("3. List Available Instruments");
  MUSICALAU_LOG("0. Exit");
            MUSICALAU_LOG("Enter your choice: ";
}

void ConsoleView::selectInstrument() {
  std::vector<std::string> instrumentNames =
      controller_.getAvailableInstrumentNames();
  if (instrumentNames.empty()) {
    MUSICALAU_LOG("No instruments available to select.");
    return;
  }

  MUSICALAU_LOG("\n--- Select Instrument ---");
  for (size_t i = 0; i < instrumentNames.size(); ++i) {
    std::cout << i + 1 << ". " << instrumentNames[i] << std::endl;
  }
  MUSICALAU_LOG("0. Cancel");
            MUSICALAU_LOG("Enter number of instrument to select: ";

            int choiceNum;
            std::cin >> choiceNum;

            if (std::cin.fail()) {
    std::cin.clear(); // Clear error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // Discard invalid input
    MUSICALAU_LOG("Invalid input. Please enter a number.");
    return;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume newline

            if (choiceNum > 0 && static_cast<size_t>(choiceNum) <= instrumentNames.size()) {
    if (controller_.selectInstrument(instrumentNames[choiceNum - 1])) {
      MUSICALAU_LOG("Instrument '" << instrumentNames[choiceNum - 1]
                                   << "' selected.");
    } else {
      MUSICALAU_LOG("Failed to select instrument.");
      Should ideally not happen if listed
    }
            } else if (choiceNum == 0) {
    MUSICALAU_LOG("Instrument selection cancelled.");
            } else {
    MUSICALAU_LOG("Invalid instrument number.");
            }
}

void ConsoleView::playNote() {
  MUSICALAU_LOG("Enter note to play (e.g., C4, G#5): ");
  std::string pitchName;
  std::cin >> pitchName;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                  '\n'); // consume newline

  if (pitchName.empty()) {
    MUSICALAU_LOG("Invalid note. Pitch name cannot be empty.");
    return;
  }

  Core::Note note(pitchName);
  // In a more advanced version, you might ask for duration, velocity etc.
  // note.durationMs = 500; // example
  controller_.playNote(note);
}

void ConsoleView::run() {
  if (!controller_.init()) {
    std::cerr << "ConsoleView: Failed to initialize MusicController. Exiting."
              << std::endl;
    return;
  }

  int choice;
  do {
    displayMenu();
    std::cin >> choice;

    if (std::cin.fail()) {
      MUSICALAU_LOG("Invalid input. Please enter a number.");
      std::cin.clear(); // Clear error flags
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                      '\n'); // Discard invalid input
      choice = -1;           // Set to an invalid choice to continue loop
      continue;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // consume the rest of the line, including newline

    switch (choice) {
    case 1:
      selectInstrument();
      break;
    case 2:
      playNote();
      break;
    case 3: {
      MUSICALAU_LOG("\n--- Available Instruments ---");
      auto names = controller_.getAvailableInstrumentNames();
      if (names.empty()) {
        MUSICALAU_LOG("No instruments loaded.");
      } else {
        for (const auto &name : names) {
          MUSICALAU_LOG("- " << name);
        }
      }
      break;
    }
    case 0:
      MUSICALAU_LOG("Exiting Music App.");
      break;
    default:
      MUSICALAU_LOG("Invalid choice. Please try again.");
    }
  } while (choice != 0);
}

} // namespace View
} // namespace MusicApp
