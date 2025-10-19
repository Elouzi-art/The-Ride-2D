#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <SDL_image.h>
#include <vector>
#include "jeu1.hpp"
#include "SDL_mixer.h"
static int SCREEN_HEIGHT;
static int SCREEN_WIDTH;

GameMap1::GameMap1(SDL_Window* win, SDL_Renderer* rend)
    : window(win), renderer(rend), running(true), level(0),score(0), timeElapsed(0), startTime(SDL_GetTicks())
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Erreur lors d'initialisation de SDL: " << SDL_GetError() << std::endl;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Erreur lors d'initialisation de SDL_mixer : " << Mix_GetError() << std::endl;
    }
    jumpSound = Mix_LoadWAV("audio/jump.wav");
    Mix_VolumeChunk(jumpSound, MIX_MAX_VOLUME / 4);  // pour le 25% volume

        if (!jumpSound) std::cerr << "Erreur lors d'ajoutement de l'audio Jump: " << Mix_GetError() << std::endl;

    hitSound = Mix_LoadWAV("audio/hit.wav");
        if (!hitSound) std::cerr << "Erreur lors d'ajoutement de l'audio hit: " << Mix_GetError() << std::endl;

    winSound = Mix_LoadWAV("audio/win.wav");
        if (!winSound) std::cerr << "Erreur lors d'ajoutement de l'audio win: " << Mix_GetError() << std::endl;

    loseSound = Mix_LoadWAV("audio/lose.wav");
        if (!loseSound) std::cerr << "Erreur lors d'ajoutement de l'audio lose: " << Mix_GetError() << std::endl;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();                                  
    srand(static_cast<unsigned>(time(nullptr))); // pour les obstacles aleatoires
    // creation de la fenetre, player , obstacle dynamiquement
    window = SDL_CreateWindow("Jeu - Course", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    groundHeight = SCREEN_HEIGHT / 12;
    PLAYER_WIDTH = SCREEN_WIDTH / 15;
    PLAYER_HEIGHT = SCREEN_HEIGHT / 10;
    
    player = {
        SCREEN_WIDTH / 10,
        SCREEN_HEIGHT - groundHeight - PLAYER_HEIGHT,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };
    
    ground = {
        0,
        SCREEN_HEIGHT - groundHeight,
        SCREEN_WIDTH,
        groundHeight
    };
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* bgSurface = IMG_Load("res/map1.jpg");
    if (!bgSurface) {
    std::cerr << "Erreur lors d'ajoutement du map: " << IMG_GetError() << std::endl;
} else {
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
}

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); 
    SDL_Surface* surface = IMG_Load("res/player1.png"); 
if (!surface)
{
    std::cerr << "Erreur lors d'ajoutement du player" << SDL_GetError() << std::endl;
}
else
{
    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!playerTexture)
    {
        std::cerr << "Failed to create player texture: " << SDL_GetError() << std::endl;
    }
    SDL_Surface* tempSurface = IMG_Load("res/heart.png");
        if (!tempSurface) {
            std::cout << "Failed to load heart image: " << IMG_GetError() << std::endl;
            // handle error
        }
        heartTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

}
std::vector<std::string> obstaclePaths = {
    "res/obstacle1.png",
    "res/obstacle2.png",
    "res/obstacle3.png"
};

for (const std::string& path : obstaclePaths) {
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) {
        std::cerr << "Failed to load " << path << ": " << IMG_GetError() << std::endl;
        continue;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (tex)
        obstacleTextures.push_back(tex);
    else
        std::cerr << "Failed to create texture for " << path << ": " << SDL_GetError() << std::endl;
}

    // Charger la police pour afficher le temps
    font = TTF_OpenFont("res/ARIALBLACKITALIC.TTF", 24);
    infoFont = TTF_OpenFont("res/ARIALBLACKITALIC.TTF", 16);

    if (!font)
    {
        std::cerr << "Erreur de chargement de la police: " << TTF_GetError() << std::endl;
    }

    // Initialisation des obstacles
    nextObstacleX = SCREEN_HEIGHT;
    for (int i = 0; i < 10; ++i)
    {
        M1_Obstacle ob;
        generateObstacle(ob);
        obstacles.push_back(ob);
    }
    finishLine.w = SCREEN_WIDTH / 20;
    finishLine.h = SCREEN_HEIGHT / 3;
    finishLine.y = SCREEN_HEIGHT - groundHeight - finishLine.h;
    finishLine.x = obstacles.back().rect.x + 800; 
    // initialisation du point d'arrive
    SDL_Surface* finishSurf = IMG_Load("res/finishline.png");
        if (!finishSurf) 
        {
            std::cerr << "Failed to load finish line: " << IMG_GetError() << std::endl;
        } else 
        {
            finishLineTexture = SDL_CreateTextureFromSurface(renderer, finishSurf);
            SDL_FreeSurface(finishSurf);
        }    
}


void GameMap1::handleEvents()
{
    SDL_Event event;
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
            else if (event.key.keysym.sym == SDLK_SPACE && jumpCount < 2)
            {
                velocityY = jumpStrength;
                jumpCount++;
                Mix_PlayChannel(-1, jumpSound, 0);
            }
            else if ((gameWon || gameLost) && event.key.keysym.sym == SDLK_r) {
                // Rejouer si l'utilisateur tape R
                gameMusic = Mix_LoadMUS("audio/game_background.wav");
                if (!gameMusic) {
                    std::cout << "Erreur chargement musique : " << Mix_GetError() << std::endl;
                } else {
                    Mix_PlayMusic(gameMusic, -1); // -1 signifie une boucle infine
                }   
                gameWon = false;
                gameLost = false;
                level = 0;
                score = 0;
                lives = 3;
                timeElapsed = 0;
                lastLevelTime = 0;
                startTime = SDL_GetTicks();
                velocityY = 0;
                jumpCount = 0;
                nextObstacleX = SCREEN_HEIGHT;
                
                player = {
                    SCREEN_WIDTH / 10,
                    SCREEN_HEIGHT - groundHeight - PLAYER_HEIGHT,
                    PLAYER_WIDTH,
                    PLAYER_HEIGHT
                };
                
                // Regenerer les obstacles
                obstacles.clear();
                for (int i = 0; i < 10; ++i) {
                    M1_Obstacle ob;
                    generateObstacle(ob);
                    obstacles.push_back(ob);
                }
                
                finishLine.w = SCREEN_WIDTH / 20;
                finishLine.h = SCREEN_HEIGHT / 3;
                finishLine.y = SCREEN_HEIGHT - groundHeight - finishLine.h;
                finishLine.x = obstacles.back().rect.x + 800;
                
                running = true;
            }
            
        }
        else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            // sauvegarde de des dimensions anciennes pour calculer les nouveux dimensions
            int oldWidth = SCREEN_WIDTH;
            int oldHeight = SCREEN_HEIGHT;
            // mise a jour des nouveaux dimensions
            SCREEN_WIDTH = event.window.data1;
            SCREEN_HEIGHT = event.window.data2;
            // calcule des factors 
            float scaleX = static_cast<float>(SCREEN_WIDTH) / oldWidth;
            float scaleY = static_cast<float>(SCREEN_HEIGHT) / oldHeight;
            // redimensionement des joueurs et la surface
            groundHeight = SCREEN_HEIGHT / 12;
            PLAYER_WIDTH = SCREEN_WIDTH / 15;
            PLAYER_HEIGHT = SCREEN_HEIGHT / 10;
            player.w = PLAYER_WIDTH;
            player.h = PLAYER_HEIGHT;
            player.x = SCREEN_WIDTH / 10;
            player.y = SCREEN_HEIGHT - groundHeight - player.h;
            ground = {
                0,
                SCREEN_HEIGHT - groundHeight,
                SCREEN_WIDTH,
                groundHeight
            };
            // redimensionner les obstacles pour les nouveux dimensions
            for (auto& ob : obstacles) {
                ob.rect.x = static_cast<int>(ob.rect.x * scaleX);
                ob.rect.y = static_cast<int>(ob.rect.y * scaleY);
                ob.rect.w = static_cast<int>(ob.rect.w * scaleX);
                ob.rect.h = static_cast<int>(ob.rect.h * scaleY);
            }
            
            // redimensionner le point d'arrive
            finishLine.x = static_cast<int>(finishLine.x * scaleX);
            finishLine.y = SCREEN_HEIGHT - groundHeight - finishLine.h;
            finishLine.w = SCREEN_WIDTH / 20;
            finishLine.h = SCREEN_HEIGHT / 3;
        }
        
    }
}

void GameMap1::update()
{
    if (gameWon || gameLost) return; // si le jeu termine on update pas
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > lastScoreUpdateTime + 200) { 
        score += 1;
        lastScoreUpdateTime = currentTime;
    }
        timeElapsed = maxAllowedTime - ((SDL_GetTicks() - startTime) / 1000);
    if (timeElapsed <= 0) {
        Mix_PlayChannel(-1, loseSound, 0);
        gameLost = true;
        timeElapsed = 0; // fixe sur 0 pour ne pas avoir des nombres negatifs
        return;
    }
    
    
    const int spacingMin = 300;
    const int spacingMax = 500;
    
    int elapsed = (SDL_GetTicks() - startTime) / 1000;
    if (elapsed / 10 > lastLevelTime)
    {
        level++;
        velocityX += 1; // optional: increase difficulty
        lastLevelTime = elapsed / 10;
    }
    
    for (auto &ob : obstacles) {
        ob.rect.x -= 5;
        
        if (ob.rect.x + ob.rect.w < 0)
        generateObstacle(ob);
    }
    finishLine.x -= 5;
    velocityY += gravity;
    player.y += static_cast<int>(velocityY);
    
    for (auto &ob : obstacles)
    {
        if (checkCollision(player, ob.rect)) {
            int playerBottom = player.y + player.h;
            int prevBottom = playerBottom - static_cast<int>(velocityY);
            
            if (prevBottom <= ob.rect.y && velocityY > 0) {
                player.y = ob.rect.y - PLAYER_HEIGHT;
                velocityY = 0;
                jumpCount = 0;
            } else {
                Uint32 currentTime = SDL_GetTicks();
                if (!isInvincible && currentTime - lastHitTime >= hitCooldown) {
                    lives--;
                    isInvincible = true;
                    invincibilityStartTime = SDL_GetTicks();
                    lastHitTime = currentTime;
                    Mix_PlayChannel(-1, hitSound, 0);
                    
                    if (lives <= 0) {
                        Mix_HaltMusic();
                        Mix_PlayChannel(-1, loseSound, 0);
                        gameLost = true;
                        return;
                    }
                }

            }
        }
    }
    
    // verifier si le joueur collise avec le pont d'arrivé
    if (checkCollision(player, finishLine)) {
        Mix_HaltMusic();
        Mix_PlayChannel(-1, winSound, 0);
        gameWon = true;
        return;
    }
    
    if (player.y + player.h >= SCREEN_HEIGHT - groundHeight)
    {
        player.y = SCREEN_HEIGHT - groundHeight - player.h;
        velocityY = 0;
        jumpCount = 0;
    }
    if (isInvincible) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - invincibilityStartTime >= invincibilityDuration) {
            isInvincible = false;
        }
    }
    
}
    
void GameMap1::render()
{
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    SDL_Rect bgRect = { 0, 0, windowWidth, windowHeight };
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);
    
    for (const auto& ob : obstacles)
    {
        if (ob.texture)
        SDL_RenderCopy(renderer, ob.texture, nullptr, &ob.rect);
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &ob.rect);
        }
    }
    // affichage du joueur
    if (!gameWon && !gameLost) {
        if (!isInvincible || (SDL_GetTicks() / 100) % 2 == 0) {
            SDL_RenderCopy(renderer, playerTexture, NULL, &player);
        }
    } else {
        SDL_RenderCopy(renderer, playerTexture, NULL, &player);
    }

    
    for (int i = 0; i < lives; i++) {
        SDL_Rect heartRect = {SCREEN_WIDTH - 10 - (lives - i) * 40, 10, 30, 30};
        SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
    }
    
    
    std::string timeText = "Time: " + std::to_string(timeElapsed) + "s";
    std::string levelText = "Level: " + std::to_string(level);
    
    SDL_Color textColor = {255, 0, 0};
    
    // Time
    SDL_Surface* timeSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
    SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(renderer, timeSurface);
    
    SDL_Rect timeRect;
    timeRect.w = timeSurface->w;
    timeRect.h = timeSurface->h;
    timeRect.x = (windowWidth - timeRect.w) / 2;
    timeRect.y = 20;
    SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
    
    // level
    SDL_Surface* levelSurface = TTF_RenderText_Solid(font, levelText.c_str(), textColor);
    SDL_Texture* levelTexture = SDL_CreateTextureFromSurface(renderer, levelSurface);
    
    SDL_Rect levelRect;
    levelRect.w = levelSurface->w;
    levelRect.h = levelSurface->h;
    levelRect.x = (windowWidth - levelRect.w) / 2;
    levelRect.y = timeRect.y + timeRect.h + 10;
    SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);

    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Color yellow = {255, 255, 0};

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), yellow);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_Rect scoreRect;
    scoreRect.w = scoreSurface->w;
    scoreRect.h = scoreSurface->h;
    scoreRect.x = 20;
    scoreRect.y = 20;

    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    
    if (gameWon || gameLost)
    {
        std::string resultText = gameWon ? "YOU WIN!" : "YOU LOSE!";
        SDL_Color resultColor = gameWon ? SDL_Color{0, 255, 0} : SDL_Color{255, 0, 0};
        
        SDL_Surface* resultSurface = TTF_RenderText_Solid(font, resultText.c_str(), resultColor);
        SDL_Texture* resultTexture = SDL_CreateTextureFromSurface(renderer, resultSurface);
        
        SDL_Rect resultRect;
        resultRect.w = resultSurface->w;
        resultRect.h = resultSurface->h;
        resultRect.x = (windowWidth - resultRect.w) / 2;
        resultRect.y = windowHeight / 2;
        
        SDL_RenderCopy(renderer, resultTexture, NULL, &resultRect);
        
        SDL_FreeSurface(resultSurface);
        SDL_DestroyTexture(resultTexture);

        std::string scoreText = "Votre Score : " + std::to_string(score);
        SDL_Color scoreColor = {255, 255, 0}; // white color for instructions
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), scoreColor);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        
        SDL_Rect scoreRect;
        scoreRect.w = scoreSurface->w;
        scoreRect.h = scoreSurface->h;
        scoreRect.x = (windowWidth - scoreRect.w) / 2;
        scoreRect.y = resultRect.y + resultRect.h + 10; // Below result text
        
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        std::string infoText = "(Pressez R pour reessayer ou ESC pour Quitter)";
        SDL_Color infoColor = {255, 255, 255}; // white color for instructions
        SDL_Surface* infoSurface = TTF_RenderText_Solid(infoFont, infoText.c_str(), infoColor);
        SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(renderer, infoSurface);
        
        SDL_Rect infoRect;
        infoRect.w = infoSurface->w;
        infoRect.h = infoSurface->h;
        infoRect.x = (windowWidth - infoRect.w) / 2;
        infoRect.y = scoreRect.y + scoreRect.h + 10; // Below score text
        
        SDL_RenderCopy(renderer, infoTexture, NULL, &infoRect);
        
        SDL_FreeSurface(infoSurface);
        SDL_DestroyTexture(infoTexture);
        }
        
        if (finishLineTexture)
        SDL_RenderCopy(renderer, finishLineTexture, nullptr, &finishLine);
        
        SDL_FreeSurface(timeSurface);
        SDL_FreeSurface(levelSurface);
        SDL_DestroyTexture(timeTexture);
        SDL_DestroyTexture(levelTexture);
        
        SDL_RenderPresent(renderer);
}


void GameMap1::run()
{
    player = {100, SCREEN_HEIGHT - groundHeight - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT};
    velocityY = 100;
    jumpCount = 0;
    nextObstacleX = 1280;
    if(Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
    gameMusic = Mix_LoadMUS("audio/game_background.wav");
    if (!gameMusic) {
        std::cout << "Erreur chargement musique : " << Mix_GetError() << std::endl;
    } else {
        Mix_PlayMusic(gameMusic, -1); // 
            }   
    while (running)
    {     
        
        handleEvents();
        update();
        render();
        SDL_Delay(16 - level * 2);
    }
}

bool GameMap1::checkCollision(const SDL_Rect &a, const SDL_Rect &b)
{
    return SDL_HasIntersection(&a, &b);
}
// Methode de generation des obstacles en se basant sur la fenetre donne
void GameMap1::generateObstacle(M1_Obstacle &ob)
{
    const int spacingMin = 300;
    const int spacingMax = 500;
    const int obstacleLevels[] = {250, 500, 650};
    
    ob.rect.w = SCREEN_WIDTH / 10 + rand() % (SCREEN_WIDTH / 20);
    int levelY = obstacleLevels[rand() % 3];
    ob.rect.h = SCREEN_HEIGHT / 10 + rand() % (SCREEN_HEIGHT / 10);
    ob.rect.y = SCREEN_HEIGHT - groundHeight - ob.rect.h;
    
    nextObstacleX += spacingMin + rand() % (spacingMax - spacingMin + 1);
    ob.rect.x = nextObstacleX;
    
    if (!obstacleTextures.empty()) {
        ob.texture = obstacleTextures[rand() % obstacleTextures.size()];
    } else {
        ob.texture = nullptr;
    }
}

// destructeur
GameMap1::~GameMap1()
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(winSound);
    Mix_FreeChunk(loseSound);
    Mix_FreeMusic(gameMusic);
    SDL_Quit();
    TTF_Quit();
}