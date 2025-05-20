#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"
#include "../Model/VideoGame.h"
#include "../View/VideoGameView.h"

class VideoGameAppController : public Controller {
private:
    VideoGame *videoGame;
    VideoGameView *videoGameView;

public:
    VideoGameAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE);

    ~VideoGameAppController();

    void processButtonAction(int buttonIndex);

    void handleVideoGameKeyClick(float mouseX, float mouseY);

    void handleVideoGameKeyHover(float mouseX, float mouseY);

    VideoGame *getVideoGame() const { return videoGame; }

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};