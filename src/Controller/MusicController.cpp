#include "Controller/MusicController.h"
#include "Utils/Log.h"
#include <algorithm> // For std::find_if

namespace MusicApp {
namespace Controller {

MusicController::MusicController(
    std::unique_ptr<Audio::AudioEngine> audioEngine)
    : audioEngine_(std::move(audioEngine)), currentInstrument_(nullptr),
      isInitialized_(false) {
  if (!audioEngine_) {
    // Consider logging or a more robust error handling strategy for production
    // code
    std::cerr << "MusicController Error: AudioEngine cannot be null."
              << std::endl;
    // Optionally throw an exception if an audioEngine is critical for
    // construction throw std::invalid_argument("AudioEngine cannot be null");
  }
  MUSICALAU_LOG("MusicController: Constructed.");
}

MusicController::~MusicController() {
  MUSICALAU_LOG("MusicController: Destructing.");
  if (audioEngine_ && isInitialized_) {
    audioEngine_->shutdown();
  }
  // unique_ptrs for instruments and audioEngine_ will be automatically cleaned
  // up.
}

bool MusicController::init() {
  if (!audioEngine_) {
    std::cerr
        << "MusicController Error: AudioEngine is not set. Cannot initialize."
        << std::endl;
    isInitialized_ = false;
    return false;
  }
  MUSICALAU_LOG("MusicController: Initializing...");
  isInitialized_ = audioEngine_->init();
  if (isInitialized_) {
    MUSICALAU_LOG("MusicController: Successfully initialized.");
  } else {
    std::cerr << "MusicController Error: Failed to initialize AudioEngine."
              << std::endl;
  }
  return isInitialized_;
}

void MusicController::addInstrument(
    std::unique_ptr<Core::Instrument> instrument) {
  if (instrument) {
    MUSICALAU_LOG("MusicController: Adding instrument '"
                  << instrument->getName() << "'.");
    instruments_.push_back(std::move(instrument));
  } else {
    std::cerr << "MusicController Warning: Attempted to add a null instrument."
              << std::endl;
  }
}

bool MusicController::selectInstrument(const std::string &name) {
  auto it = std::find_if(instruments_.begin(), instruments_.end(),
                         [&](const std::unique_ptr<Core::Instrument> &instr) {
                           return instr && instr->getName() == name;
                         });

  if (it != instruments_.end()) {
    currentInstrument_ =
        it->get(); // Get raw pointer, ownership remains with unique_ptr
    MUSICALAU_LOG("MusicController: Selected instrument '"
                  << currentInstrument_->getName() << "'.");
    return true;
  }

  currentInstrument_ = nullptr;
  std::cerr << "MusicController Warning: Instrument '" << name << "' not found."
            << std::endl;
  return false;
}

void MusicController::playNote(const Core::Note &note) {
  if (!isInitialized_) {
    std::cerr << "MusicController Error: Not initialized. Cannot play note."
              << std::endl;
    return;
  }
  if (!audioEngine_) {
    std::cerr << "MusicController Error: AudioEngine is not available. Cannot "
                 "play note."
              << std::endl;
    return;
  }
  if (currentInstrument_) {
    MUSICALAU_LOG("MusicController: Requesting to play note '"
                  << note.pitchName << "' on instrument '"
                  << currentInstrument_->getName() << "'.");
    currentInstrument_->playNote(note, *audioEngine_);
  } else {
    std::cerr
        << "MusicController Warning: No instrument selected. Cannot play note '"
        << note.pitchName << "'." << std::endl;
  }
}

std::vector<std::string> MusicController::getAvailableInstrumentNames() const {
  std::vector<std::string> names;
  for (const auto &instr : instruments_) {
    if (instr) {
      names.push_back(instr->getName());
    }
  }
  return names;
}

} // namespace Controller
} // namespace MusicApp
