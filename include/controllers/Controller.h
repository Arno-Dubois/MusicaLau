#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "../views/View.h"

class Controller {
protected:
    std::vector<Button> buttons;

    // UI parameters
    float buttonWidth = 121.0f;
    float buttonHeight = 90.0f;
    float buttonSpacing = 50.0f;
    float toolbarY = 20.0f;
    float startX = 63.0f;

public:
    Controller();
    virtual ~Controller() = default;

    void initializeButtons();
    int handleButtonClick(float x, float y);

    virtual void render(SDL_Renderer* renderer, int windowWidth, int windowHeight) = 0;

    void renderButtons(SDL_Renderer* renderer, const std::vector<Button>& buttons);

};
