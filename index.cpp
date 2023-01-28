#include <chrono>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// -I and -l
// g++ index.cpp -I/usr/include/SDL2 -lSDL2
// gcc -I/usr/include/SDL2 -lSDL2

// g++ index.cpp -I/usr/include/SDL2 -lSDL2 -lSDL2_ttf -lSDL2_image

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

#define PLAYER_SPEED 50
#define PLAYER_WIDTH 192
#define PLAYER_HEIGHT 192
#define PLAYER_INITIAL_X 250
#define PLAYER_INITIAL_Y 300

class Window
{
private:
    Window();

public:
    SDL_Window *window;

    static Window *create_window(const std::string &title, std::size_t width, std::size_t height)
    {
        Window *window = new Window();
        window->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (window->window == NULL)
        {
            printf("[Window](create_window): failed to create the window\n");
            return NULL;
        }
        return window;
    }
};

class Renderer
{
private:
    SDL_Renderer *renderer;
    Renderer();

public:
    static Renderer *create_renderer(Window *window)
    {
        Renderer *renderer = new Renderer();
        renderer->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer->renderer == NULL)
        {
            printf("[Renderer](create_renderer): failed to create the renderer\n");
            return NULL;
        }
        return renderer;
    }

    void clean();
    void copy_texture();
    void copy_surface();
    // use library to work with any image type
    void copy_image();
    void update();
    void set_pixel();
};

int main()
{
    bool running;

    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow("MyWindow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect player_rect = {PLAYER_INITIAL_X, PLAYER_INITIAL_Y, PLAYER_WIDTH, PLAYER_HEIGHT};

    /*---*/

    SDL_Surface *background_surface = SDL_LoadBMP("./images/backgrounds/background-1.bmp");

    SDL_Surface *right_side_player_surface = SDL_LoadBMP("./images/players/player-1/right-side.bmp");
    SDL_Surface *left_side_player_surface = SDL_LoadBMP("./images/players/player-1/left-side.bmp");
    SDL_Surface *front_side_player_surface = SDL_LoadBMP("./images/players/player-1/front-side.bmp");
    SDL_Surface *back_side_player_surface = SDL_LoadBMP("./images/players/player-1/back-side.bmp");

    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);

    SDL_Texture *right_side_player_texture = SDL_CreateTextureFromSurface(renderer, right_side_player_surface);
    SDL_Texture *left_side_player_texture = SDL_CreateTextureFromSurface(renderer, left_side_player_surface);
    SDL_Texture *front_side_player_texture = SDL_CreateTextureFromSurface(renderer, front_side_player_surface);
    SDL_Texture *back_side_player_texture = SDL_CreateTextureFromSurface(renderer, back_side_player_surface);

    SDL_FreeSurface(background_surface);

    SDL_FreeSurface(right_side_player_surface);
    SDL_FreeSurface(left_side_player_surface);
    SDL_FreeSurface(front_side_player_surface);
    SDL_FreeSurface(back_side_player_surface);

    SDL_Surface *water_surface = SDL_LoadBMP("./images/backgrounds/background-water.bmp");
    SDL_Texture *water_texture_1 = SDL_CreateTextureFromSurface(renderer, water_surface);
    SDL_Texture *water_texture_2 = SDL_CreateTextureFromSurface(renderer, water_surface);
    SDL_FreeSurface(water_surface);

    /*---*/

    TTF_Font *font = TTF_OpenFont("./fonts/OpenSans/Bold.ttf", 24);

    SDL_Surface *message_surface =
        TTF_RenderText_Solid(font, "hello am raiden", {255, 0, 0, 255});

    SDL_Texture *message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);

    SDL_FreeSurface(message_surface);

    SDL_Rect message_rect = {0, 0, 300, 100};

    /*---*/

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, background_texture, NULL, NULL);

    SDL_RenderCopy(renderer, front_side_player_texture, NULL, &player_rect);

    SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);

    SDL_RenderPresent(renderer);

    running = true;

    SDL_Texture *state = front_side_player_texture;

    while (running)
    {
        SDL_Event event;

        // Handle input events
        // Handle Update
        // Clear and Draw Screen

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EventType::SDL_QUIT:
                running = false;
                printf("[system](event): quit\n");
                break;

            case SDL_EventType::SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDL_KeyCode::SDLK_SPACE:
                    printf("[system]: jump\n");
                    break;
                case SDL_KeyCode::SDLK_UP:
                case SDL_KeyCode::SDLK_z:
                    printf("[system](click): up\n");
                    state = back_side_player_texture;
                    player_rect.y -= PLAYER_SPEED;
                    break;

                case SDL_KeyCode::SDLK_DOWN:
                case SDL_KeyCode::SDLK_s:
                    printf("[system](click): down\n");
                    state = front_side_player_texture;
                    player_rect.y += PLAYER_SPEED;
                    break;

                case SDL_KeyCode::SDLK_LEFT:
                case SDL_KeyCode::SDLK_q:
                    printf("[system](click): left\n");
                    state = left_side_player_texture;
                    player_rect.x -= PLAYER_SPEED;
                    break;

                case SDL_KeyCode::SDLK_RIGHT:
                case SDL_KeyCode::SDLK_d:
                    printf("[system](click): right\n");
                    state = right_side_player_texture;
                    player_rect.x += PLAYER_SPEED;
                    break;

                default:
                    printf("[system](clikc): unrecognised\n");
                    break;
                }

                /*normalise y*/
                if (player_rect.y <= 200)
                    player_rect.y = 200;
                /*---*/

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_RenderCopy(renderer, background_texture, NULL, NULL);

                SDL_RenderCopy(renderer, state, NULL, &player_rect);

                SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);

                SDL_RenderPresent(renderer);

                break;

            default:
                printf("[system](event): not handled\n");
                break;
            }
        }
    }

    SDL_DestroyTexture(background_texture);

    SDL_DestroyTexture(right_side_player_texture);
    SDL_DestroyTexture(left_side_player_texture);
    SDL_DestroyTexture(front_side_player_texture);
    SDL_DestroyTexture(back_side_player_texture);

    SDL_DestroyTexture(message_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    TTF_CloseFont(font);
    IMG_Quit();
    TTF_Quit();

    return 0;
}

/*
SDL_RenderDrawLine
SDL_RenderDrawLineF
SDL_RenderDrawLines
SDL_RenderDrawLinesF
SDL_RenderDrawPoint
SDL_RenderDrawPointF
SDL_RenderDrawPoints
SDL_RenderDrawPointsF
SDL_RenderDrawRect
SDL_RenderDrawRectF
SDL_RenderDrawRects
SDL_RenderDrawRectsF
*/
