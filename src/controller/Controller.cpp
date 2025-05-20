//
// Created by coren on 24/03/2025.
//

#include "../../include/Controller/Controller.h"
#include "../../include/utils/TextHelper.h"
#include <iostream>
#include <SDL3/SDL_dialog.h>
#include "../../include/Audio/MusicFileReader.h"
#include "../../include/Audio/SDLAudioEngine.h"
#include "../../include/View/ButtonView.h"

// Callback function for SDL_ShowOpenFileDialog
static void FileDialogCallback(void *userdata, const char *const *filePaths, int numFiles) {
    auto *controller = static_cast<Controller *>(userdata);
    std::cout << *filePaths;
    std::cout << numFiles;
    std::cout << userdata;
    if (controller && filePaths[0]) {
        controller->importedFilePath = filePaths[0];
        // Extract filename from path
        size_t lastSlash = controller->importedFilePath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            controller->importedFileName = controller->importedFilePath.substr(lastSlash + 1);
        } else {
            controller->importedFileName = controller->importedFilePath;
        }

        controller->currentSongEvents = parseMusicFile(controller->importedFilePath);
        if (!controller->currentSongEvents.empty()) {
            controller->songLoaded = true;
            std::cout << "Song loaded: " << controller->importedFileName << std::endl;
        } else {
            controller->songLoaded = false;
            controller->importedFileName.clear();
            std::cerr << "Failed to parse song file: " << controller->importedFilePath << std::endl;
        }
    } else if (controller) {
        std::cerr << "No file selected or error in file dialog." << std::endl;
        controller->songLoaded = false;
        controller->importedFileName.clear();
    }
}

Controller::Controller() : font(nullptr), audioEngine(nullptr), currentWindowWidth(0), currentWindowHeight(0),
                           songLoaded(false), buttonView_(nullptr), stopSongPlayback_(false), isSongPlaying_(false),
                           songPlaybackThread_() {
    font = TextHelper::LoadFont("Roboto-SemiBold.ttf", 16);
    buttonView_ = new ButtonView();
    if (buttonView_) {
        buttonView_->initialize(font);
    }
    initializeButtons();
}

Controller::Controller(MusicApp::Audio::AudioEngine *audioE) : audioEngine(audioE), font(nullptr),
                                                               currentWindowWidth(0), currentWindowHeight(0),
                                                               songLoaded(false), buttonView_(nullptr),
                                                               stopSongPlayback_(false), isSongPlaying_(false),
                                                               songPlaybackThread_() {
    font = TextHelper::LoadFont("Roboto-SemiBold.ttf", 16);
    buttonView_ = new ButtonView();
    if (buttonView_) {
        buttonView_->initialize(font);
    }
    initializeButtons();
}

Controller::~Controller() {
    if (songPlaybackThread_.joinable()) {
        songPlaybackThread_.join();
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    delete buttonView_;
    buttonView_ = nullptr;
}

void Controller::initializeButtons() {
    std::vector<std::string> buttonNames = {
            "Select",
            "Remove Octave",
            "Add Octave",
            "Import File",
            "Play Song",
            "Start Recording",
            "Export",
            "Finish Recording"
    };

    buttons.clear();

    float availableWidth = currentWindowWidth * 0.95f;
    int maxButtonsToShow = 8;

    if (currentWindowWidth > 0) {
        float requiredSpace = 8 * buttonWidth + 7 * buttonSpacing;
        if (requiredSpace > availableWidth) {
            int spacePerButton = static_cast<int>(buttonWidth + buttonSpacing);
            maxButtonsToShow = spacePerButton > 0 ? static_cast<int>(availableWidth / spacePerButton) : 3;
            maxButtonsToShow = std::max(3, std::min(8, maxButtonsToShow));
        }
    }

    SDL_Color darkGray = {46, 46, 46, 255};
    for (int i = 0; i < maxButtonsToShow; i++) {
        Button btn;
        btn.rect = {startX + i * (buttonWidth + buttonSpacing), toolbarY, buttonWidth, buttonHeight};
        btn.color = darkGray;
        btn.name = buttonNames[i];
        btn.icon = nullptr;
        buttons.push_back(btn);
    }
}

float Controller::calculateRelativeWidth(int windowWidth, float percentage) {
    return windowWidth * percentage;
}

float Controller::calculateRelativeHeight(int windowHeight, float percentage) {
    return windowHeight * percentage;
}

void Controller::updateDimensions(int windowWidth, int windowHeight) {
    currentWindowWidth = windowWidth;
    currentWindowHeight = windowHeight;

    int maxButtonsCount = 8;
    float availableWidth = windowWidth * 0.95f;
    float minButtonWidth = 90.0f;
    float idealButtonSpacing = windowWidth * 0.02f;

    int visibleButtonCount = maxButtonsCount;
    buttonWidth = minButtonWidth;

    while (visibleButtonCount > 3) {
        float requiredSpace = visibleButtonCount * buttonWidth + (visibleButtonCount - 1) * idealButtonSpacing;

        if (requiredSpace <= availableWidth) {
            buttonWidth = std::min(150.0f, (availableWidth - (visibleButtonCount - 1) * idealButtonSpacing) /
                                           visibleButtonCount);
            break;
        }

        visibleButtonCount--;
    }

    buttonHeight = calculateRelativeHeight(windowHeight, 0.088f);
    buttonSpacing = idealButtonSpacing;
    toolbarY = calculateRelativeHeight(windowHeight, 0.02f);
    startX = (windowWidth - (visibleButtonCount * buttonWidth + (visibleButtonCount - 1) * buttonSpacing)) /
             2;

    initializeButtons();
}

int Controller::handleButtonClick(float x, float y) {
    for (size_t i = 0; i < buttons.size(); i++) {
        if (x >= buttons[i].rect.x && x <= buttons[i].rect.x + buttons[i].rect.w &&
            y >= buttons[i].rect.y && y <= buttons[i].rect.y + buttons[i].rect.h) {
            return i;  // Retourne l'index du bouton cliqué
        }
    }
    return -1;  // Aucun bouton cliqué
}

void Controller::handleImportSong() {
    SDL_DialogFileFilter filters[1] = {{"Text files", "txt"}};
    // SDL_ShowOpenFileDialog takes a C-style function pointer, so we use the static callback
    SDL_ShowOpenFileDialog(FileDialogCallback, this, nullptr, filters, SDL_arraysize(filters), nullptr, false);
}

void Controller::handlePlaySong(const std::string &instrumentName) {
    if (isSongPlaying_) {
        std::cout << "Controller: Song is already playing. Stop the current song first." << std::endl;
        return;
    }
    if (songLoaded && audioEngine) {
        if (songPlaybackThread_.joinable()) {
            songPlaybackThread_.join(); // Ensure previous thread is finished
        }
        stopSongPlayback_ = false; // Reset stop flag
        // It's crucial to copy currentSongEvents if it might be modified, or ensure access is thread-safe.
        // For now, we pass a copy to the thread.
        std::vector<MusicalEvent> eventsCopy = currentSongEvents;
        songPlaybackThread_ = std::thread(&Controller::songPlaybackLoop, this, instrumentName, eventsCopy);
    } else {
        std::cerr << "Controller: No song loaded or audio engine not available." << std::endl;
    }
}

void Controller::songPlaybackLoop(std::string instrumentName, std::vector<MusicalEvent> eventsToPlay) {
    isSongPlaying_ = true;
    auto sdlAudioEngine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
    if (!sdlAudioEngine) {
        std::cerr << "Controller: Cannot play song, audio engine is not SDLAudioEngine." << std::endl;
        isSongPlaying_ = false;
        return;
    }

    for (const auto &event: eventsToPlay) {
        if (stopSongPlayback_) {
            break; // Exit loop if stop is requested
        }

        int delayMilliseconds = static_cast<int>(event.durationSeconds * 1000.0f);

        if (event.pitchName != "0") {
            MusicApp::Core::Note noteToPlay(event.pitchName);
            sdlAudioEngine->playSound(instrumentName, noteToPlay, 1.0f); // Default velocity

            if (delayMilliseconds > 0) {
                SDL_Delay(delayMilliseconds); // This delay still blocks this thread, but not the main one
            }
            // Only stop the note if it was actually played (and not a silence that got through)
            // And also only if we are not immediately exiting due to stopSongPlayback_
            if (!stopSongPlayback_) {
                sdlAudioEngine->stopSound(instrumentName, noteToPlay);
            }
        } else {
            // Silence
            if (delayMilliseconds > 0) {
                SDL_Delay(delayMilliseconds);
            }
        }
        if (!stopSongPlayback_) {
            SDL_Delay(10); // Small delay between events
        }
    }
    std::cout << "Song playback finished or stopped." << std::endl;
    isSongPlaying_ = false;
    stopSongPlayback_ = false; // Reset for next playback
}

void Controller::stopCurrentlyPlayingSong() {
    if (isSongPlaying_ && songPlaybackThread_.joinable()) {
        stopSongPlayback_ = true; // Signal the thread to stop
        songPlaybackThread_.join(); // Wait for the thread to finish
        std::cout << "Controller: Song playback stopped by user." << std::endl;
        // Resetting flags is handled at the end of songPlaybackLoop
    } else {
        std::cout << "Controller: No song is currently playing or thread not joinable." << std::endl;
    }
}

std::string Controller::getImportedFileName() const {
    return importedFileName;
}