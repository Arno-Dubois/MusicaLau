#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"

// Déclarations anticipées
class VideoGame;

class VideoGameView;

class VideoGameAppController : public Controller {
private:
    VideoGame *videoGame;
    VideoGameView *videoGameView;

public:
    VideoGameAppController(int windowWidth, int windowHeight);

    ~VideoGameAppController();

    void processButtonAction(int buttonIndex);

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};