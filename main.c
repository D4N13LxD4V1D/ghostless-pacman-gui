#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WIDTH 640
#define HEIGHT 640
#define FPS 60

// Map Settings
#define MAP_X 10
#define MAP_Y 10
#define BLOCKS 10

// Display Configuration
#define AIR {1, 1, 10, 10}
#define BLOCK '#'
#define FOOD '*'
#define EXIT '$'
#define PLAYER 'O'

// Game States
#define GAME_STOPPED -2
#define GAME_LOST -1
#define GAME_PAUSED 0
#define GAME_PLAYING 1
#define GAME_WON 2

void fillAir(SDL_Rect map[MAP_X][MAP_Y]) {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
      map[x][y].x = 50 + 20*x;
      map[x][y].y = 50 + 20*y;
      map[x][y].w = 10;
      map[x][y].h = 10;
    }
  }
}


void renderMap(SDL_Renderer *rend, SDL_Rect map[MAP_X][MAP_Y]) {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
      SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
      SDL_RenderFillRect(rend, &map[x][y]);
    }
  }
}


int main(int argc, char* argv[])
{
  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 0;
  }

  /* Create a window */
  SDL_Window* wind = SDL_CreateWindow("Ghostless Pacman",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WIDTH, HEIGHT, 0);
  if (!wind)
  {
    printf("Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }

  /* Set window icon */
  SDL_Surface* icon = IMG_Load("icon.png");
  SDL_SetWindowIcon(wind, icon); 

  /* Create a renderer */
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);
  if (!rend)
  {
    printf("Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
  }

  /* Main game loop */
  bool running = true, jump_pressed = false, can_jump = true,
                  left_pressed = false, right_pressed = false;
  SDL_Event event;

  SDL_Rect map[MAP_X][MAP_Y];
  fillAir(map);

  while (running)
  {
    /* Process keyboard events */
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left_pressed = true;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right_pressed = true;
              break;
            default:
              break;
            }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left_pressed = false;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right_pressed = false;
              break;
            default:
              break;
            }
          break;
        default:
          break;
      }
    }

    /* Clear screen */
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    /* Move the player */
    
    /* Draw the map */
    renderMap(rend, map);

    /* Draw the player */

    /* Draw to window and loop */
    SDL_RenderPresent(rend);
    SDL_Delay(1000/FPS);
  }

  /* Release resources */
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);
  SDL_Quit();
  return 0;
}