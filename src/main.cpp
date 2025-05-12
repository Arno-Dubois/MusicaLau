#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_events.h>
#include <iostream>
#include <vector>
#include <string>

#include "controllers/Controller.h"
#include "models/Piano.h"
#include "views/PianoView.h"

// Classe principale qui hérite de Controller
class PianoAppController : public Controller {
private:
    Piano *piano;
    PianoView *pianoView;

public:
    PianoAppController(int windowWidth, int windowHeight) : Controller() {
        // Position et dimensions du piano
        float mainAreaX = 50.0f;
        float mainAreaY = toolbarY + buttonHeight + 30.0f;
        float mainAreaWidth = windowWidth - 100.0f;
        float headerHeight = 50.0f;
        float pianoPanelHeight = 150.0f;
        float pianoY = mainAreaY + headerHeight + pianoPanelHeight + 20.0f;
        float pianoHeight = 250.0f;

        piano = new Piano(mainAreaX, pianoY, mainAreaWidth, pianoHeight);
        pianoView = new PianoView(piano);
    }

    ~PianoAppController() {
        delete piano;
        delete pianoView;
    }

    void processButtonAction(int buttonIndex) {
        if (buttonIndex == -1) return;

        switch (buttonIndex) {
            case 1:  // Remove Octave
                piano->removeOctave();
                break;
            case 2:  // Add Octave
                piano->addOctave();
                break;
                // Autres actions de boutons à ajouter...
        }
    }

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override {
        // Dessiner la surface de travail (background)
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        SDL_FRect workspace = {0, 0, (float) windowWidth, (float) windowHeight};
        SDL_RenderFillRect(renderer, &workspace);

        // Barre du haut
        SDL_Color brownColor = {90, 60, 55, 255};
        SDL_SetRenderDrawColor(renderer, brownColor.r, brownColor.g, brownColor.b, brownColor.a);
        SDL_FRect topBar = {0, 0, (float) windowWidth, 129.0f};
        SDL_RenderFillRect(renderer, &topBar);

        // Dessiner les boutons
        Controller::renderButtons(renderer, buttons);




        // Dimensions pour l'interface principale
        float mainAreaX = 50.0f;
        float mainAreaY = toolbarY + buttonHeight + 30.0f;
        float mainAreaWidth = windowWidth - 100.0f;
        float headerHeight = 50.0f;
        float pianoPanelHeight = 150.0f;

        // En-tête instrument
        SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
        SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
        SDL_RenderFillRect(renderer, &instrumentHeader);

        // Panneau du piano
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_FRect pianoPanel = {mainAreaX, mainAreaY + headerHeight + 10, mainAreaWidth, pianoPanelHeight};
        SDL_RenderFillRect(renderer, &pianoPanel);

        // Zone d'affichage pour le nom de fichier
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_FRect fileNameDisplay = {pianoPanel.x + pianoPanel.w * 0.25f, pianoPanel.y + 25, pianoPanel.w * 0.5f,
                                     pianoPanel.h - 50};
        SDL_RenderFillRect(renderer, &fileNameDisplay);

        // Zones d'affichage pour les chiffres
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        float digitSize = pianoPanel.h - 50;
        SDL_FRect leftDigit = {pianoPanel.x + 30, pianoPanel.y + 25, digitSize, digitSize};
        SDL_FRect rightDigit = {pianoPanel.x + pianoPanel.w - digitSize - 30, pianoPanel.y + 25, digitSize, digitSize};
        SDL_RenderFillRect(renderer, &leftDigit);
        SDL_RenderFillRect(renderer, &rightDigit);

        // Rendre le piano
        pianoView->render(renderer);
    }
};

int main(int argc, char *argv[]) {
    // Initializer SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Dimensions de la fenêtre
    int windowWidth = 1440;
    int windowHeight = 1024;

    SDL_Window *window = SDL_CreateWindow("Piano Interface", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Créer le contrôleur principal
    PianoAppController appController(windowWidth, windowHeight);

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

                int buttonClicked = appController.handleButtonClick(mouseX, mouseY);
                appController.processButtonAction(buttonClicked);
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // Rendre l'interface utilisateur et le piano
        appController.render(renderer, windowWidth, windowHeight);

        // Présenter le rendu
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyer et quitter
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}