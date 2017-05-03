/*
 *	Created By: Nick Kane
 */

#include <SDL.h>
#include <SDL_image.h>

#include "window_state.h"

#include <stdio.h>
#include <stdlib.h>

#define global_variable static

global_variable const int Screen_Width = 640;
global_variable const int Screen_Height = 480;
global_variable const int Sdl_Image_Flags = IMG_INIT_PNG;

// TODO(nick): rename this ...
global_variable WindowState *Window; 
global_variable bool GameRunning = true;
global_variable SDL_RWops *ReadWriteOperations;

// TODO(nick): move structs in to a .h file

struct AssetTexture
{
	SDL_Rect RenderBox;
	SDL_Texture *Texture;
	int MeterHeight;
	int MeterWidth;
};

struct Entity
{
	AssetTexture *CurrentTexture;
};

global_variable Entity *PlayerEntity;

SDL_Window *
InitializeGameWindow();

bool
InitializeAssetPipeline();

WindowState *
InitializeGame();

AssetTexture *
LoadAsset(SDL_RWops *, SDL_Surface *, SDL_Renderer *);

int
main(int argc, char *argv[])
{
	// TODO(nick): add this maybe to gamestate / windowstate?
	SDL_Event CurrentEvent;

	Window = InitializeGame();

	if(Window->GameWindow != NULL)
	{
		// game initialized successfully

		while (GameRunning)
		{
			while (SDL_PollEvent(&CurrentEvent))
			{
				switch (CurrentEvent.type)
				{
					case SDL_QUIT:
					{
						GameRunning = false;
					} break;

					// TODO(nick): figure out a better way to handle up / release
					// key presses
					// TODO(nick): debug this - key press / release is broken
					case SDL_KEYDOWN:
					{
						switch (CurrentEvent.key.keysym.sym)
						{
							case SDLK_UP: 
							{
								printf("arrow up pressed\n");
							} break;

							case SDLK_DOWN:
							{
								printf("arrow down pressed\n");
							} break;

							case SDLK_LEFT:
							{
								printf("arrow left pressed\n");
							} break;

							case SDLK_RIGHT:
							{
								printf("arrow right pressed\n");
							} break;

							case SDLK_w: 
							{
								printf("w key pressed\n");
							} break;

							case SDLK_a:
							{
								printf("a key pressed\n");
							} break;

							case SDLK_s:
							{
								printf("s key pressed\n");
							} break;

							case SDLK_d:
							{
								printf("d key pressed\n");
							} break;

							case SDLK_SPACE: 
							{
								printf("space pressed\n");
							} break;

							default: 
							{
								// TODO(nick): not valid key pressed here - just ignore?
							} break;
						}
					} break;

					case SDL_KEYUP:
					{
						switch (CurrentEvent.key.keysym.sym)
						{
							case SDLK_UP: 
							{
								printf("arrow up released\n");
							} break;

							case SDLK_DOWN:
							{
								printf("arrow down released\n");
							} break;

							case SDLK_LEFT:
							{
								printf("arrow left released\n");
							} break;

							case SDLK_RIGHT:
							{
								printf("arrow right released\n");
							} break;

							case SDLK_w: 
							{
								printf("w key released\n");
							} break;

							case SDLK_a:
							{
								printf("a key released\n");
							} break;

							case SDLK_s:
							{
								printf("s key released\n");
							} break;

							case SDLK_d:
							{
								printf("d key released\n");
							} break;

							case SDLK_SPACE: 
							{
								printf("space released\n");
							} break;

							default: 
							{
								// TODO(nick): not valid key pressed here - just ignore?
							} break;
						}
					} break;

					default:
					{
						// TODO(nick): not valid code path here ... 
						// figure out what to do .. 
					} break;
				}
			}

			// clear the screen
			SDL_RenderClear(Window->GameRenderer);
	
			// render texture(s) to screen
			SDL_RenderCopy(Window->GameRenderer, PlayerEntity->CurrentTexture->Texture,
				       NULL, &PlayerEntity->CurrentTexture->RenderBox);

			// update screen
			SDL_RenderPresent(Window->GameRenderer);
		}
	}
	else
	{
		// TODO(nick): proper logging / clean exit
		return -1;
	}

	// TODO(nick): stream line some clean up process that is going to unload assest / clean up memory
	// Destroy window
	SDL_DestroyWindow(Window->GameWindow);

	// quit SDL image
	IMG_Quit();

	// quit SDL subsystems
	SDL_Quit();

	return 0;
}

SDL_Window *
InitializeGameWindow()
{
	SDL_Window *Window = NULL;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// TODO(nick): proper logging / clean exit
		printf("ERROR - SDL could not init - SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		Window = SDL_CreateWindow("Prototype Alpha 0.1",
					  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					  Screen_Width, Screen_Height,
					  SDL_WINDOW_SHOWN);
	}

	return Window;
}

bool
InitializeAssetPipeline()
{
	int sdlImageInit = IMG_Init(Sdl_Image_Flags);

	if ((sdlImageInit & Sdl_Image_Flags) != Sdl_Image_Flags)
	{
		// TODO(nick): proper logging / clean exit
		printf("ERROR - SDL_image did not initialize properly - IMG_Error: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

WindowState *
InitializeGame()
{
	// TODO(nick): clean up of windowstate memory is needed 
	WindowState *CurrentWindowState = (WindowState *)malloc(sizeof(WindowState));
	CurrentWindowState->GameWindow = InitializeGameWindow();

	if (CurrentWindowState->GameWindow)
	{
		CurrentWindowState->GameSurface = SDL_GetWindowSurface(CurrentWindowState->GameWindow);

		// TODO(nick): toggle between software / hardware renderering
		CurrentWindowState->GameRenderer = SDL_CreateRenderer(CurrentWindowState->GameWindow, 
								      -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(CurrentWindowState->GameRenderer, 0x00, 0x00, 0x00, 0x00);

		// NOTE(nick): this might not be necessary?
		if (SDL_SetSurfaceBlendMode(CurrentWindowState->GameSurface, SDL_BLENDMODE_BLEND) == 0)
		{
			if (InitializeAssetPipeline())
			{
				// TODO(nick): need a better approach to loading game assets
				// TODO(nick): instead of single malloc - do a large malloc
				// (or lower level call and control the memory / clean up)
				
				// start loading game assets
				PlayerEntity = (Entity *)malloc(sizeof(Entity));
				// TODO(nick): add checking to make sure assets load properly - else log some failure message
				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/_0014_Idle-.png", "rb");

				// TODO(nick): split entity / texture? Current texture could be another struct
				// something like texture info?
				PlayerEntity->CurrentTexture = LoadAsset(ReadWriteOperations,
								       	 CurrentWindowState->GameSurface,
								         CurrentWindowState->GameRenderer);
			}
			else
			{
				printf("ERROR - Asset pipeline failed - SDL_ERROR: %s - SDL_Img_ERROR: %s\n",
				SDL_GetError(), IMG_GetError());
			}
		}
		else
		{
			printf("ERROR!\n");
		}
	}
	else
	{
		// TODO(nick): proper logging / clean exit
		printf("ERROR - SDL could not create window - SDL_Error: %s\n", SDL_GetError());
	}
	
	return CurrentWindowState;
}

AssetTexture *
LoadAsset (SDL_RWops *RWOperations, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer)
{
	AssetTexture *Result = NULL;
	SDL_Texture *Texture = NULL;
	SDL_Surface *Raw = IMG_LoadPNG_RW(RWOperations);

	if (!Raw)
	{
		// TODO(nick): proper logging / clean exit
		printf("ERROR - SDL_image could not load image properly - IMG_Error: %s\n", IMG_GetError());
	}
	else
	{
		Texture = SDL_CreateTextureFromSurface(GameRenderer, Raw);
		if (Texture == NULL)
		{
			// TODO(nick): debug / logging support
			printf("ERROR - SDL could not create texture - SDL_Error: %s\n", SDL_GetError());
		}

		// NOTE(nick): average heigh for asset should be 1.6 meters
		// need to figure out how to determine scaling for assets
		Result = (AssetTexture *)malloc(sizeof(AssetTexture));

		// TODO(nick): clean this up - a bit repetitive
		Result->MeterHeight = Raw->w;
		Result->MeterWidth = Raw->h;
		Result->Texture = Texture;
		Result->RenderBox =
		{
			// TODO(nick): change to non-static values
			240,
			190,
			Raw->w,
			Raw->h,
		};
		 
		SDL_FreeSurface(Raw);
	}

	return Result;
}

