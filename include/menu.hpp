#ifndef MENU_HPP
#define MENU_HPP
#include "SDL_mixer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>

class Menu
{
public:
    Menu(SDL_Window *window, SDL_Renderer *renderer);
    ~Menu();

    void afficher();
    int getChoix() const;

public:
    void renderOption(const std::string &texte, int x, int y, bool selected, int maxWidth);
    Mix_Music* music = nullptr;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    std::vector<std::string> options;
    int choix_actuel;
};

#endif // MENU_HPP
