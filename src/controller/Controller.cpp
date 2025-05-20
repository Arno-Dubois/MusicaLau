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
    Controller *controller = static_cast<Controller *>(userdata);
    if (controller && filePaths && filePaths[0]) {
        controller->importedFilePath = filePaths[0];
        size_t lastSlash = controller->importedFilePath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            controller->importedFileName = controller->importedFilePath.substr(lastSlash + 1);
        } else {
            controller->importedFileName = controller->importedFilePath;
        }
        controller->currentSongEvents_for_playback = parseMusicFile(controller->importedFilePath);
        if (!controller->currentSongEvents_for_playback.empty()) {
            controller->songLoaded = true;
            controller->songPlayRequested_ = false; // Reset play request status
            std::cout << "Controller: Song loaded: " << controller->importedFileName << std::endl;
        } else {
            controller->songLoaded = false;
            controller->songPlayRequested_ = false;
            controller->importedFileName.clear();
            std::cerr << "Controller: Failed to parse song file: " << controller->importedFilePath << std::endl;
        }
    } else if (controller) {
        std::cerr << "Controller: No file selected or error in file dialog." << std::endl;
        controller->songLoaded = false;
        controller->songPlayRequested_ = false;
        controller->importedFileName.clear();
    }
}

Controller::Controller() : font(nullptr), audioEngine(nullptr), currentWindowWidth(0), currentWindowHeight(0),
                           songLoaded(false), buttonView_(nullptr), songPlayRequested_(false) {
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
                                                               songPlayRequested_(false) {
    font = TextHelper::LoadFont("Roboto-SemiBold.ttf", 16);
    buttonView_ = new ButtonView();
    if (buttonView_) {
        buttonView_->initialize(font);
    }
    initializeButtons();
}

Controller::~Controller() {
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
    SDL_ShowOpenFileDialog(FileDialogCallback, this, nullptr, filters, SDL_arraysize(filters), nullptr, false);
}

void Controller::handlePlaySongClicked(const std::string &instrumentName) {
    if (songLoaded) {
        currentInstrumentName_for_song_ = instrumentName;
        songPlayRequested_ = true;
        std::cout << "Controller: Play song requested for " << importedFileName << " with instrument " << instrumentName
                << std::endl;
    } else {
        std::cerr << "Controller: No song loaded to play." << std::endl;
        songPlayRequested_ = false;
    }
}

std::string Controller::getCurrentInstrumentForSong() const {
    return currentInstrumentName_for_song_;
}

const std::vector<MusicalEvent> &Controller::getLoadedSongEvents() const {
    return currentSongEvents_for_playback;
}

bool Controller::isSongReadyToPlay() const {
    return songPlayRequested_;
}

bool Controller::getSongLoaded() const {
    return songLoaded;
}

void Controller::resetPlayRequestStatus() {
    songPlayRequested_ = false;
    std::cout << "Controller: Song play request status reset." << std::endl;
}

std::string Controller::getImportedFileName() const {
    return importedFileName;
}