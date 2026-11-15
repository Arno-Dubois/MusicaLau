#pragma once

#include "Model/Xylophone.h"
#include "View/View.h"
#include <SDL3/SDL.h>

class XylophoneView : public View {
private:
  Xylophone *xylophone;

public:
  explicit XylophoneView(Xylophone *xylophone);

  void render(SDL_Renderer *renderer, int windowWidth = 0,
              int windowHeight = 0) override;
};