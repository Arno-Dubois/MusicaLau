#include "../include/Application.h"
#include <iostream>

Application::Application(int width, int height)
        : window(nullptr),
          renderer(nullptr),
          mainController(nullptr),
          windowWidth(width),
          windowHeight(height),
          initialized(false),
          currentInstrument(InstrumentType::PIANO) {
}

Application::~Application() {
    cleanup();
}

bool Application::initialize() {
    // Initializer SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
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

    // Créer le contrôleur par défaut (Piano)
    setInstrument(currentInstrument);

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
                } else if (event.key.key == SDLK_1) {
                    setInstrument(InstrumentType::PIANO);
                } else if (event.key.key == SDLK_2) {
                    setInstrument(InstrumentType::XYLOPHONE);
                } else if (event.key.key == SDLK_3) {
                    setInstrument(InstrumentType::VIDEO_GAME);
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Gestion des clics de bouton selon le contrôleur actif
                int buttonClicked = mainController->handleButtonClick(mouseX, mouseY);

                if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                    pianoController->processButtonAction(buttonClicked);
                } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                    xylophoneController->processButtonAction(buttonClicked);
                } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                    videoGameController->processButtonAction(buttonClicked);
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // Rendre l'interface utilisateur et l'instrument actif
        mainController->render(renderer, windowWidth, windowHeight);

        // Présenter le rendu
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return true;
}

void Application::cleanup() {
    delete mainController;
    mainController = nullptr;

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