#include "../include/Application.h"
#include <iostream>

Application::Application(int width, int height)
        : window(nullptr),
          renderer(nullptr),
          mainController(nullptr),
          windowWidth(width),
          windowHeight(height),
          initialized(false) {
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

    window = SDL_CreateWindow("Piano Interface", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
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

    // Créer le contrôleur principal
    mainController = new PianoAppController(windowWidth, windowHeight);

    initialized = true;
    return true;
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

                // Cast vers PianoAppController pour accéder à la méthode processButtonAction
                PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController);
                if (pianoController) {
                    int buttonClicked = pianoController->handleButtonClick(mouseX, mouseY);
                    pianoController->processButtonAction(buttonClicked);
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // Rendre l'interface utilisateur et le piano
        mainController->render(renderer, windowWidth, windowHeight);

        // Présenter le rendu
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return true;
}

void Application::cleanup() {
    delete mainController;

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