#include "../include/Application.h"
#include <iostream>
#include <SDL3/SDL_ttf.h>
#include <unordered_map>

Application::Application(int width, int height)
        : window(nullptr),
          renderer(nullptr),
          mainController(nullptr),
          audioEngine(nullptr),
          windowWidth(width),
          windowHeight(height),
          initialized(false),
          currentInstrument(InstrumentType::PIANO),
          instrumentMenu(nullptr),
          currentPlayingNote(""),
          isMouseButtonDown(false),
          lastNotePlayTime(0) {
}

Application::~Application() {
    cleanup();
}

void Application::initializeInstrumentMenu() {
    // On utilise des valeurs relatives aux dimensions de la fenêtre
    float toolbarY = 20.0f;
    float buttonHeight = windowHeight * 0.09f; // ~9% de la hauteur de la fenêtre
    float spacing = windowHeight * 0.03f; // ~3% de la hauteur

    // Position et dimensions adaptatives du rectangle bleu
    float mainAreaX = windowWidth * 0.035f; // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + spacing;
    float mainAreaWidth = windowWidth * 0.93f; // ~93% de la largeur
    float headerHeight = windowHeight * 0.05f; // ~5% de la hauteur

    // Créer le menu déroulant avec le nom de l'instrument actif
    std::string headerLabel;
    switch (currentInstrument) {
        case InstrumentType::PIANO:
            headerLabel = "Piano";
            break;
        case InstrumentType::XYLOPHONE:
            headerLabel = "Xylophone";
            break;
        case InstrumentType::VIDEO_GAME:
            headerLabel = "Jeu vidéo";
            break;
    }

    instrumentMenu = new DropdownMenu(mainAreaX, mainAreaY, mainAreaWidth, headerHeight, headerLabel);

    // Ajouter les éléments au menu
    instrumentMenu->addItem("Piano", [this]() {
        setInstrument(InstrumentType::PIANO);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });

    instrumentMenu->addItem("Xylophone", [this]() {
        setInstrument(InstrumentType::XYLOPHONE);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });

    instrumentMenu->addItem("Jeu vidéo", [this]() {
        setInstrument(InstrumentType::VIDEO_GAME);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });
}

bool Application::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialiser SDL_ttf
    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf could not initialize! Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    audioEngine = new MusicApp::Audio::SDLAudioEngine();
    if (!audioEngine->init()) {
        SDL_Log("AudioEngine could not initialize!\n");
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("MusicaLau - Instrument Interface", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Initialiser le menu déroulant
    try {
        initializeInstrumentMenu();
        if (!instrumentMenu) {
            SDL_Log("Failed to initialize instrument menu!");
            return false;
        }
    } catch (const std::exception &e) {
        SDL_Log("Exception during menu initialization: %s", e.what());
        return false;
    }

    // Créer le contrôleur par défaut (Piano)
    try {
        setInstrument(currentInstrument);
        if (!mainController) {
            SDL_Log("Failed to create main controller!");
            return false;
        }
    } catch (const std::exception &e) {
        SDL_Log("Exception during controller creation: %s", e.what());
        return false;
    }

    initialized = true;
    return true;
}

void Application::setInstrument(InstrumentType instrument) {
    // Nettoyer le contrôleur existant
    delete mainController;

    // Créer le nouveau contrôleur
    switch (instrument) {
        case InstrumentType::PIANO:
            mainController = new PianoAppController(windowWidth, windowHeight, audioEngine);
            break;
        case InstrumentType::XYLOPHONE:
            mainController = new XylophoneAppController(windowWidth, windowHeight, audioEngine);
            break;
        case InstrumentType::VIDEO_GAME:
            mainController = new VideoGameAppController(windowWidth, windowHeight, audioEngine);
            break;
    }

    currentInstrument = instrument;
}

bool Application::run() {
    if (!initialized) {
        if (!initialize()) {
            return false;
        }
    }

    bool quit = false;
    SDL_Event event;

    std::unordered_map<std::string, bool> activeNotes;

    Uint32 lastUpdateTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    quit = true;
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                bool clickHandled = instrumentMenu->handleClick(mouseX, mouseY);

                if (!clickHandled && !instrumentMenu->isMenuOpen()) {
                    int buttonClicked = mainController->handleButtonClick(mouseX, mouseY);

                    if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                        if (buttonClicked != -1) {
                            pianoController->processButtonAction(buttonClicked);
                        } else {
                            std::string pitchName = pianoController->getPiano()->getPitchAt(mouseX, mouseY);
                            if (!pitchName.empty()) {
                                MusicApp::Core::Note note(pitchName);
                                audioEngine->playSound("Piano", note);
                                activeNotes[pitchName] = true;
                                currentPlayingNote = pitchName;
                                isMouseButtonDown = true;
                                lastNotePlayTime = SDL_GetTicks();
                            }
                        }
                    } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                        if (buttonClicked != -1) {
                            xylophoneController->processButtonAction(buttonClicked);
                        } else {
                            xylophoneController->handleXylophoneKeyClick(mouseX, mouseY);
                        }
                    } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                        if (buttonClicked != -1) {
                            videoGameController->processButtonAction(buttonClicked);
                        } else {
                            // TODO: Implémenter cette méthode
                            // videoGameController->handleGameInput(mouseX, mouseY);
                        }
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                instrumentMenu->updateHoverState(mouseX, mouseY);

                if (!instrumentMenu->isMenuOpen()) {
                    if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                        pianoController->handlePianoKeyHover(mouseX, mouseY);
                    } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                        xylophoneController->handleXylophoneKeyHover(mouseX, mouseY);
                    }
                    /* else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                        videoGameController->handleVideoGameKeyHover(mouseX, mouseY);
                    } */
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                    float mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    std::string pitchName = pianoController->getPiano()->getPitchAt(mouseX, mouseY);
                    if (!pitchName.empty()) {
                        MusicApp::Core::Note note(pitchName);
                        MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
                        if (engine) {
                            engine->stopSound("Piano", note);
                            currentPlayingNote = "";
                            isMouseButtonDown = false;
                        }
                    }
                }
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);

                if (instrumentMenu) {
                    delete instrumentMenu;
                    initializeInstrumentMenu();
                }

                if (mainController) {
                    setInstrument(currentInstrument);
                }
            }
        }

        // Mettre à jour les sons toutes les 100ms
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime > 100) {
            lastUpdateTime = currentTime;

            // Rejouer la note actuelle si le bouton de la souris est toujours enfoncé
            if (isMouseButtonDown && !currentPlayingNote.empty() &&
                (currentTime - lastNotePlayTime >= 450)) { // Rejouer légèrement avant la fin de la note
                if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                    MusicApp::Core::Note note(currentPlayingNote);
                    audioEngine->playSound("Piano", note);
                    lastNotePlayTime = currentTime;
                }
            }

            // Nettoyer les notes qui jouent depuis trop longtemps (2 secondes maximum)
            MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
            if (engine) {
                engine->cleanupLongPlayingNotes(1000); // Réduire à 1 seconde pour plus de sécurité
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // Rendre l'interface utilisateur et l'instrument actif
        mainController->render(renderer, windowWidth, windowHeight);
        // Rendre le menu déroulant
        instrumentMenu->render(renderer);
        // Présenter le rendu
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return true;
}

void Application::cleanup() {
    delete mainController;
    mainController = nullptr;

    delete instrumentMenu;
    instrumentMenu = nullptr;

    // Shutdown and delete AudioEngine
    if (audioEngine) {
        audioEngine->shutdown();
        delete audioEngine;
        audioEngine = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
    initialized = false;
}