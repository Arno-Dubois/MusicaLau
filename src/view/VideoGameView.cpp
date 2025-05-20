//
// Created by coren on 24/03/2025.
//

#include "../../include/View/VideoGameView.h"
#include "../../include/Model/VideoGame.h"
#include <SDL3/SDL.h>
#include <cmath>

VideoGameView::VideoGameView(VideoGame *videoGame) : videoGame(videoGame) {
}

void VideoGameView::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
    float x = videoGame->getX();
    float y = videoGame->getY();
    float w = videoGame->getWidth();
    float h = videoGame->getHeight();
    int numKeys = videoGame->getKeys();

    // Dimensions et positions de la console rétro - faire une grande console
    float consoleWidth = w * 0.98f; // Utiliser presque toute la largeur
    float consoleHeight = h * 0.95f; // Utiliser presque toute la hauteur
    float consoleX = x + (w - consoleWidth) / 2;
    float consoleY = y + (h - consoleHeight) / 2;

    // Couleurs 8-bit améliorées
    SDL_Color consoleDarkGray = {45, 45, 50, 255}; // Console externe plus sombre
    SDL_Color consoleGray = {70, 70, 75, 255}; // Intérieur de la console
    SDL_Color screenBorder = {30, 30, 35, 255};
    SDL_Color screenColor = {15, 15, 20, 255}; // Écran plus sombre
    SDL_Color pixelColors[] = {
            {255, 0,   0,   255},     // Rouge vif
            {255, 165, 0,   255},   // Orange
            {255, 255, 0,   255},   // Jaune
            {0,   255, 0,   255},     // Vert vif
            {0,   255, 255, 255},   // Cyan
            {0,   120, 255, 255},   // Bleu électrique
            {255, 0,   255, 255},   // Magenta
            {255, 105, 180, 255}, // Rose néon
    };

    // Structure principale - boîtier externe de la console
    SDL_SetRenderDrawColor(renderer, consoleDarkGray.r, consoleDarkGray.g, consoleDarkGray.b, consoleDarkGray.a);
    SDL_FRect outerBody = {consoleX - 5, consoleY - 5, consoleWidth + 10, consoleHeight + 10};
    SDL_RenderFillRect(renderer, &outerBody);

    // Dessiner le corps principal de la console (intérieur)
    SDL_SetRenderDrawColor(renderer, consoleGray.r, consoleGray.g, consoleGray.b, consoleGray.a);
    SDL_FRect consoleBody = {consoleX, consoleY, consoleWidth, consoleHeight};
    SDL_RenderFillRect(renderer, &consoleBody);

    // Bordure de la console (plus épaisse pour un look rétro)
    SDL_SetRenderDrawColor(renderer, screenBorder.r, screenBorder.g, screenBorder.b, screenBorder.a);
    SDL_RenderRect(renderer, &consoleBody);

    // Ajouter des détails de design à la console
    float cornerRadius = consoleHeight * 0.03f;
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    // Coins arrondis (simulés avec des petits rectangles aux coins)
    SDL_FRect topLeftCorner = {consoleX, consoleY, cornerRadius, cornerRadius};
    SDL_FRect topRightCorner = {consoleX + consoleWidth - cornerRadius, consoleY, cornerRadius, cornerRadius};
    SDL_FRect bottomLeftCorner = {consoleX, consoleY + consoleHeight - cornerRadius, cornerRadius, cornerRadius};
    SDL_FRect bottomRightCorner = {consoleX + consoleWidth - cornerRadius, consoleY + consoleHeight - cornerRadius,
                                   cornerRadius, cornerRadius};

    SDL_RenderFillRect(renderer, &topLeftCorner);
    SDL_RenderFillRect(renderer, &topRightCorner);
    SDL_RenderFillRect(renderer, &bottomLeftCorner);
    SDL_RenderFillRect(renderer, &bottomRightCorner);

    // Diviser la console en trois zones : écran, clavier, contrôles
    float screenMargin = consoleHeight * 0.025f;
    float screenWidth = consoleWidth - screenMargin * 2;
    float screenHeight = consoleHeight * 0.35f; // Réduire la taille de l'écran
    float screenX = consoleX + screenMargin;
    float screenY = consoleY + screenMargin;

    // Zone clavier - sous l'écran
    float keyboardMargin = screenMargin;
    float keyboardHeight = consoleHeight * 0.45f;
    float keyboardWidth = screenWidth;
    float keyboardX = screenX;
    float keyboardY = screenY + screenHeight + screenMargin;

    // Zone contrôles - tout en bas
    float controlsHeight = consoleHeight * 0.12f;
    float controlsWidth = screenWidth;
    float controlsX = screenX;
    float controlsY = keyboardY + keyboardHeight + screenMargin * 0.5f;

    // Dessiner les trois zones principales avec des bordures distinctes
    // 1. Zone écran
    SDL_SetRenderDrawColor(renderer, screenBorder.r, screenBorder.g, screenBorder.b, screenBorder.a);
    SDL_FRect screenBorderRect = {screenX - 4, screenY - 4, screenWidth + 8, screenHeight + 8};
    SDL_RenderFillRect(renderer, &screenBorderRect);

    SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
    SDL_FRect screenRect = {screenX, screenY, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &screenRect);

    // Ajout d'une texture de scan-lines pour un effet CRT rétro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30);
    for (int y = 0; y < screenHeight; y += 3) {
        SDL_FRect scanLine = {screenX, screenY + y, screenWidth, 1};
        SDL_RenderFillRect(renderer, &scanLine);
    }

    // 2. Zone clavier (avec bordure légèrement différente)
    SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
    SDL_FRect keyboardBorderRect = {keyboardX - 4, keyboardY - 4, keyboardWidth + 8, keyboardHeight + 8};
    SDL_RenderFillRect(renderer, &keyboardBorderRect);

    // Fond du clavier plus foncé que la console
    SDL_SetRenderDrawColor(renderer, 30, 50, 60, 255);
    SDL_FRect keyboardRect = {keyboardX, keyboardY, keyboardWidth, keyboardHeight};
    SDL_RenderFillRect(renderer, &keyboardRect);

    // 3. Zone contrôles (avec bordure différente)
    SDL_SetRenderDrawColor(renderer, 40, 40, 45, 255);
    SDL_FRect controlsBorderRect = {controlsX - 4, controlsY - 4, controlsWidth + 8, controlsHeight + 8};
    SDL_RenderFillRect(renderer, &controlsBorderRect);

    // Fond de la zone contrôles
    SDL_SetRenderDrawColor(renderer, 50, 55, 60, 255);
    SDL_FRect controlsRect = {controlsX, controlsY, controlsWidth, controlsHeight};
    SDL_RenderFillRect(renderer, &controlsRect);

    // Dessiner des détails "électroniques" sur l'écran
    if (!videoGame->getButtons().empty()) {
        // Afficher un titre "8-BIT KEYBOARD" sur l'écran
        SDL_SetRenderDrawColor(renderer, 90, 140, 170, 255);
        SDL_FRect titleBar = {screenX + 10, screenY + 10, screenWidth - 20, screenHeight * 0.15f};
        SDL_RenderFillRect(renderer, &titleBar);

        // Ajouter quelques "indicateurs LED" à droite de l'écran
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // LED verte
        SDL_FRect greenLED = {screenX + screenWidth - 25, screenY + 15, 10, 10};
        SDL_RenderFillRect(renderer, &greenLED);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // LED rouge
        SDL_FRect redLED = {screenX + screenWidth - 25, screenY + 30, 10, 10};
        SDL_RenderFillRect(renderer, &redLED);

        // Ajouter une petite grille numérique sur l'écran
        SDL_SetRenderDrawColor(renderer, 30, 80, 100, 255);
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 8; col++) {
                if ((row + col) % 2 == 0) {
                    SDL_FRect gridCell = {
                            screenX + screenWidth * 0.1f + col * 15,
                            screenY + screenHeight * 0.25f + row * 15,
                            12, 12
                    };
                    SDL_RenderFillRect(renderer, &gridCell);
                }
            }
        }

        // Afficher une forme d'onde sur l'écran
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 180);
        for (int i = 0; i < screenWidth * 0.6; i++) {
            float x = screenX + screenWidth * 0.2f + i;
            float y = screenY + screenHeight * 0.75f +
                      sin(i * 0.05) * 15;

            SDL_FRect wavePoint = {x, y, 2, 2};
            SDL_RenderFillRect(renderer, &wavePoint);
        }
    }

    const std::vector<GameButton> &buttons = videoGame->getButtons();

    if (buttons.empty()) {
        // Afficher des pixels décoratifs quand il n'y a pas de boutons
        float pixelSize = screenWidth / 32; // 32 pixels de largeur
        int pixelRows = static_cast<int>(screenHeight / pixelSize);

        for (int row = 0; row < pixelRows; row++) {
            for (int col = 0; col < 32; col++) {
                if ((row + col) % 4 == 0 || (row * col) % 7 == 0) {
                    SDL_Color pixelColor = pixelColors[(row + col) % 8];
                    SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);

                    SDL_FRect pixel = {
                            screenX + col * pixelSize,
                            screenY + row * pixelSize,
                            pixelSize - 1,
                            pixelSize - 1
                    };
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
    } else {
        // Division de la zone clavier en deux octaves distinctes
        float octaveHeight = keyboardHeight * 0.48f; // Chaque octave prend un peu moins de la moitié
        float octaveSpacing = keyboardHeight * 0.04f; // Espacement entre les octaves
        float octave1Y = keyboardY + octaveHeight * 0.05f;
        float octave2Y = octave1Y + octaveHeight + octaveSpacing;

        // Diviser chaque octave avec une ligne
        SDL_SetRenderDrawColor(renderer, 60, 85, 100, 255);
        SDL_FRect octave1Bg = {keyboardX + 5, octave1Y, keyboardWidth - 10, octaveHeight};
        SDL_FRect octave2Bg = {keyboardX + 5, octave2Y, keyboardWidth - 10, octaveHeight};
        SDL_RenderFillRect(renderer, &octave1Bg);
        SDL_RenderFillRect(renderer, &octave2Bg);

        // Ajouter des lignes de séparation entre les octaves et des étiquettes
        SDL_SetRenderDrawColor(renderer, 80, 110, 130, 255);
        SDL_FRect octave1Label = {keyboardX + 10, octave1Y + 5, 20, 10};
        SDL_FRect octave2Label = {keyboardX + 10, octave2Y + 5, 20, 10};
        SDL_RenderFillRect(renderer, &octave1Label);
        SDL_RenderFillRect(renderer, &octave2Label);

        const int NATURAL_NOTES = 7; // C, D, E, F, G, A, B
        float keySpacingX = keyboardWidth * 0.01f; // Réduire l'espacement pour éviter la superposition
        float naturalKeyWidth = (keyboardWidth - keySpacingX * (NATURAL_NOTES + 1)) / NATURAL_NOTES;

        // Pour améliorer l'apparence des touches naturelles
        float naturalKeyHeight = octaveHeight * 0.85f;
        // Pour les touches accidentées (noires)
        float accidentalKeyWidth = naturalKeyWidth * 0.6f;
        float accidentalKeyHeight = naturalKeyHeight * 0.6f;

        // Dessiner de subtiles lignes de séparation entre les touches
        SDL_SetRenderDrawColor(renderer, 40, 60, 75, 100);
        for (int i = 0; i <= NATURAL_NOTES; i++) {
            float lineX = keyboardX + keySpacingX + i * (naturalKeyWidth + keySpacingX);
            SDL_FRect line1 = {lineX, octave1Y, 1, octaveHeight};
            SDL_FRect line2 = {lineX, octave2Y, 1, octaveHeight};
            SDL_RenderFillRect(renderer, &line1);
            SDL_RenderFillRect(renderer, &line2);
        }

        // Dessiner les labels des notes
        const char *noteLabels[7] = {"C", "D", "E", "F", "G", "A", "B"};
        for (int i = 0; i < NATURAL_NOTES; i++) {
            // Espace pour les étiquettes
            float x = keyboardX + keySpacingX + i * (naturalKeyWidth + keySpacingX) + naturalKeyWidth / 2 - 5;
            SDL_SetRenderDrawColor(renderer, 150, 200, 230, 100);

            SDL_FRect labelBg1 = {x - 5, octave1Y + naturalKeyHeight - 15, 15, 10};
            SDL_FRect labelBg2 = {x - 5, octave2Y + naturalKeyHeight - 15, 15, 10};
            SDL_RenderFillRect(renderer, &labelBg1);
            SDL_RenderFillRect(renderer, &labelBg2);
        }
    }

    // Couleurs pour les notes naturelles et accidentées
    SDL_Color naturalColors[] = {
            {0,   180, 255, 255},    // Bleu clair (Do)
            {255, 100, 0,   255},    // Orange (Ré)
            {180, 255, 0,   255},    // Vert clair (Mi)
            {255, 0,   100, 255},    // Rose (Fa)
            {100, 0,   255, 255},    // Violet (Sol)
            {255, 255, 0,   255},    // Jaune (La)
            {0,   255, 150, 255},    // Turquoise (Si)
    };

    SDL_Color accidentalColors[] = {
            {0,   120, 200, 255},    // Bleu foncé (Do#)
            {200, 80,  0,   255},     // Orange foncé (Ré#)
            {0,   0,   0,   0},          // (pas de Mi#)
            {200, 0,   80,  255},     // Rose foncé (Fa#)
            {80,  0,   200, 255},     // Violet foncé (Sol#)
            {200, 200, 0,   255},    // Jaune foncé (La#)
            {0,   0,   0,   0},          // (pas de Si#)
    };

    // Dessiner chaque bouton avec sa couleur respective
    for (size_t i = 0; i < buttons.size(); i++) {
        const GameButton &button = buttons[i];

        // Déterminer si c'est une note naturelle ou une note altérée
        bool isAccidental = i >= buttons.size() / 2;
        int noteIndex = button.note;

        // Déterminer l'octave (0 ou 1)
        int octave = noteIndex / 12;

        // Déterminer le type de note et sa couleur
        SDL_Color color;

        if (!isAccidental) {
            // Notes naturelles 
            int naturalIndex = noteIndex % 12;
            if (naturalIndex > 6) {
                naturalIndex -= 12; // Correction pour la deuxième octave
            }
            color = naturalColors[naturalIndex];

            // Rendre les touches naturelles plus vives
            color.r = SDL_min(255, color.r + 30);
            color.g = SDL_min(255, color.g + 30);
            color.b = SDL_min(255, color.b + 30);
        } else {
            // Notes accidentées
            int naturalCount = buttons.size() / 2;
            int accidentalIndex = (i - naturalCount) % 5;  // 0-4 pour les touches noires
            color = accidentalColors[accidentalIndex];

            // Les touches accidentées sont légèrement plus sombres
            color.r = SDL_max(0, color.r - 20);
            color.g = SDL_max(0, color.g - 20);
            color.b = SDL_max(0, color.b - 20);
        }

        // Si le bouton est survolé, le rendre plus lumineux et pulsatif
        if (button.isHovered) {
            // Effet de pulsation quand le bouton est survolé
            Uint32 ticks = SDL_GetTicks();
            float pulse = (sin(ticks * 0.01f) + 1.0f) / 2.0f; // Valeur entre 0 et 1

            color.r = SDL_min(255, static_cast<int>(color.r * (0.7f + pulse * 0.6f)));
            color.g = SDL_min(255, static_cast<int>(color.g * (0.7f + pulse * 0.6f)));
            color.b = SDL_min(255, static_cast<int>(color.b * (0.7f + pulse * 0.6f)));
        }

        // Dessiner le bouton avec un effet de pixel 8-bit et effet 3D
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &button.rect);

        // Ajouter un effet 3D sur les touches
        SDL_Color highlightColor = {
                static_cast<Uint8>(SDL_min(255, static_cast<int>(color.r) + 70)),
                static_cast<Uint8>(SDL_min(255, static_cast<int>(color.g) + 70)),
                static_cast<Uint8>(SDL_min(255, static_cast<int>(color.b) + 70)),
                255
        };

        SDL_Color shadowColor = {
                static_cast<Uint8>(SDL_max(0, static_cast<int>(color.r) - 50)),
                static_cast<Uint8>(SDL_max(0, static_cast<int>(color.g) - 50)),
                static_cast<Uint8>(SDL_max(0, static_cast<int>(color.b) - 50)),
                255
        };

        // Effet de lumière (haut et gauche)
        SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
        SDL_FRect topHighlight = {button.rect.x, button.rect.y, button.rect.w, 2};
        SDL_FRect leftHighlight = {button.rect.x, button.rect.y, 2, button.rect.h};
        SDL_RenderFillRect(renderer, &topHighlight);
        SDL_RenderFillRect(renderer, &leftHighlight);

        // Effet d'ombre (bas et droite)
        SDL_SetRenderDrawColor(renderer, shadowColor.r, shadowColor.g, shadowColor.b, shadowColor.a);
        SDL_FRect bottomShadow = {button.rect.x, button.rect.y + button.rect.h - 2, button.rect.w, 2};
        SDL_FRect rightShadow = {button.rect.x + button.rect.w - 2, button.rect.y, 2, button.rect.h};
        SDL_RenderFillRect(renderer, &bottomShadow);
        SDL_RenderFillRect(renderer, &rightShadow);

        // Ajouter un effet de brillance pour le style "pixel LED"
        if (button.isHovered) {
            // Contour blanc lumineux et plus épais avec glow
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_RenderRect(renderer, &button.rect);

            // Effet de halo lumineux autour du bouton survolé
            SDL_FRect halo = {
                    button.rect.x - 3,
                    button.rect.y - 3,
                    button.rect.w + 6,
                    button.rect.h + 6
            };
            SDL_RenderRect(renderer, &halo);

            // Deuxième halo plus large et plus léger 
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_FRect outerHalo = {
                    button.rect.x - 5,
                    button.rect.y - 5,
                    button.rect.w + 10,
                    button.rect.h + 10
            };
            SDL_RenderRect(renderer, &outerHalo);

            // Ajouter un effet de surbrillance au centre de la touche
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
            SDL_FRect innerGlow = {
                    button.rect.x + button.rect.w * 0.25f,
                    button.rect.y + button.rect.h * 0.25f,
                    button.rect.w * 0.5f,
                    button.rect.h * 0.5f
            };
            SDL_RenderFillRect(renderer, &innerGlow);
        } else {
            // Contour standard avec effet 3D subtil
            if (isAccidental) {
                // Contour plus sombre pour les touches noires
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                // Ajouter un effet 3D pour les touches noires pour renforcer l'apparence de piano
                // Effet d'ombrage pour montrer qu'elles sont surélevées
                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                SDL_FRect sideEffect = {
                        button.rect.x + button.rect.w,
                        button.rect.y + 2,
                        3,
                        button.rect.h - 4
                };
                SDL_RenderFillRect(renderer, &sideEffect);

                SDL_FRect bottomEffect = {
                        button.rect.x + 2,
                        button.rect.y + button.rect.h,
                        button.rect.w - 4,
                        3
                };
                SDL_RenderFillRect(renderer, &bottomEffect);
            } else {
                // Contour gris foncé pour les touches blanches
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            }
            SDL_RenderRect(renderer, &button.rect);
        }

        // Petit rectangle lumineux pour simuler le reflet (style "pixel brillant")
        if (!button.isHovered) {  // N'ajouter l'effet que sur les boutons non survolés
            SDL_FRect highlight = {
                    button.rect.x + button.rect.w * 0.2f,
                    button.rect.y + button.rect.h * 0.2f,
                    button.rect.w * 0.2f,
                    button.rect.h * 0.2f
            };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_RenderFillRect(renderer, &highlight);
        }
    }

    // Dessiner les contrôles (boutons colorés et D-pad) dans la zone dédiée
    const std::vector<ConsoleControl> &controls = videoGame->getControls();

    // Dessiner la base du D-pad
    float dpadSize = controlsHeight * 0.8f;
    float dpadX = controlsX + controlsWidth * 0.15f - dpadSize / 2;
    float dpadY = controlsY + (controlsHeight - dpadSize) / 2;
    float dpadThickness = dpadSize * 0.3f;

    // D-pad horizontal (base)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_FRect dpadHorizontal = {
            dpadX,
            dpadY + (dpadSize - dpadThickness) / 2,
            dpadSize,
            dpadThickness
    };
    SDL_RenderFillRect(renderer, &dpadHorizontal);

    // D-pad vertical (base)
    SDL_FRect dpadVertical = {
            dpadX + (dpadSize - dpadThickness) / 2,
            dpadY,
            dpadThickness,
            dpadSize
    };
    SDL_RenderFillRect(renderer, &dpadVertical);

    // Base des boutons A, B
    float buttonDiameter = controlsHeight * 0.5f;
    float buttonSpacing = buttonDiameter * 0.3f;
    float buttonsX = controlsX + controlsWidth * 0.85f - buttonDiameter;
    float buttonsY = controlsY + (controlsHeight - buttonDiameter) / 2;

    // Petits boutons Start et Select au centre
    float smallButtonWidth = buttonDiameter;
    float smallButtonHeight = buttonDiameter * 0.4f;
    float smallButtonSpacing = smallButtonWidth * 0.3f;
    float smallButtonsX = controlsX + controlsWidth * 0.5f - smallButtonWidth / 2 - smallButtonSpacing / 2;

    // Dessiner chaque contrôle avec gestion du survol
    for (const ConsoleControl &control: controls) {
        SDL_Color baseColor;

        // Différentes couleurs en fonction du type de contrôle
        switch (control.type) {
            case BUTTON_A:
                baseColor = {255, 50, 50, 255}; // Rouge
                break;
            case BUTTON_B:
                baseColor = {50, 50, 255, 255}; // Bleu
                break;
            case BUTTON_START:
            case BUTTON_SELECT:
                baseColor = {160, 160, 160, 255}; // Gris
                break;
            case DPAD_UP:
            case DPAD_DOWN:
            case DPAD_LEFT:
            case DPAD_RIGHT:
                baseColor = {60, 60, 60, 255}; // Gris foncé
                break;
        }

        // Si le contrôle est survolé, le rendre plus lumineux
        if (control.isHovered) {
            baseColor.r = SDL_min(255, baseColor.r + 70);
            baseColor.g = SDL_min(255, baseColor.g + 70);
            baseColor.b = SDL_min(255, baseColor.b + 70);

            // Ajouter un effet lumineux
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_FRect glowRect = {
                    control.rect.x - 3,
                    control.rect.y - 3,
                    control.rect.w + 6,
                    control.rect.h + 6
            };
            SDL_RenderRect(renderer, &glowRect);
        }

        // Dessiner le contrôle avec sa couleur
        SDL_SetRenderDrawColor(renderer, baseColor.r, baseColor.g, baseColor.b, baseColor.a);
        SDL_RenderFillRect(renderer, &control.rect);

        // Dessiner une bordure
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &control.rect);

        // Ajouter des effets 3D pour les contrôles
        if (control.type == BUTTON_A || control.type == BUTTON_B) {
            // Effet de lumière pour les boutons ronds
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            float highlightSize = control.rect.w * 0.3f;
            SDL_FRect highlight = {
                    control.rect.x + control.rect.w * 0.2f,
                    control.rect.y + control.rect.h * 0.2f,
                    highlightSize,
                    highlightSize
            };
            SDL_RenderFillRect(renderer, &highlight);
        } else if (control.type == BUTTON_START || control.type == BUTTON_SELECT) {
            // Effet de dégradé pour les boutons rectangulaires
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 100);
            SDL_FRect shadow = {
                    control.rect.x,
                    control.rect.y + control.rect.h - 2,
                    control.rect.w,
                    2
            };
            SDL_RenderFillRect(renderer, &shadow);

            // Ajouter un texte "START" ou "SELECT"
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            if (control.type == BUTTON_START) {
                // Simuler un texte "START" avec des rectangles
                float textX = control.rect.x + control.rect.w * 0.2f;
                float textY = control.rect.y + control.rect.h * 0.3f;
                float textW = control.rect.w * 0.6f;
                float textH = control.rect.h * 0.4f;
                SDL_FRect textRect = {textX, textY, textW, textH};
                SDL_RenderRect(renderer, &textRect);
            } else {
                // Simuler un texte "SELECT" avec des rectangles
                float textX = control.rect.x + control.rect.w * 0.1f;
                float textY = control.rect.y + control.rect.h * 0.3f;
                float textW = control.rect.w * 0.8f;
                float textH = control.rect.h * 0.4f;
                SDL_FRect textRect = {textX, textY, textW, textH};
                SDL_RenderRect(renderer, &textRect);
            }
        }
    }

    // Ajouter des petits "trous de vis" décoratifs aux coins de la console
    float screwSize = 5.0f;
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

    SDL_FRect screw1 = {consoleX + 10, consoleY + 10, screwSize, screwSize};
    SDL_FRect screw2 = {consoleX + consoleWidth - 10 - screwSize, consoleY + 10, screwSize, screwSize};
    SDL_FRect screw3 = {consoleX + 10, consoleY + consoleHeight - 10 - screwSize, screwSize, screwSize};
    SDL_FRect screw4 = {consoleX + consoleWidth - 10 - screwSize, consoleY + consoleHeight - 10 - screwSize, screwSize,
                        screwSize};

    SDL_RenderFillRect(renderer, &screw1);
    SDL_RenderFillRect(renderer, &screw2);
    SDL_RenderFillRect(renderer, &screw3);
    SDL_RenderFillRect(renderer, &screw4);

    // Ajouter une petite LED d'alimentation
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect powerLED = {consoleX + consoleWidth - 20, consoleY + 10, 5, 5};
    SDL_RenderFillRect(renderer, &powerLED);
}