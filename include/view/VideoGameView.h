#pragma once

#include "SDL3/SDL.h"
#include "View.h"
#include "../Model/VideoGame.h"

class VideoGameView : public View {
private:
    VideoGame *videoGame;

public:
    explicit VideoGameView(VideoGame *videoGame);

    void render(SDL_Renderer *renderer, int windowWidth = 0, int windowHeight = 0) override;
};