#ifndef Unlimite1_HPP
#define Unlimite1_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <SDL_mixer.h>

struct Obstacle {
    SDL_Rect rect;
    SDL_Texture* texture;
};
class GameMode11
{
public:
    GameMode11(SDL_Window* win, SDL_Renderer* rend);
    ~GameMode11();
    void handleEvents();
    void update();
    void render();
    void run();
    bool checkCollision(const SDL_Rect &a, const SDL_Rect &b);
    void generateObstacle(Obstacle &ob);

private:
    SDL_Texture* backgroundTexture;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect player;
    Mix_Chunk* jumpSound = nullptr;
    Mix_Chunk* hitSound = nullptr;
    Mix_Chunk* winSound = nullptr;
    Mix_Chunk* loseSound = nullptr;
    Mix_Music* gameMusic = nullptr;
    int lives = 3;
    bool isInvincible = false;
    Uint32 invincibilityStartTime = 0;
    Uint32 invincibilityDuration = 3000; // 3 seconds
    Uint32 lastHitTime = 0;
    const Uint32 hitCooldown = 1000; // 1 second of invincibility after getting hit
    SDL_Rect ground;
    int PLAYER_WIDTH;
    int PLAYER_HEIGHT;
    int groundHeight;
    SDL_Texture* playerTexture;
    SDL_Texture* heartTexture = NULL;
    std::vector<SDL_Texture*> obstacleTextures;
    std::vector<Obstacle> obstacles;    
    int level;
    int score = 0;
    Uint32 lastScoreUpdateTime = 0;
    int timeElapsed;
    int lastTimeCheckpoint = 0;
    Uint32 startTime;
    int nextObstacleX;
    float velocityX;
    float velocityY;
    const float gravity = 0.5f;
    const float jumpStrength = -10.0f;
    int jumpCount;
    TTF_Font *font;
    TTF_Font *infoFont;
    bool running;
    bool gameWon = false;
    bool gameLost = false;
};

#endif
