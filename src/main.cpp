#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "../include/menu.hpp"
#include "SDL_mixer.h"

int main(int argc, char *argv[])
{
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Erreur d'initialisation de SDL : " << SDL_GetError() << std::endl;
        return -1;
    }
    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Erreur initialisation SDL_mixer: " << Mix_GetError() << std::endl;
    }
    

    // Initialisation de SDL_image
    if (IMG_Init(IMG_INIT_PNG) == 0) // Assurez-vous de charger des images PNG ou autres formats avec transparence
    {
        std::cout << "Erreur d'initialisation de SDL_image : " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initialisation de SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cout << "Erreur d'initialisation de SDL_ttf : " << TTF_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Création de la fenêtre et du renderer
    SDL_Window *window = SDL_CreateWindow("Menu SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Erreur de création de la fenêtre : " << SDL_GetError() << std::endl;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Erreur de création du renderer : " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Créer une instance du menu
    Menu menu(window, renderer);

    // Afficher le menu
    menu.afficher();

    // Nettoyer et fermer SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();

    return 0;
}
