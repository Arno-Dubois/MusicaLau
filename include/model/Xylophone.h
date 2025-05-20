#pragma once

#include "SDL3/SDL.h"
#include <string>
#include <vector>

struct XylophoneBar {
    SDL_FRect rect;         // Rectangle représentant la lame du xylophone
    bool isHovered;         // État de survol
};

class Xylophone {
private:
    float x, y, width, height;
    int bars;
    std::vector<XylophoneBar> xylophones;  // Stockage des lames du xylophone
    int hoveredBarIndex;                   // Index de la lame survolée (-1 si aucune)

public:
    Xylophone(float x = 0, float y = 0, float width = 0, float height = 0, int bars = 8);

    float getX() const { return x; }

    float getY() const { return y; }

    float getWidth() const { return width; }

    float getHeight() const { return height; }

    int getBars() const { return bars; }

    void setPosition(float newX, float newY);

    void setDimensions(float newWidth, float newHeight);

    void addBar();

    void removeBar();

    // Méthodes pour gérer le survol des lames
    bool updateHoveredBar(float mouseX, float mouseY);

    void clearHoveredBar();

    // Obtenir l'index de la lame à une position donnée
    int getBarAt(float mouseX, float mouseY) const;

    // Obtenir l'index de la lame et son rectangle à une position donnée
    bool getBarInfoAt(float mouseX, float mouseY, int &barIndex, SDL_FRect &barRect) const;

    // Accéder aux lames pour le rendu
    const std::vector<XylophoneBar> &getXylophoneBars() const { return xylophones; }

    // Calculer la disposition des lames
    void calculateBarsLayout();
};