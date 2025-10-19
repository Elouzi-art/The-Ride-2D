#include "menu_options.hpp"
#include <SDL_image.h>
#include <iostream>
#include "jeu1.hpp"
#include "jeu2.hpp"
#include "Unlimite1.hpp"
#include "Unlimite2.hpp"

MenuOption::MenuOption(SDL_Window *win, SDL_Renderer *ren)
    : Menu(win, ren), choix_ligne(0), index_map(0) {}

void MenuOption::display(std::string &selectedMap,int choix_actuel)
{
    bool running = true; 
    SDL_Event event;

    // Boucle principale de l'écran des options
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                        index_map = (index_map - 1 + maps.size()) % maps.size();
                    break;
                case SDLK_DOWN:
                        index_map = (index_map - 1 + maps.size()) % maps.size();
                    break;
                case SDLK_LEFT:
                        index_map = (index_map - 1 + maps.size()) % maps.size();
                    break;
                case SDLK_RIGHT:
                        index_map = (index_map - 1 + maps.size()) % maps.size();
                    break;
                case SDLK_RETURN:
                    // Valider la sélection
                    selectedMap = maps[index_map];
                    std::cout << "Map selectionnee: " << selectedMap << std::endl;

                    running = false; // Quitte la boucle du menu

                    // Lancer le niveau avec les choix :
                    {   
                        if (index_map == 0) {
                            if (choix_actuel == 0) {
                            std::cout << "GameMap1  = Choix : " << choix_actuel << "\t Index :" << index_map;
                            GameMap1 jeu(window,renderer);
                            jeu.run();
                            }
                            else if (choix_actuel == 1){
                             std::cout << "GameMode11 = Choix : " << choix_actuel << "\t Index :" << index_map;
                            GameMode11 jeu(window,renderer);
                            jeu.run();
                            }
                        }
                        else 
                            {
                            // Lancer le jeu
                            if (choix_actuel == 0) {
                                std::cout << "GameMap2 = Choix : " << choix_actuel << "\t Index :" << index_map;
                                GameMap2 jeu(window,renderer);
                                jeu.run();
                                }
                                else if (choix_actuel == 1){
                                std::cout << "GameMode12 = Choix : " << choix_actuel << "\t Index :" << index_map;
                                GameMode12 jeu(window,renderer);
                                jeu.run();
                                }
                        }
                    }

                    break;

                case SDLK_ESCAPE:
                    running = false; // Retour ou annulation
                    break;
                }
            }
        }

        // Effacer l'écran
        SDL_RenderClear(renderer);
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);

        // Charger et afficher le fond d'écran
        SDL_Surface *backgroundSurface = IMG_Load("res/menu_background.jpg");
        if (!backgroundSurface)
        {
            std::cerr << "Erreur chargement du fond: " << IMG_GetError() << std::endl;
            continue;
        }
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_DestroyTexture(backgroundTexture);

        // Afficher les options (Map et Player)
        renderOption("Map : " + maps[index_map], 100, 200, choix_ligne == 0, 300);
        // map preview
        SDL_Surface* previewSurface = IMG_Load(mapPreviews[index_map].c_str());
        if (!previewSurface)
            {
                std::cerr << "Erreur chargement preview: " << IMG_GetError() << std::endl;
            }
        else
            {
                SDL_Texture* previewTexture = SDL_CreateTextureFromSurface(renderer, previewSurface);
                SDL_FreeSurface(previewSurface);

                SDL_Rect destRect;
                destRect.x = 100; // Position X
                destRect.y = 250; // Position Y (under the "Map : ...")
                destRect.w = 300; // Width of the preview
                destRect.h = 200; // Height of the preview

                SDL_RenderCopy(renderer, previewTexture, nullptr, &destRect);
                SDL_DestroyTexture(previewTexture);
             }
         // Afficher le rendu
        SDL_RenderPresent(renderer);
    }
}