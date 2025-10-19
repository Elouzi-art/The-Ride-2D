#ifndef MENU_OPTION_HPP
#define MENU_OPTION_HPP

#include "menu.hpp"
#include <vector>
#include <string>

class MenuOption : public Menu
{
public:
    MenuOption(SDL_Window *window, SDL_Renderer *renderer);

    void display(std::string &selectedMap,int choix);
    std::vector<std::string> maps = {"FIRE", "WINTER"};
    std::vector<std::string> mapPreviews = {"res/map1_preview.png", "res/map2_preview.png"};

private:
    int choix_ligne = 0; // 0 = map, 1 = joueur
    int index_map = 0;
};

#endif // MENU_OPTION_HPP
