#include "../../include/View/ButtonView.h"
#include "../../include/utils/TextHelper.h" // For TextHelper functions
#include <iostream> // For std::cer
#include <cmath>

ButtonView::ButtonView() : font_(nullptr) {
}

ButtonView::~ButtonView() {
    // Font is managed externally (e.g., by Controller or a FontManager)
    // So, ButtonView should not delete it.
}

bool ButtonView::initialize(TTF_Font* font) {
    if (!font) {
        std::cerr << "ButtonView Error: Provided font is null." << std::endl;
        return false;
    }
    font_ = font;
    return true;
}

// Implementation of renderButtons and drawing utility functions
// (These will be moved from Controller.cpp)

void ButtonView::renderButtons(SDL_Renderer *renderer, const std::vector<Button> &buttons,
                               bool isSongPlayingActive, bool isSongPaused) {
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

        // Calculer la position du texte (plus proche du bas)
        float textY = button.rect.y + button.rect.h - 15;
        // Calculer la position des icônes (plus haute pour laisser de la place au texte)
        float iconY = centerY - 10;

        // Dessiner l'icône ou le texte en fonction de l'index du bouton
        // Note: This switch statement relies on the order and number of buttons
        // as defined in the Controller. This coupling might need to be addressed
        // if the button definitions change frequently or become more dynamic.
        switch (i) {
            case 0: // Select
            {
                renderTextCentered(renderer, centerX, centerY, button.name, {255, 255, 255, 255});
                break;
            }
            case 1: // Remove Octave (-)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_FRect minus = {centerX - 15, iconY - 2, 30, 4};
                SDL_RenderFillRect(renderer, &minus);
                renderSmallText(renderer, centerX, textY, "Remove", {255, 255, 255, 255});
                break;
            }
            case 2: // Add Octave (+)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_FRect plusH = {centerX - 15, iconY - 2, 30, 4};
                SDL_RenderFillRect(renderer, &plusH);
                SDL_FRect plusV = {centerX - 2, iconY - 15, 4, 30};
                SDL_RenderFillRect(renderer, &plusV);
                renderSmallText(renderer, centerX, textY, "Add", {255, 255, 255, 255});
                break;
            }
            case 3: {
                drawFileIcon(renderer, centerX, iconY, 30, {255, 255, 255, 255});
                renderSmallText(renderer, centerX, textY, "Import", {255, 255, 255, 255});
                break;
            }
            case 4: {
                if (isSongPlayingActive) {
                    if (isSongPaused) {
                        drawPlayIcon(renderer, centerX, iconY, 20, {200, 200, 200, 255});
                        renderSmallText(renderer, centerX, textY, "Resume", {255, 255, 255, 255});
                    } else {
                        drawPauseIcon(renderer, centerX, iconY, 20, {200, 200, 200, 255});
                        renderSmallText(renderer, centerX, textY, "Pause", {255, 255, 255, 255});
                    }
                } else {
                    drawPlayIcon(renderer, centerX, iconY, 20, {200, 200, 200, 255});
                    renderSmallText(renderer, centerX, textY, "Play", {255, 255, 255, 255});
                }
                break;
            }
            case 5: // Start Recording
            {
                drawPlayIcon(renderer, centerX, iconY, 20, {255, 50, 50, 255});
                renderSmallText(renderer, centerX, textY, "Record", {255, 255, 255, 255});
                break;
            }
            case 6: // Export
            {
                drawUpArrow(renderer, centerX, iconY, 20, {255, 50, 50, 255});
                renderSmallText(renderer, centerX, textY, "Export", {255, 255, 255, 255});
                break;
            }
            case 7: // Finish Recording
            {
                drawStopIcon(renderer, centerX, iconY, 20, {255, 50, 50, 255});
                renderSmallText(renderer, centerX, textY, "Stop", {255, 255, 255, 255});
                break;
            }
            // Add a default case or handle other buttons if necessary
        }
    }
}

void ButtonView::renderTextCentered(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color) {
    if (!font_ || !renderer || text.empty()) return;

    SDL_Surface *textSurface = TextHelper::RenderTextSolid(font_, text, color);
    if (!textSurface) {
        std::cerr << "ButtonView Error: Failed to render text surface: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    if (!textTexture) {
        std::cerr << "ButtonView Error: Failed to create text texture: " << SDL_GetError() << std::endl;
        return;
    }

    float textWidth, textHeight;
    SDL_GetTextureSize(textTexture, &textWidth, &textHeight);

    SDL_FRect renderQuad = {
            centerX - textWidth / 2.0f,
            centerY - textHeight / 2.0f,
            textWidth,
            textHeight
    };

    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void ButtonView::renderSmallText(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color) {
    if (!font_ || !renderer || text.empty()) return;

    // Assuming TextHelper::LoadFont can be used here, or use a pre-loaded small font if available
    // For simplicity, this example reuses the main font. If a different size is strictly needed,
    // the ButtonView should perhaps manage its own small font or be provided one.
    TTF_Font *smallFont = TextHelper::LoadFont("Roboto-SemiBold.ttf", 14); // Consider managing this font instance

    if (!smallFont) {
        std::cerr << "ButtonView Warning: Could not load small font. Using default font for small text." << std::endl;
        renderTextCentered(renderer, centerX, centerY, text, color); // Fallback to default font and size
        return;
    }

    SDL_Surface *textSurface = TextHelper::RenderTextSolid(smallFont, text, color);
    TTF_CloseFont(smallFont); // Close if loaded ad-hoc

    if (!textSurface) {
        std::cerr << "ButtonView Error: Failed to render small text surface: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);

    if (!textTexture) {
        std::cerr << "ButtonView Error: Failed to create small text texture: " << SDL_GetError() << std::endl;
        return;
    }

    float textWidth, textHeight;
    SDL_GetTextureSize(textTexture, &textWidth, &textHeight);

    SDL_FRect renderQuad = {
            centerX - textWidth / 2.0f,
            centerY - textHeight / 2.0f,
            textWidth,
            textHeight
    };

    SDL_RenderTexture(renderer, textTexture, NULL, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void ButtonView::drawFileIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect docRect = {centerX - size / 2, centerY - size / 2, size, size};
    SDL_RenderRect(renderer, &docRect);
    float foldSize = size / 4;
    SDL_FPoint fold[] = {
            {centerX + size / 2 - foldSize, centerY - size / 2},             
            {centerX + size / 2,            centerY - size / 2 + foldSize},             
            {centerX + size / 2 - foldSize, centerY - size / 2 + foldSize}   
    };
    for (int i = 0; i < 2; i++) {
        SDL_RenderLine(renderer, fold[i].x, fold[i].y, fold[i + 1].x, fold[i + 1].y);
    }
    SDL_RenderLine(renderer, fold[0].x, fold[0].y, fold[2].x, fold[2].y);
    float plusSize = size / 3;
    SDL_FRect plusH = {centerX - plusSize / 2, centerY, plusSize, size / 10};
    SDL_FRect plusV = {centerX - size / 20, centerY - plusSize / 2, size / 10, plusSize};
    SDL_RenderFillRect(renderer, &plusH);
    SDL_RenderFillRect(renderer, &plusV);
}

void ButtonView::drawPlayIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FPoint triangle[3];
    float halfHeight = size / 2;
    float width = size * 0.9f;
    triangle[0] = {centerX - width / 3, centerY - halfHeight};
    triangle[1] = {centerX + width * 2 / 3, centerY};
    triangle[2] = {centerX - width / 3, centerY + halfHeight};
    float minY = triangle[0].y;
    float maxY = triangle[2].y;
    for (float y = minY; y <= maxY; y += 0.5f) {
        float leftX = triangle[0].x;
        float progress = (y - minY) / (maxY - minY);
        float rightX;
        if (progress <= 0.5f) {
            float normalizedProgress = progress * 2;
            rightX = triangle[0].x + normalizedProgress * (triangle[1].x - triangle[0].x);
        } else {
            float normalizedProgress = (progress - 0.5f) * 2;
            rightX = triangle[1].x - normalizedProgress * (triangle[1].x - triangle[2].x);
        }
        SDL_RenderLine(renderer, leftX, y, rightX, y);
    }
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        SDL_RenderLine(renderer, triangle[i].x, triangle[i].y, triangle[next].x, triangle[next].y);
    }
}

void ButtonView::drawUpArrow(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FPoint arrow[] = {
            {centerX,            centerY - size / 2},                  
            {centerX - size / 3, centerY - size / 6},       
            {centerX - size / 6, centerY - size / 6},       
            {centerX - size / 6, centerY + size / 2},       
            {centerX + size / 6, centerY + size / 2},       
            {centerX + size / 6, centerY - size / 6},       
            {centerX + size / 3, centerY - size / 6}        
    };
    for (float y_ = arrow[0].y; y_ <= arrow[3].y; y_ += 1.0f) {
        float left = centerX - size / 6;
        float right = centerX + size / 6;
        if (y_ < arrow[1].y) {
            float t = (y_ - arrow[0].y) / (arrow[1].y - arrow[0].y);
            left = centerX - t * size / 3;
            right = centerX + t * size / 3;
        }
        SDL_RenderLine(renderer, left, y_, right, y_);
    }
    for (int i = 0; i < 7; i++) {
        int next = (i + 1) % 7;
        SDL_RenderLine(renderer, arrow[i].x, arrow[i].y, arrow[next].x, arrow[next].y);
    }
}

void ButtonView::drawStopIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const int numPoints = 32;
    SDL_FPoint circle[numPoints];
    for (int i = 0; i < numPoints; i++) {
        float angle = i * 2 * M_PI / numPoints;
        circle[i].x = centerX + size / 2 * cos(angle);
        circle[i].y = centerY + size / 2 * sin(angle);
    }
    for (float y_ = centerY - size / 2; y_ <= centerY + size / 2; y_ += 1.0f) {
        float dy = y_ - centerY;
        float dx = sqrt((size / 2) * (size / 2) - dy * dy);
        SDL_RenderLine(renderer, centerX - dx, y_, centerX + dx, y_);
    }
    for (int i = 0; i < numPoints; i++) {
        int next = (i + 1) % numPoints;
        SDL_RenderLine(renderer, circle[i].x, circle[i].y, circle[next].x, circle[next].y);
    }
    float squareSize = size / 3;
    SDL_FRect square = {
            centerX - squareSize / 2,
            centerY - squareSize / 2,
            squareSize,
            squareSize
    };
    SDL_RenderFillRect(renderer, &square);
}

void ButtonView::drawPauseIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    float barWidth = size / 4; // Width of each bar of the pause icon
    float barHeight = size; // Height of each bar
    float spacing = size / 5; // Spacing between the two bars

    // Left bar
    SDL_FRect leftBar = {centerX - spacing / 2 - barWidth, centerY - barHeight / 2, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &leftBar);

    // Right bar
    SDL_FRect rightBar = {centerX + spacing / 2, centerY - barHeight / 2, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &rightBar);
}
