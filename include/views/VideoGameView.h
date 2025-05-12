#pragma once

#include <SDL3/SDL.h>

// Déclaration anticipée
class VideoGame;

class VideoGameView {
private:
    VideoGame *videoGame;

public:
    VideoGameView(VideoGame *videoGame);

    void render(SDL_Renderer *renderer);
};