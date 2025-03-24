#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

// Structure pour stocker les informations des boutons
struct Button {
    SDL_FRect rect;
    std::string text;
    SDL_Color color;
    SDL_Color textColor;
    SDL_Texture *iconTexture;
    bool isIcon;
};

// Couleurs utilisées dans l'application
const SDL_Color DARK_BROWN = {60, 40, 40, 255};
const SDL_Color LIGHT_BLUE = {173, 216, 230, 255};
const SDL_Color DARK_GRAY = {40, 40, 40, 255};
const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color RED = {255, 50, 50, 255};

// Fonction pour créer une texture à partir d'un texte
SDL_Texture *createTextTexture(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, SDL_Color color) {
    // SDL3 nécessite toujours la longueur du texte
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) {
        std::cerr << "TTF_RenderText_Blended Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    return texture;
}

// Fonction pour obtenir les dimensions d'une texture
bool getTextureSize(SDL_Texture *texture, int *width, int *height) {
    SDL_PropertiesID props = SDL_GetTextureProperties(texture);
    if (props) {
        *width = (int) SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0);
        *height = (int) SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
        return true;
    }
    return false;
}

// Fonction pour dessiner un bouton
void drawButton(SDL_Renderer *renderer, const Button &button, TTF_Font *font) {
    // Dessiner le fond du bouton
    SDL_SetRenderDrawColor(renderer,
                           button.color.r,
                           button.color.g,
                           button.color.b,
                           button.color.a);
    SDL_RenderFillRect(renderer, &button.rect);

    // Dessiner le texte ou l'icône
    if (button.isIcon && button.iconTexture) {
        // Si c'est une icône, dessiner la texture de l'icône
        int iconWidth, iconHeight;
        if (getTextureSize(button.iconTexture, &iconWidth, &iconHeight)) {
            SDL_FRect iconRect = {
                    button.rect.x + (button.rect.w - iconWidth) / 2,
                    button.rect.y + (button.rect.h - iconHeight) / 2,
                    (float) iconWidth,
                    (float) iconHeight
            };

            SDL_RenderTexture(renderer, button.iconTexture, nullptr, &iconRect);
        }
    } else {
        // Si c'est du texte, créer et dessiner la texture du texte
        SDL_Texture *textTexture = createTextTexture(renderer, font, button.text, button.textColor);
        if (textTexture) {
            int textWidth, textHeight;
            if (getTextureSize(textTexture, &textWidth, &textHeight)) {
                SDL_FRect textRect = {
                        button.rect.x + (button.rect.w - textWidth) / 2,
                        button.rect.y + (button.rect.h - textHeight) / 2,
                        (float) textWidth,
                        (float) textHeight
                };

                SDL_RenderTexture(renderer, textTexture, nullptr, &textRect);
            }
            SDL_DestroyTexture(textTexture);
        }
    }
}

// Fonction principale
int main(int argc, char *argv[]) {
    // Initialiser SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialiser SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Créer la fenêtre avec les dimensions spécifiées
    SDL_Window *window = SDL_CreateWindow("Application Musicale", 1440, 1024, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Créer le renderer (avec la nouvelle signature de SDL3)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Charger la police
    TTF_Font *font = TTF_OpenFont("arial.ttf", 16); // Remplacez par le chemin de votre police
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Définir les boutons de la barre d'outils
    std::vector<Button> buttons;

    // Bouton Select
    buttons.push_back({
                              {64, 60, 85, 35},  // x, y, width, height
                              "Select",
                              DARK_GRAY,
                              WHITE,
                              nullptr,
                              false
                      });

    // Bouton Remove Octave
    buttons.push_back({
                              {173, 60, 85, 35},
                              "Remove Octave",
                              DARK_GRAY,
                              WHITE,
                              nullptr,
                              false
                      });

    // Bouton Add Octave
    buttons.push_back({
                              {282, 60, 85, 35},
                              "Add Octave",
                              DARK_GRAY,
                              WHITE,
                              nullptr,
                              false
                      });

    // Bouton Import File
    buttons.push_back({
                              {391, 60, 85, 35},
                              "Import File",
                              DARK_GRAY,
                              WHITE,
                              nullptr,
                              false
                      });

    // Bouton Play Song
    buttons.push_back({
                              {500, 60, 85, 35},
                              "Play Song",
                              DARK_GRAY,
                              WHITE,
                              nullptr,
                              false
                      });

    // Bouton Start Recording
    buttons.push_back({
                              {609, 60, 85, 35},
                              "Start Recording",
                              DARK_GRAY,
                              RED,
                              nullptr,
                              false
                      });

    // Bouton Export
    buttons.push_back({
                              {718, 60, 85, 35},
                              "Export",
                              DARK_GRAY,
                              RED,
                              nullptr,
                              false
                      });

    // Bouton Finish Recording
    buttons.push_back({
                              {827, 60, 85, 35},
                              "Finish Recording",
                              DARK_GRAY,
                              RED,
                              nullptr,
                              false
                      });

    // Créer le panneau d'instrument
    SDL_FRect instrumentPanel = {100, 220, 1240, 50};

    // Boucle principale
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Gérer les événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, DARK_BROWN.r, DARK_BROWN.g, DARK_BROWN.b, DARK_BROWN.a);
        SDL_RenderClear(renderer);

        // Dessiner les boutons
        for (const auto &button: buttons) {
            drawButton(renderer, button, font);
        }

        // Dessiner le panneau d'instrument
        SDL_SetRenderDrawColor(renderer, LIGHT_BLUE.r, LIGHT_BLUE.g, LIGHT_BLUE.b, LIGHT_BLUE.a);
        SDL_RenderFillRect(renderer, &instrumentPanel);

        // Dessiner le texte "INSTRUMENT ACTUEL (PIANO)"
        SDL_Texture *instrumentText = createTextTexture(renderer, font, "INSTRUMENT ACTUEL (PIANO)", DARK_GRAY);
        if (instrumentText) {
            int textWidth, textHeight;
            if (getTextureSize(instrumentText, &textWidth, &textHeight)) {
                SDL_FRect textRect = {
                        instrumentPanel.x + (instrumentPanel.w - textWidth) / 2,
                        instrumentPanel.y + (instrumentPanel.h - textHeight) / 2,
                        (float) textWidth,
                        (float) textHeight
                };

                SDL_RenderTexture(renderer, instrumentText, nullptr, &textRect);
            }
            SDL_DestroyTexture(instrumentText);
        }

        // Afficher le rendu
        SDL_RenderPresent(renderer);
    }

    // Nettoyer et quitter
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}