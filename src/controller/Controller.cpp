//
// Created by coren on 24/03/2025.
//

#include "../../include/Controller/Controller.h"

Controller::Controller(MusicApp::Audio::AudioEngine* audioE) : audioEngine(audioE) {
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
    for (int i = 0; i < 8; i++) {
        Button btn;
        btn.rect = {startX + i * (buttonWidth + buttonSpacing), toolbarY, buttonWidth, buttonHeight};
        btn.color = darkGray;
        btn.name = buttonNames[i];
        btn.icon = nullptr;
        buttons.push_back(btn);
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

void Controller::renderButtons(SDL_Renderer *renderer, const std::vector<Button> &buttons) {
    for (size_t i = 0; i < buttons.size(); i++) {
        const auto &button = buttons[i];
        // Fond du bouton
        SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
        SDL_RenderFillRect(renderer, &button.rect);

        // Dessiner les bordures du bouton
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Bordure légèrement plus sombre
        SDL_RenderRect(renderer, &button.rect);

        // Calculer le centre du bouton pour positionner les icônes
        float centerX = button.rect.x + button.rect.w / 2;
        float centerY = button.rect.y + button.rect.h / 2;

        // Dessiner l'icône ou le texte en fonction de l'index du bouton
        switch (i) {
            case 0: // Select
            {
                // Dessiner le texte "Select" centré dans le bouton
                renderTextCentered(renderer, centerX, centerY, button.name, {255, 255, 255, 255});
                break;
            }

            case 1: // Remove Octave (-)
            {
                // Dessiner un symbole minus
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_FRect minus = {centerX - 15, centerY - 2, 30, 4};
                SDL_RenderFillRect(renderer, &minus);

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Remove Octave", {200, 200, 200, 255});
                break;
            }

            case 2: // Add Octave (+)
            {
                // Dessiner un symbole plus (+)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // Ligne horizontale
                SDL_FRect plusH = {centerX - 15, centerY - 2, 30, 4};
                SDL_RenderFillRect(renderer, &plusH);
                // Ligne verticale
                SDL_FRect plusV = {centerX - 2, centerY - 15, 4, 30};
                SDL_RenderFillRect(renderer, &plusV);

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Add Octave", {200, 200, 200, 255});
                break;
            }

            case 3: // Import File
            {
                // Dessiner une icône de fichier
                drawFileIcon(renderer, centerX, centerY, 30, {255, 255, 255, 255});

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Import File", {200, 200, 200, 255});
                break;
            }

            case 4: // Play Song
            {
                // Dessiner un triangle de lecture (blanc)
                drawPlayIcon(renderer, centerX, centerY, 20, {200, 200, 200, 255});

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Play Song", {200, 200, 200, 255});
                break;
            }

            case 5: // Start Recording
            {
                // Dessiner un triangle de lecture (rouge)
                drawPlayIcon(renderer, centerX, centerY, 20, {255, 50, 50, 255});

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Start Recording", {200, 200, 200, 255});
                break;
            }

            case 6: // Export
            {
                // Dessiner une flèche vers le haut
                drawUpArrow(renderer, centerX, centerY, 20, {255, 50, 50, 255});

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Export", {200, 200, 200, 255});
                break;
            }

            case 7: // Finish Recording
            {
                // Dessiner un cercle avec un carré à l'intérieur
                drawStopIcon(renderer, centerX, centerY, 20, {255, 50, 50, 255});

                // Texte descriptif en petit en bas du bouton
                renderSmallText(renderer, centerX, centerY + 20, "Finish Recording", {200, 200, 200, 255});
                break;
            }
        }
    }
}

// Fonction utilitaire pour dessiner un texte centré (simulé sans SDL_ttf)
void Controller::renderTextCentered(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text,
                                    SDL_Color color) {
    // Comme nous n'utilisons pas SDL_ttf pour le moment, je vais dessiner quelque chose de plus propre qu'un simple rectangle
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    float charWidth = 10.0f;   // Largeur moyenne d'un caractère
    float charHeight = 16.0f;  // Hauteur du texte

    float textWidth = text.length() * charWidth;
    float startX = centerX - textWidth / 2;
    float startY = centerY - charHeight / 2;

    // Dessine chaque caractère (simplifié)
    for (size_t i = 0; i < text.length(); i++) {
        float x = startX + i * charWidth;

        // Si c'est un espace, ne rien dessiner
        if (text[i] == ' ') continue;

        // Dessiner un simple caractère rectangulaire
        SDL_FRect charRect = {x + 1, startY, charWidth - 2, charHeight};
        SDL_RenderFillRect(renderer, &charRect);
    }
}

// Fonction pour dessiner un texte en petit
void Controller::renderSmallText(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text,
                                 SDL_Color color) {
    // Version plus petite et plus fine pour le texte secondaire
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    float charWidth = 5.0f;    // Largeur réduite pour le texte petit
    float charHeight = 8.0f;   // Hauteur réduite

    float textWidth = text.length() * charWidth;
    float startX = centerX - textWidth / 2;
    float startY = centerY - charHeight / 2;

    // Dessine chaque caractère (simplifié)
    for (size_t i = 0; i < text.length(); i++) {
        float x = startX + i * charWidth;

        // Si c'est un espace, ne rien dessiner
        if (text[i] == ' ') continue;

        // Dessiner un simple caractère rectangulaire plus fin
        SDL_FRect charRect = {x + 0.5f, startY, charWidth - 1, charHeight};
        SDL_RenderFillRect(renderer, &charRect);
    }
}

// Fonction pour dessiner une icône de fichier
void Controller::drawFileIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Rectangle principal du document
    SDL_FRect docRect = {centerX - size / 2, centerY - size / 2, size, size};
    SDL_RenderRect(renderer, &docRect);

    // Coin plié
    float foldSize = size / 4;
    SDL_FPoint fold[] = {
            {centerX + size / 2 - foldSize, centerY - size / 2},               // Coin supérieur droit moins fold
            {centerX + size / 2,            centerY - size / 2 + foldSize},               // Coin droit du pli
            {centerX + size / 2 - foldSize, centerY - size / 2 + foldSize}     // Coin gauche du pli
    };

    // Dessiner les lignes du pli
    for (int i = 0; i < 2; i++) {
        SDL_RenderLine(renderer, fold[i].x, fold[i].y, fold[i + 1].x, fold[i + 1].y);
    }

    // Ligne du pli vers le coin du document
    SDL_RenderLine(renderer, fold[0].x, fold[0].y, fold[2].x, fold[2].y);

    // Ajouter un + dans le document
    float plusSize = size / 3;
    SDL_FRect plusH = {centerX - plusSize / 2, centerY, plusSize, size / 10};
    SDL_FRect plusV = {centerX - size / 20, centerY - plusSize / 2, size / 10, plusSize};
    SDL_RenderFillRect(renderer, &plusH);
    SDL_RenderFillRect(renderer, &plusV);
}

// Fonction pour dessiner une icône de lecture (triangle)
void Controller::drawPlayIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Coordonnées du triangle (pointe vers la droite)
    // Créer un triangle plus large et plus court que précédemment
    SDL_FPoint triangle[3];
    float halfHeight = size / 2;
    float width = size * 0.9f; // Triangle un peu plus large

    triangle[0] = {centerX - width / 3, centerY - halfHeight}; // Sommet supérieur gauche
    triangle[1] = {centerX + width * 2 / 3, centerY};            // Sommet droit (pointe)
    triangle[2] = {centerX - width / 3, centerY + halfHeight}; // Sommet inférieur gauche

    // Remplir le triangle avec une méthode plus précise
    float minY = triangle[0].y;
    float maxY = triangle[2].y;

    for (float y = minY; y <= maxY; y += 0.5f) { // On utilise un pas plus petit pour plus de précision
        // Calcul de la limite gauche (toujours au même x car c'est un triangle)
        float leftX = triangle[0].x;

        // Calcul de la limite droite en interpolant entre le sommet et la pointe
        float progress = (y - minY) / (maxY - minY); // 0 à 1
        float rightX;

        // Si on est dans la moitié supérieure
        if (progress <= 0.5f) {
            float normalizedProgress = progress * 2; // 0 à 1 pour la partie supérieure
            rightX = triangle[0].x + normalizedProgress * (triangle[1].x - triangle[0].x);
        }
            // Si on est dans la moitié inférieure
        else {
            float normalizedProgress = (progress - 0.5f) * 2; // 0 à 1 pour la partie inférieure
            rightX = triangle[1].x - normalizedProgress * (triangle[1].x - triangle[2].x);
        }

        SDL_RenderLine(renderer, leftX, y, rightX, y);
    }

    // Dessiner le contour du triangle
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        SDL_RenderLine(renderer, triangle[i].x, triangle[i].y, triangle[next].x, triangle[next].y);
    }
}

// Fonction pour dessiner une flèche vers le haut
void Controller::drawUpArrow(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Dessiner la flèche avec un remplissage
    SDL_FPoint arrow[] = {
            {centerX,            centerY - size / 2},                  // Pointe de la flèche
            {centerX - size / 3, centerY - size / 6},       // Coin gauche
            {centerX - size / 6, centerY - size / 6},       // Intérieur gauche
            {centerX - size / 6, centerY + size / 2},       // Bas gauche
            {centerX + size / 6, centerY + size / 2},       // Bas droit
            {centerX + size / 6, centerY - size / 6},       // Intérieur droit
            {centerX + size / 3, centerY - size / 6}        // Coin droit
    };

    // Remplir l'intérieur de la flèche
    for (float y = arrow[0].y; y <= arrow[3].y; y += 1.0f) {
        float left = centerX - size / 6;
        float right = centerX + size / 6;

        // Pour la partie triangulaire (pointe de la flèche)
        if (y < arrow[1].y) {
            float t = (y - arrow[0].y) / (arrow[1].y - arrow[0].y);
            left = centerX - t * size / 3;
            right = centerX + t * size / 3;
        }

        SDL_RenderLine(renderer, left, y, right, y);
    }

    // Dessiner les contours de la flèche
    for (int i = 0; i < 7; i++) {
        int next = (i + 1) % 7;
        SDL_RenderLine(renderer, arrow[i].x, arrow[i].y, arrow[next].x, arrow[next].y);
    }
}

// Fonction pour dessiner une icône d'arrêt (cercle avec carré intérieur)
void Controller::drawStopIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Approximation d'un cercle par un octogone avec remplissage
    const int numPoints = 32;
    SDL_FPoint circle[numPoints];

    for (int i = 0; i < numPoints; i++) {
        float angle = i * 2 * M_PI / numPoints;
        circle[i].x = centerX + size / 2 * cos(angle);
        circle[i].y = centerY + size / 2 * sin(angle);
    }

    // Remplir l'intérieur du cercle
    for (float y = centerY - size / 2; y <= centerY + size / 2; y += 1.0f) {
        float dy = y - centerY;
        float dx = sqrt((size / 2) * (size / 2) - dy * dy);

        // Cercle: (x - cx)² + (y - cy)² = r²
        SDL_RenderLine(renderer, centerX - dx, y, centerX + dx, y);
    }

    // Dessiner le contour du cercle
    for (int i = 0; i < numPoints; i++) {
        int next = (i + 1) % numPoints;
        SDL_RenderLine(renderer, circle[i].x, circle[i].y, circle[next].x, circle[next].y);
    }

    // Dessiner le carré à l'intérieur
    float squareSize = size / 3;
    SDL_FRect square = {
            centerX - squareSize / 2,
            centerY - squareSize / 2,
            squareSize,
            squareSize
    };
    SDL_RenderFillRect(renderer, &square);
}