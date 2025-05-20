#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Controller/Controller.h"
#include "Controller/PianoAppController.h"
#include "Controller/XylophoneAppController.h"
#include "Controller/VideoGameAppController.h"
#include "Utils/DropdownMenu.h"
#include "Audio/SDLAudioEngine.h"

// Structure pour mapper les touches du clavier aux notes musicales
struct KeyboardMapping {
    SDL_Keycode key;    // Code de la touche SDL
    std::string note;   // Nom de la note (ex: "C4", "D#4", etc.)
    int octave;         // Octave de la note (4 ou 5)
};

enum class InstrumentType {
    PIANO,
    XYLOPHONE,
    VIDEO_GAME
};

class Application {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Controller *mainController;
    MusicApp::Audio::AudioEngine *audioEngine;
    int windowWidth;
    int windowHeight;
    bool initialized;
    InstrumentType currentInstrument;
    DropdownMenu *instrumentMenu;

    void initializeInstrumentMenu();

    // Mapping des touches du clavier
    std::vector<KeyboardMapping> keyboardMappings;
    std::unordered_map<SDL_Keycode, bool> keyboardNotesState; // true = pressed, false = released

    // Méthodes pour le contrôle clavier
    void initializeKeyboardMappings();

    void handleKeyPress(SDL_Keycode key);

    void handleKeyRelease(SDL_Keycode key);

    std::string getNoteForKey(SDL_Keycode key);

    float getVelocityForKey(); // Pourrait être paramétré plus tard

    // Pour suivre la note actuellement jouée via la souris
    std::string currentPlayingNote;
    bool isMouseButtonDown;
    Uint32 lastNotePlayTime;

public:
    Application(int width = 1440, int height = 1024);

    ~Application();

    bool initialize();

    bool run();

    void cleanup();

    void setInstrument(InstrumentType instrument);
};