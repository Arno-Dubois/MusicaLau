#include "../../include/utils/DropdownMenu.h"

DropdownMenu::DropdownMenu(float x, float y, float width, float height, const std::string &headerLabel)
        : isOpen(false), selectedIndex(-1) {

    headerRect = {x, y, width, height};

    // Couleurs par défaut du menu
    headerColor = {185, 211, 230, 255}; // Bleu clair
    textColor = {0, 0, 0, 255}; // Noir
    itemColor = {220, 220, 220, 255}; // Gris clair
    hoverColor = {150, 190, 220, 255}; // Bleu plus foncé
    darkBlue = {100, 150, 200, 255};

    // Ajouter le label d'en-tête
    itemLabels.push_back(headerLabel);
}

void DropdownMenu::addItem(const std::string &label, std::function<void()> action) {
    // Calculer la position du nouvel élément
    float itemHeight = headerRect.h;
    // Positionner chaque élément directement sous le précédent, en commençant sous l'en-tête
    float itemY = headerRect.y + (itemRects.size() + 1) * itemHeight;

    // Créer un rectangle pour le nouvel élément
    SDL_FRect itemRect = {headerRect.x, itemY, headerRect.w, itemHeight};

    // Ajouter l'élément aux collections
    itemRects.push_back(itemRect);
    itemLabels.push_back(label);
    itemActions.push_back(action);
}

void DropdownMenu::render(SDL_Renderer *renderer) {
    // Dessiner l'en-tête
    SDL_SetRenderDrawColor(renderer, headerColor.r, headerColor.g, headerColor.b, headerColor.a);
    SDL_RenderFillRect(renderer, &headerRect);

    // Dessiner une bordure noire autour de l'en-tête
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &headerRect);

    // Dessiner un indicateur visuel pour le titre - une petite barre plus foncée
    SDL_SetRenderDrawColor(renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_FRect titleBar = {
            headerRect.x + 10,  // Un peu d'espace à gauche
            headerRect.y + 5,   // Un peu d'espace en haut
            30,                 // Largeur fixe
            headerRect.h - 10   // Presque toute la hauteur
    };
    SDL_RenderFillRect(renderer, &titleBar);

    // Simuler l'affichage du texte sélectionné dans l'en-tête
    if (itemLabels.size() > 0) {
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        // Calculer le centre du texte
        float textX = headerRect.x + 50; // Après la barre de titre
        float textY = headerRect.y + headerRect.h / 2 - 5; // Centré verticalement

        // Dessiner un rectangle pour simuler le texte
        int textLength = itemLabels[0].length() * 10; // Largeur approximative du texte
        SDL_FRect textRect = {textX, textY, static_cast<float>(textLength), 10};
        SDL_RenderFillRect(renderer, &textRect);
    }

    // Positionnement et dimensions pour l'indicateur de menu déroulant 
    float triangleSize = headerRect.h * 0.3f;
    float indicatorX = headerRect.x + headerRect.w - triangleSize * 3;
    float indicatorY = headerRect.y + (headerRect.h - triangleSize) / 2;

    // Dessiner un petit cercle pour le fond de l'indicateur
    float circleSize = triangleSize * 2.0f;
    SDL_FRect circleRect = {
            indicatorX - triangleSize / 2,
            headerRect.y + (headerRect.h - circleSize) / 2,
            circleSize,
            circleSize
    };
    SDL_SetRenderDrawColor(renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
    SDL_RenderFillRect(renderer, &circleRect);

    // Centre exact du cercle
    float circleCenterX = circleRect.x + circleRect.w / 2;
    float circleCenterY = circleRect.y + circleRect.h / 2;

    // Dessiner le triangle centré dans le carré
    float triangleLeft = circleCenterX - triangleSize / 2;
    float triangleTop = circleCenterY - triangleSize / 2;

    // Points du triangle
    SDL_FPoint points[3];
    points[0] = {triangleLeft, triangleTop};
    points[1] = {triangleLeft + triangleSize, triangleTop};
    points[2] = {triangleLeft + triangleSize / 2, triangleTop + triangleSize};

    // On trace un triangle en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        SDL_RenderLine(renderer,
                       points[i].x, points[i].y,
                       points[next].x, points[next].y);
    }

    // Si le menu est ouvert, dessiner les éléments
    if (isOpen) {
        for (size_t i = 0; i < itemRects.size(); ++i) {
            // Utiliser une couleur différente pour l'élément survolé
            if (static_cast<int>(i) == selectedIndex) {
                SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
            } else {
                SDL_SetRenderDrawColor(renderer, itemColor.r, itemColor.g, itemColor.b, itemColor.a);
            }

            SDL_RenderFillRect(renderer, &itemRects[i]);

            // Dessiner une bordure noire
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderRect(renderer, &itemRects[i]);

            // Ajouter une séparation entre les éléments
            if (i > 0) {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                SDL_FRect separatorLine = {
                        itemRects[i].x + 5,
                        itemRects[i].y,
                        itemRects[i].w - 10,
                        1
                };
                SDL_RenderFillRect(renderer, &separatorLine);
            }

            // Simuler l'affichage du texte pour chaque élément
            if (i < itemRects.size() && i + 1 < itemLabels.size()) {
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
                float textX = itemRects[i].x + 50; // Aligner avec le texte principal
                float textY = itemRects[i].y + itemRects[i].h / 2 - 5; // Centré verticalement

                // Dessiner un rectangle pour simuler le texte
                int textLength = itemLabels[i + 1].length() * 10;
                SDL_FRect textRect = {textX, textY, static_cast<float>(textLength), 10};
                SDL_RenderFillRect(renderer, &textRect);
            }

            // Ajouter un indicateur de sélection pour l'élément actif
            if (i < itemRects.size() && i + 1 < itemLabels.size() && itemLabels[i + 1] == itemLabels[0]) {
                SDL_SetRenderDrawColor(renderer, darkBlue.r, darkBlue.g, darkBlue.b, darkBlue.a);
                SDL_FRect activeIndicator = {
                        itemRects[i].x + 10,
                        itemRects[i].y + itemRects[i].h / 2 - 5,
                        10,
                        10
                };
                SDL_RenderFillRect(renderer, &activeIndicator);
            }
        }
    }
}

void DropdownMenu::updateHoverState(float x, float y) {
    selectedIndex = -1;

    if (!isOpen) return;

    for (size_t i = 0; i < itemRects.size(); ++i) {
        if (x >= itemRects[i].x && x <= itemRects[i].x + itemRects[i].w &&
            y >= itemRects[i].y && y <= itemRects[i].y + itemRects[i].h) {
            selectedIndex = i;
            break;
        }
    }
}

bool DropdownMenu::handleClick(float x, float y) {
    // Vérifier si le clic est sur l'en-tête
    if (x >= headerRect.x && x <= headerRect.x + headerRect.w &&
        y >= headerRect.y && y <= headerRect.y + headerRect.h) {
        toggleOpen();
        return true;
    }

    // Si le menu est ouvert, vérifier si le clic est sur un élément
    if (isOpen) {
        for (size_t i = 0; i < itemRects.size(); ++i) {
            if (x >= itemRects[i].x && x <= itemRects[i].x + itemRects[i].w &&
                y >= itemRects[i].y && y <= itemRects[i].y + itemRects[i].h) {

                // Exécuter l'action associée à cet élément
                if (i < itemActions.size() && itemActions[i]) {
                    itemActions[i]();
                }

                // Fermer le menu après la sélection
                isOpen = false;
                return true;
            }
        }

        // Si le clic est en dehors du menu, le fermer
        isOpen = false;
        return true;
    }

    return false;
}

void DropdownMenu::toggleOpen() {
    isOpen = !isOpen;
}