#pragma once

#include "SDL3/SDL.h"
#include <vector>
#include <string>

struct Button {
    SDL_FRect rect;
    SDL_Color color;
    std::string name;
    SDL_Texture *icon;  // Pour ajouter des icônes plus tard si nécessaire
};

class View {
protected:
    // Couleurs communes
    SDL_Color darkGray = {46, 46, 46, 255};
    SDL_Color brownColor = {90, 60, 55, 255};

    // Dimensions communes
    float topBarHeight = 129.0f;

public:
    virtual ~View() = default;

    // La méthode render doit toujours recevoir les dimensions de la fenêtre pour permettre l'ajustement
    virtual void render(SDL_Renderer *renderer, int windowWidth = 0, int windowHeight = 0) = 0;

    // Méthode utilitaire pour dessiner les boutons
};