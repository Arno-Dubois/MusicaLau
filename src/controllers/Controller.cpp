//
// Created by coren on 24/03/2025.
//

#include "../../include/controllers/Controller.h"

Controller::Controller() {
    initializeButtons();
}

void Controller::initializeButtons() {
    // Tableau des noms de boutons
    std::vector<std::string> buttonNames = {
            "Select",
            "Remove Octave",
            "Add Octave",
            "Import File",
            "Play Song",
            "Start Recording",
            "Export",
            "Finish Recording"
    };

    // Créer les 8 boutons avec les bonnes dimensions et espacements
    SDL_Color darkGray = {46, 46, 46, 255};
    for(int i = 0; i < 8; i++) {
        buttons.push_back({
                                  {startX + i * (buttonWidth + buttonSpacing), toolbarY, buttonWidth, buttonHeight},
                                  darkGray,
                                  buttonNames[i],
                                  nullptr
                          });
    }
}

int Controller::handleButtonClick(float x, float y) {
    for (size_t i = 0; i < buttons.size(); i++) {
        if (x >= buttons[i].rect.x && x <= buttons[i].rect.x + buttons[i].rect.w &&
            y >= buttons[i].rect.y && y <= buttons[i].rect.y + buttons[i].rect.h) {
            return i;  // Retourne l'index du bouton cliqué
        }
    }
    return -1;  // Aucun bouton cliqué
}

void Controller::renderButtons(SDL_Renderer* renderer, const std::vector<Button>& buttons) {
    for(const auto& button : buttons) {
        // Fond du bouton
        SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
        SDL_RenderFillRect(renderer, &button.rect);

        // Le texte des boutons pourrait être ajouté ici avec SDL_ttf
    }
}