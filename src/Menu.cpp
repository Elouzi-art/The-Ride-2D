#include "menu.hpp"
#include <SDL_image.h>
#include <iostream>
#include "menu_options.hpp"


Menu::Menu(SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer), choix_actuel(0)
{
    font = TTF_OpenFont("res/ARIALBLACKITALIC.TTF", 26);
    if (!font)
    {
        std::cout << "Erreur chargement police : " << TTF_GetError() << std::endl;
    }

    // Initialiser les options du menu
    options.push_back("Jouer");
    options.push_back("Mode 1");
    options.push_back("Quitter");
}


void Menu::afficher()
{   
    // chargement du soundtrack
    
    music = Mix_LoadMUS("audio/menu_background.wav");
    if (!music) {
        std::cout << "Erreur chargement musique : " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(music, -1); // -1 means loop forever
    }   
    
    // Charger le background
    SDL_Surface *backgroundSurface = IMG_Load("res/menu_background.jpg");
    if (!backgroundSurface)
    {
        std::cout << "Erreur chargement image : " << IMG_GetError() << std::endl;
        return;
    }
    
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    
    if (!backgroundTexture)
    {
        std::cout << "Erreur création texture : " << SDL_GetError() << std::endl;
        return;
    }
    // Calculer largeur maximale pour les rectangles des options
    int maxTextWidth = 0;
    for (const auto &opt : options)
    {
        SDL_Surface *tempSurface = TTF_RenderText_Blended(font, opt.c_str(), {255, 0, 0});
        if (tempSurface)
        {
            if (tempSurface->w > maxTextWidth)
            maxTextWidth = tempSurface->w;
            SDL_FreeSurface(tempSurface);
        }
    }
    bool running = true;
    SDL_Event event;
    int alpha = 0;
    const int alphaStep = 2;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            running = false;
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    exit(0);
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    choix_actuel = (choix_actuel - 1 + options.size()) % options.size();
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    choix_actuel = (choix_actuel + 1) % options.size();
                }
                else if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (options[choix_actuel] == "Jouer")
                    {
                        std::string selectedMap = "FIRE.png";
                        MenuOption opt(window, renderer);
                        opt.display(selectedMap,choix_actuel);
                        
                    }
                    else if (options[choix_actuel] == "Mode 1")
                    {
                        std::string selectedMap = "FIRE.png";
                        MenuOption opt(window, renderer);
                        opt.display(selectedMap,choix_actuel);
                    }
                    else if (options[choix_actuel] == "Quitter")
                    {
                        running = false;
                    }
                }
            }
            
        }
                
                SDL_RenderClear(renderer);
                int w, h;
                SDL_GetRendererOutputSize(renderer, &w, &h);
                SDL_Rect destRect = {0, 0, w, h};
                
                if (alpha < 255)
                {
                    alpha += alphaStep;
                    if (alpha > 255)
                    alpha = 255;
                    SDL_SetTextureAlphaMod(backgroundTexture, alpha);
                }
                SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
                
                // Afficher les options du menu
                int y = 200;
                for (size_t i = 0; i < options.size(); ++i)
                {
                    renderOption(options[i], 100, y, (i == choix_actuel), maxTextWidth);
                    
                    y += 60;
                }
                
                SDL_RenderPresent(renderer);
    }
            
            SDL_DestroyTexture(backgroundTexture);
}
        void Menu::renderOption(const std::string &texte, int x, int y, bool selected, int maxWidth)
        {
            SDL_Color textColor = selected ? SDL_Color{255, 255, 255} : SDL_Color{230, 230, 230}; // Blanc ou gris clair
            
            SDL_Surface *textSurface = TTF_RenderText_Blended(font, texte.c_str(), textColor); // Blended = anti-aliasing pro
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            
            SDL_Rect bgRect = {x - 20, y - 10, maxWidth + 40, textSurface->h + 20};
            
            if (selected)
            {
                // Dégradé du violet foncé vers le bleu
                for (int i = 0; i < bgRect.h; ++i)
                {
                    Uint8 r = 70 + (30 * i) / bgRect.h; // Violet -> Bleu
                    Uint8 g = 30 + (50 * i) / bgRect.h;
                    Uint8 b = 180 + (50 * i) / bgRect.h;
                    
                    SDL_SetRenderDrawColor(renderer, r, g, b, 200); // Alpha 200 = semi-transparent
                    SDL_RenderDrawLine(renderer, bgRect.x, bgRect.y + i, bgRect.x + bgRect.w, bgRect.y + i);
                }
            }
            else
            {
                // Fond transparent gris foncé
                SDL_SetRenderDrawColor(renderer, 40, 40, 40, 150); // Gris presque noir, semi-transparent
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_RenderFillRect(renderer, &bgRect);
            }
            
            // Dessiner le texte par-dessus
            int textX = x - 20 + (maxWidth + 40 - textSurface->w) / 2;
            SDL_Rect dstRect = {textX, y, textSurface->w, textSurface->h};
            SDL_FreeSurface(textSurface);
            SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
            SDL_DestroyTexture(textTexture);
        }
        int Menu::getChoix() const
        {
            return choix_actuel;
        }
        
        
        Menu::~Menu()
        {
            TTF_CloseFont(font);
        
        }