#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <string>
#include <functional>

class DropdownMenu {
private:
    bool isOpen;
    SDL_FRect headerRect;
    std::vector<SDL_FRect> itemRects;
    std::vector<std::string> itemLabels;
    std::vector<std::function<void()>> itemActions;
    int selectedIndex;
    SDL_Color headerColor;
    SDL_Color textColor;
    SDL_Color itemColor;
    SDL_Color hoverColor;
    SDL_Color darkBlue;
    TTF_Font *font;

public:
    DropdownMenu(float x, float y, float width, float height, const std::string &headerLabel);

    ~DropdownMenu();

    void addItem(const std::string &label, std::function<void()> action);

    void render(SDL_Renderer *renderer);

    void renderText(SDL_Renderer *renderer, const std::string &text, SDL_FRect &targetRect, SDL_Color color);

    bool handleClick(float x, float y);

    void toggleOpen();

    bool isMenuOpen() const { return isOpen; }

    void closeMenu() { isOpen = false; }

    void updateHoverState(float x, float y);
};