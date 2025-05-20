#pragma once

#include <SDL3/SDL.h>
#include <string>

struct Button {
    SDL_FRect rect;        // Position and dimensions
    SDL_Color color;       // Background color
    std::string name;      // Name/label of the button (can be used for text or ID)
    SDL_Texture *icon;     // Optional icon texture
    // Add any other properties a button might need, e.g., state (hovered, pressed)
};
