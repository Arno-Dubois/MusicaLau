#pragma once

#include "Model/VideoGame.h"
#include "View/View.h"
#include <SDL3/SDL.h>

class VideoGameView : public View {
private:
  VideoGame *videoGame;

public:
  explicit VideoGameView(VideoGame *videoGame);

  void render(SDL_Renderer *renderer, int windowWidth = 0,
              int windowHeight = 0) override;
};