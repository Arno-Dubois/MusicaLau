//
// Created by coren on 24/03/2025.
//

#include "../../include/Model/Xylophone.h"

Xylophone::Xylophone(float x, float y, float width, float height, int bars)
        : x(x), y(y), width(width), height(height), bars(bars), hoveredBarIndex(-1) {
    calculateBarsLayout();
}

void Xylophone::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
    calculateBarsLayout();
}

void Xylophone::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
    calculateBarsLayout();
}

void Xylophone::addBar() {
    if (bars < 12) {
        bars++;
        calculateBarsLayout();
    }
}

void Xylophone::removeBar() {
    if (bars > 4) {
        bars--;
        calculateBarsLayout();
    }
}

void Xylophone::calculateBarsLayout() {
    // Sauvegarder l'index de la lame survolée
    int previousHoveredIndex = hoveredBarIndex;

    xylophones.clear();
    hoveredBarIndex = -1;

    if (width <= 0 || height <= 0 || bars <= 0) {
        return;
    }

    // Dimensions pour le xylophone
    float xylophoneWidth = width * 0.9f; // Largeur totale du xylophone
    float xylophoneHeight = height * 0.8f; // Hauteur totale du xylophone

    // Position du xylophone
    float xylophoneX = x + (width - xylophoneWidth) / 2;
    float xylophoneY = y + height * 0.2f;

    // Dimensions des lames
    float barAreaWidth = xylophoneWidth * 0.85f;
    float barAreaX = xylophoneX + (xylophoneWidth - barAreaWidth) / 2;
    float barHeight = xylophoneHeight * 0.1f;
    float totalBarsSpace = xylophoneHeight * 0.75f;
    float barSpacing = totalBarsSpace / bars;

    // Créer les lames horizontales
    for (int i = 0; i < bars; i++) {
        // La largeur diminue progressivement pour les notes plus hautes
        float barWidth = barAreaWidth * (1.0f - (i * 0.03f));
        float barX = barAreaX + (barAreaWidth - barWidth) / 2;
        float barY = xylophoneY + xylophoneHeight * 0.15f + i * barSpacing;

        XylophoneBar bar;
        bar.rect = {barX, barY, barWidth, barHeight};
        bar.isHovered = (i == previousHoveredIndex);

        xylophones.push_back(bar);

        // Restaurer l'état de survol si c'était cette lame
        if (i == previousHoveredIndex) {
            hoveredBarIndex = i;
        }
    }
}

bool Xylophone::updateHoveredBar(float mouseX, float mouseY) {
    // Effacer l'état de survol précédent
    if (hoveredBarIndex >= 0 && hoveredBarIndex < xylophones.size()) {
        xylophones[hoveredBarIndex].isHovered = false;
    }
    hoveredBarIndex = -1;

    for (int i = 0; i < xylophones.size(); i++) {
        const auto &bar = xylophones[i];
        if (mouseX >= bar.rect.x && mouseX <= (bar.rect.x + bar.rect.w) &&
            mouseY >= bar.rect.y && mouseY <= (bar.rect.y + bar.rect.h)) {
            xylophones[i].isHovered = true;
            hoveredBarIndex = i;
            SDL_Log("Xylophone bar hovered: %d", i);
            return true;
        }
    }
    return false;
}

void Xylophone::clearHoveredBar() {
    if (hoveredBarIndex >= 0 && hoveredBarIndex < xylophones.size()) {
        xylophones[hoveredBarIndex].isHovered = false;
    }
    hoveredBarIndex = -1;
}

int Xylophone::getBarAt(float mouseX, float mouseY) const {
    for (int i = 0; i < xylophones.size(); i++) {
        const auto &bar = xylophones[i];
        if (mouseX >= bar.rect.x && mouseX <= (bar.rect.x + bar.rect.w) &&
            mouseY >= bar.rect.y && mouseY <= (bar.rect.y + bar.rect.h)) {
            return i;
        }
    }
    return -1; // Aucune lame trouvée
}