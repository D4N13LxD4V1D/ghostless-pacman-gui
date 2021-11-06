#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Map Settings
#define MAP_X 30
#define MAP_Y 30
#define BLOCKS 10

// Windows Settings
#define SCALE 1
#define WIDTH MAP_X * SCALE * (MAP_X + 2)
#define HEIGHT MAP_Y * SCALE * (MAP_Y + 2)
#define FPS 60

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

	while (true) {
		x = rand() % MAP_X;
		y = rand() % MAP_Y;

		if (map[x][y] == PATH && !(x == 0 && y == 0) && !(x == 0 && y == 1) && !(x == 1 && y == 0)) {
			map[x][y] = EXIT;
			break;
		}
	}
}


void renderMap(SDL_Renderer *rend, int map[MAP_X][MAP_Y]) {
  SDL_Rect mapBlocks[MAP_X][MAP_Y];

	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
      mapBlocks[x][y].x = WIDTH/(MAP_X+2)+(WIDTH/(MAP_X+2))*x;
      mapBlocks[x][y].y = HEIGHT/(MAP_Y+2)+(HEIGHT/(MAP_Y+2))*y;
      mapBlocks[x][y].w = WIDTH/(MAP_X+2);
      mapBlocks[x][y].h = HEIGHT/(MAP_Y+2);

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
    border.x = (WIDTH/(MAP_X+2))*x;
    border.y = 0;
    border.w = WIDTH/(MAP_X+2);
    border.h = HEIGHT/(MAP_Y+2);

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);

    border.x = (WIDTH/(MAP_X+2))*x;
    border.y = HEIGHT - HEIGHT/(MAP_Y+2);
    border.w = WIDTH/(MAP_X+2);
    border.h = HEIGHT/(MAP_Y+2);

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);
  }
  for (int y = 0; y < MAP_Y + 2; y++) {
    SDL_Rect border;
    border.x = 0;
    border.y = (HEIGHT/(MAP_Y+2))*y;
    border.w = WIDTH/(MAP_X+2);
    border.h = HEIGHT/(MAP_Y+2);

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);

    border.x = WIDTH - WIDTH/(MAP_X+2);
    border.y = (HEIGHT/(MAP_Y+2))*y;
    border.w = WIDTH/(MAP_X+2);
    border.h = HEIGHT/(MAP_Y+2);

    SDL_SetRenderDrawColor(rend, 86,86,255,255);
    SDL_RenderFillRect(rend, &border);
  }
}

void renderPlayer(SDL_Renderer *rend, int playerX, int playerY) {
  SDL_Texture* player = IMG_LoadTexture(rend, "icon.png");
	SDL_Rect texr;
  texr.x = WIDTH/(MAP_X+2)+(WIDTH/(MAP_X+2))*playerX;
  texr.y = HEIGHT/(MAP_Y+2)+(HEIGHT/(MAP_Y+2))*playerY;
  texr.w = WIDTH/(MAP_X+2);
  texr.h = HEIGHT/(MAP_Y+2);
  SDL_RenderCopy(rend, player, NULL, &texr);
  SDL_DestroyTexture(player);
}

int checkGameStatus(int map[MAP_X][MAP_Y], int playerPositionX, int playerPositionY, int *remainingFood) {
	if (playerPositionX < 0 || playerPositionX > MAP_X || playerPositionY < 0 || playerPositionY > MAP_Y)
		return GAME_LOST;
	switch (map[playerPositionX][playerPositionY]) {
	case BLOCK:
		return GAME_LOST;
	case FOOD:
		(*remainingFood)--;
		return GAME_PLAYING;
	case EXIT:
		if (!(*remainingFood)) {
			return GAME_WON;
    }
	default:
		return GAME_PLAYING;
	}
}

int startGame(int numOfFoods) {
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
  SDL_Event event;

	srand(SDL_GetTicks());

	int map[MAP_X][MAP_Y];
	int remainingFood = numOfFoods;
	int gameState = GAME_PLAYING;
	int playerX = 0, playerY = 0;

	fillPath(map);
	generateBlocks(map, BLOCKS);
	generateFoods(map, numOfFoods);
	generateExit(map);


  while (gameState == GAME_PLAYING) {
    /* Check game state */
    gameState = checkGameStatus(map, playerX, playerY, &remainingFood);
    map[playerX][playerY] = AIR;
    
    /* Process keyboard events */
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          gameState = GAME_STOPPED;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_M:
            case SDL_SCANCODE_ESCAPE:
              gameState = GAME_PAUSED;
              break;
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

    /* Draw the map */
    renderMap(rend, map);

    /* Draw the player */
    renderPlayer(rend, playerX, playerY);

    /* Draw to window and loop */
    SDL_RenderPresent(rend);
    SDL_Delay(1000/FPS);
  }

  /* Release resources */
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);
  SDL_Quit();

  return gameState;
}

int main(int argc, char* argv[])
{
  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 0;
  }
  printf("%d",startGame(9));
  return 0;
}