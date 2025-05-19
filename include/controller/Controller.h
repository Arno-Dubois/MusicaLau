#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <string>
#include <cmath> // Pour M_PI
#include "../View/View.h"

namespace MusicApp {
    namespace Audio {
        class AudioEngine;
    }
}

class Controller {
protected:
    std::vector<Button> buttons;
    TTF_Font *font;

    // UI parameters
    float buttonWidth = 121.0f;
    float buttonHeight = 90.0f;
    float buttonSpacing = 50.0f;
    float toolbarY = 20.0f;
    float startX = 63.0f;

    // Suivi des dimensions actuelles de la fenêtre
    int currentWindowWidth;
    int currentWindowHeight;

    // Audio engine
    MusicApp::Audio::AudioEngine *audioEngine;

public:
    Controller();

    Controller(MusicApp::Audio::AudioEngine *audioE);

    virtual ~Controller();

    void initializeButtons();

    int handleButtonClick(float x, float y);

    virtual void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) = 0;

    void renderButtons(SDL_Renderer *renderer, const std::vector<Button> &buttons);

    // Fonctions utilitaires pour dessiner les icônes
    void
    renderTextCentered(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color);

    void
    renderSmallText(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color);

    void drawFileIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    void drawPlayIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    void drawUpArrow(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    void drawStopIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    // Nouvelles méthodes pour calculer les dimensions relatives
    float calculateRelativeWidth(int windowWidth, float percentage);

    float calculateRelativeHeight(int windowHeight, float percentage);

    void updateDimensions(int windowWidth, int windowHeight);
};