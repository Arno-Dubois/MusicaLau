#include "menu.h"
#include <SDL3_ttf/SDL_ttf.h>


Menu::Menu(SDL_Renderer *renderer, int width, int height)
        : renderer(renderer), width(width), height(height) {

    // Chargement des textures et création des éléments de menu
    menuItems.push_back({loadTexture("path/to/select.png"), {10, 10, 50, 30}, "select"});
    menuItems.push_back({loadTexture("path/to/remove_octave.png"), {70, 10, 80, 30}, "remove_octave"});
    // ... ajouter les autres éléments de menu

    // Création de la texture pour le texte "INSTRUMENT ACTUEL (PIANO)"
    SDL_Color textColor = {0, 0, 0, 255}; // Couleur noire
    instrumentTextTexture = createTextTexture("INSTRUMENT ACTUEL (PIANO)", textColor);
}

Menu::~Menu() {
    // Libération des textures
    for (auto &item: menuItems) {
        SDL_DestroyTexture(item.texture);
    }
    SDL_DestroyTexture(instrumentTextTexture);
}

void Menu::handleEvent(const SDL_Event &event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);


        for (auto &item: menuItems) {
            if (x >= item.rect.x && x < item.rect.x + item.rect.w &&
                y >= item.rect.y && y < item.rect.y + item.rect.h) {
                handleMenuItemClick(item.action);
                break;
            }
        }
    }
}

void Menu::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Affichage des éléments de menu
    for (auto &item: menuItems) {
        SDL_RenderTexture(renderer, texture, NULL, &rect);
    }

    // Affichage du texte "INSTRUMENT ACTUEL (PIANO)"
    SDL_Rect textRect = {10, 60, 200, 30}; // Position et taille du texte
    SDL_RenderTexture(renderer, texture, NULL, &rect);

    SDL_RenderPresent(renderer);
}

SDL_Texture *Menu::loadTexture(const std::string &path) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = SDL_LoadBMP(path.c_str());
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
    } else {
        SDL_Log("Erreur de chargement de texture : %s", SDL_GetError());
    }
    return texture;
}

SDL_Texture *Menu::createTextTexture(const std::string &text, SDL_Color color) {
    SDL_Texture *texture = nullptr;
    TTF_Font *font = TTF_OpenFont("path/to/your/font.ttf", 24); // Charger la police
    if (font) {
        SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
        } else {
            SDL_Log("Erreur de création de texture de texte : %s", SDL_GetError());
        }
        TTF_CloseFont(font);
    } else {
        SDL_Log("Erreur de chargement de police : %s", TTF_GetError());
    }
    return texture;
}

void Menu::handleMenuItemClick(const std::string &action) {
    // Gérer les actions associées aux éléments de menu
    if (action == "select") {
        // Action pour le bouton "Select"
    } else if (action == "remove_octave") {
        // Action pour le bouton "Remove Octave"
    }
    // ... gérer les autres actions
}
