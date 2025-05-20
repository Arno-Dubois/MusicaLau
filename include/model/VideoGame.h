#pragma once

#include "SDL3/SDL.h"
#include <string>
#include <vector>

struct GameButton {
    SDL_FRect rect;  // Position et taille du bouton
    bool isHovered;  // État de survol
    int note;        // Note associée (index de la touche)
};

// Structure pour les contrôles de la console
enum ConsoleControlType {
    DPAD_UP,
    DPAD_RIGHT,
    DPAD_DOWN,
    DPAD_LEFT,
    BUTTON_A,
    BUTTON_B,
    BUTTON_START,
    BUTTON_SELECT
};

struct ConsoleControl {
    SDL_FRect rect;             // Position et taille du contrôle
    bool isHovered;             // État de survol
    ConsoleControlType type;    // Type de contrôle
};

class VideoGame {
private:
    float x, y, width, height;
    int keys;
    std::vector<GameButton> buttons;  // Boutons de la console
    std::vector<ConsoleControl> controls; // Contrôles de la console
    int hoveredButtonIndex;           // Index du bouton survolé (-1 si aucun)
    int hoveredControlIndex;          // Index du contrôle survolé (-1 si aucun)

    // Calculer la disposition des boutons
    void calculateButtonsLayout();

    // Calculer la disposition des contrôles
    void calculateControlsLayout();

public:
    VideoGame(float x = 0, float y = 0, float width = 0, float height = 0,
              int keys = 24); // 24 touches = 2 octaves complètes

    float getX() const { return x; }

    float getY() const { return y; }

    float getWidth() const { return width; }

    float getHeight() const { return height; }

    int getKeys() const { return keys; }

    // Obtenir le nom de la note à une position donnée
    std::string getNoteAt(float mouseX, float mouseY) const;

    // Obtenir le nom de la note et les informations du rectangle à une position donnée
    bool getNoteInfoAt(float mouseX, float mouseY, std::string &noteName, SDL_FRect &buttonRect) const;

    // Obtenir l'index du bouton à une position donnée
    int getButtonAt(float mouseX, float mouseY) const;

    // Obtenir l'index du contrôle à une position donnée
    int getControlAt(float mouseX, float mouseY) const;

    // Méthodes pour gérer le survol des touches
    bool updateHoveredButton(float mouseX, float mouseY);

    // Méthode pour gérer le survol des contrôles
    bool updateHoveredControl(float mouseX, float mouseY);

    void clearHoveredButtons();

    void clearHoveredControls();

    // Accéder aux boutons pour le rendu
    const std::vector<GameButton> &getButtons() const { return buttons; }

    // Accéder aux contrôles pour le rendu
    const std::vector<ConsoleControl> &getControls() const { return controls; }

    void setPosition(float newX, float newY);

    void setDimensions(float newWidth, float newHeight);

    void addKeys();

    void removeKeys();
};