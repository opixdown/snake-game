#include<stdio.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 20
int score = 0;
typedef struct {
    int x, y;
} Point;
void DrawRotatedTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle) {
    SDL_Rect rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}
int snakerun(SDL_Window *window,SDL_Renderer *renderer) {

    SDL_Texture* headTexture = IMG_LoadTexture(renderer, "C:/pacmanimages/head.png");
    SDL_Texture* bodyTexture = IMG_LoadTexture(renderer, "C:/pacmanimages/body.png");
    SDL_Texture* tailTexture = IMG_LoadTexture(renderer, "C:/pacmanimages/tail.png");
    SDL_Texture* foodTexture = IMG_LoadTexture(renderer, "C:/pacmanimages/food.png");

    if (!headTexture || !bodyTexture || !tailTexture || !foodTexture) {
        printf("IMG_LoadTexture Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event event;

    Point snake[100] = { {200, 200}, {180, 200}, {160, 200} };
    int snakeLength = 3;
    Point food = { 300, 300 };
    int dirX = BLOCK_SIZE, dirY = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (dirY == 0) { dirX = 0; dirY = -BLOCK_SIZE; }
                    break;
                case SDLK_DOWN:
                    if (dirY == 0) { dirX = 0; dirY = BLOCK_SIZE; }
                    break;
                case SDLK_LEFT:
                    if (dirX == 0) { dirX = -BLOCK_SIZE; dirY = 0; }
                    break;
                case SDLK_RIGHT:
                    if (dirX == 0) { dirX = BLOCK_SIZE; dirY = 0; }
                    break;
                }
            }
        }

        for (int i = snakeLength - 1; i > 0; --i) {
            snake[i] = snake[i - 1];
        }
        snake[0].x += dirX;
        snake[0].y += dirY;

        if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT) {
            running = 0;
        }

        for (int i = 1; i < snakeLength; ++i) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                running = 0;
            }
        }

        // Check food collision
        if (snake[0].x == food.x && snake[0].y == food.y) {
            snakeLength++;
            food.x = (rand() % (SCREEN_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
            food.y = (rand() % (SCREEN_HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
            score += 10;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        // Render snake with angles
        double angle;
        // Head
        if (dirX > 0) angle = 0;
        else if (dirX < 0) angle = 180;
        else if (dirY > 0) angle = 90;
        else angle = 270;
        DrawRotatedTexture(renderer, headTexture, snake[0].x, snake[0].y, angle);

        // Body
        for (int i = 1; i < snakeLength - 1; ++i) {
            DrawRotatedTexture(renderer, bodyTexture, snake[i].x, snake[i].y, 0); // Default body orientation
        }
        Point tailDirection = { snake[snakeLength - 1].x - snake[snakeLength - 2].x,
                                snake[snakeLength - 1].y - snake[snakeLength - 2].y };
        if (tailDirection.x > 0) angle = 0;
        else if (tailDirection.x < 0) angle = 180;
        else if (tailDirection.y > 0) angle = 90;
        else angle = 270;
        DrawRotatedTexture(renderer, tailTexture, snake[snakeLength - 1].x, snake[snakeLength - 1].y, angle);

        // Render food
        DrawRotatedTexture(renderer, foodTexture, food.x, food.y, 0);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    // Clean up
    SDL_DestroyTexture(headTexture);
    SDL_DestroyTexture(bodyTexture);
    SDL_DestroyTexture(tailTexture);
    SDL_DestroyTexture(foodTexture);
    printf("score %d", score);
    return 3;
}
int welcome(SDL_Window* window, SDL_Renderer* renderer) {
    // Load and create the background texture
    SDL_Surface* snake = IMG_Load("C:/pacmanimages/snake.jpg");
    if (!snake) {
        printf("Failed to load snake image: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture* snaketext = SDL_CreateTextureFromSurface(renderer, snake);
    SDL_FreeSurface(snake);
    if (!snaketext) {
        printf("Failed to create texture for snake: %s\n", SDL_GetError());
        return -1;
    }


    SDL_Surface* play = IMG_Load("C:/pacmanimages/playbuttonn.png");
    if (!play) {
        printf("Failed to load play button image: %s\n", IMG_GetError());
        SDL_DestroyTexture(snaketext);
        return -1;
    }
    SDL_Texture* playtext = SDL_CreateTextureFromSurface(renderer, play);
    SDL_FreeSurface(play);
    if (!playtext) {
        printf("Failed to create texture for play button: %s\n", SDL_GetError());
        SDL_DestroyTexture(snaketext);
        return -1;
    }

  
    SDL_Rect rectPlayButton = { 300, 400, 200, 200 };

 
    int run = 1;
    SDL_Event event;
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = 0; 
                return -1; 
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;

                
                if (x > rectPlayButton.x && x < rectPlayButton.x + rectPlayButton.w &&
                    y > rectPlayButton.y && y < rectPlayButton.y + rectPlayButton.h) {
                    SDL_DestroyTexture(snaketext);
                    SDL_DestroyTexture(playtext);
                    return 2; 
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, snaketext, NULL, NULL);
        SDL_RenderCopyEx(renderer, playtext, NULL, &rectPlayButton, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
    }

    
    SDL_DestroyTexture(snaketext);
    SDL_DestroyTexture(playtext);

    return -1; 
}
int endsrc(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("C:/pacmanimages/Roboto-Regular.ttf", 24);
    SDL_Color textColor = { 25, 55, 25, 255 };
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "SCORE: %d", score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, buffer, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_Surface*ends= IMG_Load("C:/pacmanimages/endsrc.jpg");
    SDL_Texture* endtext = SDL_CreateTextureFromSurface(renderer, ends);
    SDL_FreeSurface(ends);
    SDL_Surface* quit = IMG_Load("C:/pacmanimages/quit.png");
    SDL_Texture* quittext = SDL_CreateTextureFromSurface(renderer, quit);
    SDL_FreeSurface(quit);
    SDL_Surface* res = IMG_Load("C:/pacmanimages/restart.png");
    SDL_Texture* restext = SDL_CreateTextureFromSurface(renderer, res);
    SDL_FreeSurface(res);
    int run2 = 1;
    SDL_Event event1;
    SDL_Rect qu = { 450,400,200,200 };
    SDL_Rect re = { 250,400,200,200 };
    
    SDL_Rect textRect = {150,280,200,100};
    while (run2) {
       
        while (SDL_PollEvent(&event1)) {
            if (event1.type == SDL_QUIT) {
                run2 = 0; 
                return -1; 
            }
            else if (event1.type == SDL_MOUSEBUTTONDOWN && event1.button.button == SDL_BUTTON_LEFT) {
                int x = event1.button.x;
                int y = event1.button.y;

                
                if (x > qu.x && x < qu.x + qu.w &&
                    y > qu.y && y < qu.y + qu.h) {
                    SDL_DestroyTexture(quittext);
                    SDL_DestroyTexture(restext);
                    return -1;
                }
                else if (x > re.x && x < re.x + re.w &&
                    y > re.y && y < re.y + re.h) {
                    SDL_DestroyTexture(quittext);
                    SDL_DestroyTexture(restext);
                    return 2;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, endtext, NULL, NULL);
        SDL_RenderCopyEx(renderer, restext, NULL, &re, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, quittext, NULL, &qu, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }
}

