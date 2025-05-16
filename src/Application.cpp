#include "../include/Application.h"
#include <iostream>

Application::Application(int width, int height)
        : window(nullptr),
          renderer(nullptr),
          mainController(nullptr),
          windowWidth(width),
          windowHeight(height),
          initialized(false),
          currentInstrument(InstrumentType::PIANO),
          instrumentMenu(nullptr) {
}

Application::~Application() {
    cleanup();
}

void Application::initializeInstrumentMenu() {
    // On utilise les mêmes valeurs que dans PianoAppController pour le positionnement
    float toolbarY = 20.0f;
    float buttonHeight = 90.0f;
    float spacing = 30.0f;

    // Position et dimensions exactes du rectangle bleu
    float mainAreaX = 50.0f;
    float mainAreaY = toolbarY + buttonHeight + spacing;
    float mainAreaWidth = windowWidth - 100.0f;
    float headerHeight = 50.0f;

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
    // Initializer SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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
            mainController = new PianoAppController(windowWidth, windowHeight);
            break;
        case InstrumentType::XYLOPHONE:
            mainController = new XylophoneAppController(windowWidth, windowHeight);
            break;
        case InstrumentType::VIDEO_GAME:
            mainController = new VideoGameAppController(windowWidth, windowHeight);
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

    // Boucle principale
    bool quit = false;
    SDL_Event event;

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

                // Vérifier d'abord les clics sur le menu déroulant
                bool clickHandled = instrumentMenu->handleClick(mouseX, mouseY);

                // Si le clic n'a pas été géré par le menu, le transmettre au contrôleur
                if (!clickHandled && !instrumentMenu->isMenuOpen()) {
                    int buttonClicked = mainController->handleButtonClick(mouseX, mouseY);

                    if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                        pianoController->processButtonAction(buttonClicked);
                    } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                        xylophoneController->processButtonAction(buttonClicked);
                    } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                        videoGameController->processButtonAction(buttonClicked);
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                // Gérer l'effet de survol pour les éléments du menu
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                instrumentMenu->updateHoverState(mouseX, mouseY);
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

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    initialized = false;
}