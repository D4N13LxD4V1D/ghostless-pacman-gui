#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WIDTH 720
#define HEIGHT 720
#define FPS 60

// Map Settings
#define MAP_X 10
#define MAP_Y 10
#define BLOCKS 10

// Block IDs
#define AIR 0
#define PATH 1
#define BLOCK 2
#define FOOD 3
#define EXIT 4
#define PLAYER 5

// Game States
#define GAME_STOPPED -2
#define GAME_LOST -1
#define GAME_PAUSED 0
#define GAME_PLAYING 1
#define GAME_WON 2


void fillPath(int map[MAP_X][MAP_Y]) {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
      if (x==0 && y==0)
        continue;
			map[x][y] = PATH;
    }
  }
}

void generateBlocks(int map[MAP_X][MAP_Y], int numOfBlocks) {
	int x, y;

	while (numOfBlocks > 0) {
		x = rand() % MAP_X;
		y = rand() % MAP_Y;

		if (map[x][y] == PATH && !(x == 0 && y == 0) && !(x == 0 && y == 1) && !(x == 1 && y == 0)) {
			map[x][y] = BLOCK;
			numOfBlocks--;
		}
	}
}

void generateFoods(int map[MAP_X][MAP_Y], int numOfFoods) {
	int x, y;

	while (numOfFoods > 0) {
		x = rand() % MAP_X;
		y = rand() % MAP_Y;

		if (map[x][y] == PATH && !(x == 0 && y == 0) && !(x == 0 && y == 1) && !(x == 1 && y == 0)) {
			map[x][y] = FOOD;
			numOfFoods--;
		}
	}
}

void generateExit(int map[MAP_X][MAP_Y]) {
	int x, y;
	int numberOfExits = 1;

	while (numberOfExits > 0) {
		x = rand() % MAP_X;
		y = rand() % MAP_Y;

		if (map[x][y] == PATH && !(x == 0 && y == 0) && !(x == 0 && y == 1) && !(x == 1 && y == 0)) {
			map[x][y] = EXIT;
			numberOfExits--;
		}
	}
}


void renderMap(SDL_Renderer *rend, int map[MAP_X][MAP_Y], int playerX, int playerY) {
  SDL_Texture* player = IMG_LoadTexture(rend, "icon.png");
	SDL_Rect texr;
  texr.x = WIDTH/MAP_X+(WIDTH/MAP_X)*playerX;
  texr.y = HEIGHT/MAP_Y+(HEIGHT/MAP_X)*playerY;
  texr.w = WIDTH/MAP_X;
  texr.h = HEIGHT/MAP_Y;
  SDL_RenderCopy(rend, player, NULL, &texr);

  SDL_Rect mapBlocks[MAP_X][MAP_Y];

	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
      mapBlocks[x][y].x = WIDTH/MAP_X+(WIDTH/MAP_X)*x;
      mapBlocks[x][y].y = HEIGHT/MAP_Y+(HEIGHT/MAP_X)*y;
      mapBlocks[x][y].w = WIDTH/MAP_X;
      mapBlocks[x][y].h = HEIGHT/MAP_Y;

      switch (map[x][y]) {
        case PATH:
          mapBlocks[x][y].w /= 4;
          mapBlocks[x][y].h /= 4;
          mapBlocks[x][y].x += 2*mapBlocks[x][y].w - mapBlocks[x][y].w/2;
          mapBlocks[x][y].y += 2*mapBlocks[x][y].h - mapBlocks[x][y].h/2;
          SDL_SetRenderDrawColor(rend, 254,254,172,255);
          SDL_RenderFillRect(rend, &mapBlocks[x][y]);
          break;
        case FOOD:
          SDL_SetRenderDrawColor(rend, 254,254,172,255);
          SDL_RenderFillRect(rend, &mapBlocks[x][y]);
          break;
        case BLOCK:
          SDL_SetRenderDrawColor(rend, 86,86,255,255);
          SDL_RenderFillRect(rend, &mapBlocks[x][y]);
          break;
        case EXIT:
          SDL_SetRenderDrawColor(rend, 255,171,255,255);
          SDL_RenderFillRect(rend, &mapBlocks[x][y]);
          break;
        default:
          break;
      }
    }
  }
  
  for (int x = 0; x < MAP_X+2; x++) {
    SDL_Rect border;
    border.x = (WIDTH/MAP_X)*x;
    border.y = 0;
    border.w = WIDTH/MAP_X;
    border.h = HEIGHT/MAP_X;

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);

    border.x = (WIDTH/MAP_X)*x;
    border.y = HEIGHT - HEIGHT/MAP_X;
    border.w = WIDTH/MAP_X;
    border.h = HEIGHT/MAP_X;

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);
  }
  for (int y = 0; y < MAP_Y + 2; y++) {
    SDL_Rect border;
    border.x = 0;
    border.y = (HEIGHT/MAP_X)*y;
    border.w = WIDTH/MAP_X;
    border.h = HEIGHT/MAP_X;

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);border;

    border.x = WIDTH - WIDTH/MAP_X;
    border.y = (HEIGHT/MAP_X)*y;
    border.w = WIDTH/MAP_X;
    border.h = HEIGHT/MAP_X;

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);
  }

  SDL_DestroyTexture(player);
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

  int numOfFoods = 2;

	srand(SDL_GetTicks());

	int map[MAP_X][MAP_Y];
	int remainingFood = numOfFoods;
	int gameState = GAME_PLAYING;
	int playerX = 0, playerY = 0;

	fillPath(map);
	generateBlocks(map, BLOCKS);
	generateFoods(map, numOfFoods);
	generateExit(map);

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
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
              playerY -= 1;
              break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
              playerY += 1;
              break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              playerX -= 1;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              playerX += 1;
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
    renderMap(rend, map, playerX, playerY);

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