#ifndef MENU_H
#define MENU_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Menu {
public:
    Menu(SDL_Renderer *renderer, int width, int height);

    ~Menu();

    void handleEvent(const SDL_Event &event);

    void render();

private:
    SDL_Renderer *renderer;
    int width;
    int height;

    // Structure pour représenter un élément de menu (onglet ou bouton)
    struct MenuItem {
        SDL_Texture *texture;
        SDL_Rect rect;
        std::string action; // Action associée à l'élément
    };

    std::vector<MenuItem> menuItems;

    // Texture pour le texte "INSTRUMENT ACTUEL (PIANO)"
    SDL_Texture *instrumentTextTexture;

    // Méthode pour charger une texture à partir d'un fichier
    SDL_Texture *loadTexture(const std::string &path);

    // Méthode pour créer une texture à partir d'un texte
    SDL_Texture *createTextTexture(const std::string &text, SDL_Color color);

    // Méthode pour gérer les clics sur les éléments de menu
    void handleMenuItemClick(const std::string &action);
};


#endif