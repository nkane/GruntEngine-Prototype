/*
 *	Created By: Nick Kane
 */

#include <SDL.h>
#include <SDL_image.h>

#include "gameplatform.h"
#include "gamememory.h"
#include "gamestate.h"
#include "windowstate.h"

#include <stdio.h>
#include <stdlib.h>

#define local_persist 	static
#define internal 	static
#define global_variable static

#define Assert(Expression) if(!(Expression)) {*(int *)0=0;}

global_variable const int Screen_Width = 640;
global_variable const int Screen_Height = 480;
global_variable const int Sdl_Image_Flags = IMG_INIT_PNG;

global_variable const float Frame_Rate_Lock = (1000.0f / 60.0f);

// TODO(nick): rename this ...
global_variable WindowState *Window; 
global_variable GameState *Game;
global_variable bool GameRunning = true;

// TODO(nick): this could be moved elsewhere?
global_variable SDL_RWops *ReadWriteOperations;

// TODO(nick): bit mask may not be needed, think about it some more ...
enum EntityState
{
	Idle      = (1u << 0),
	FaceLeft  = (1u << 1),
	FaceRight = (1u << 2),
};

struct Vector2
{
	int X;
	int Y;
};

// TODO(nick): move structs in to a .h file
struct AssetTexture
{
	double Rotation;
	SDL_RendererFlip Flip;
	SDL_Texture *Texture;
	int Width;
	int Height;
};

struct Entity
{
	AssetTexture *CurrentTexture;
	AssetTexture *IdleTexture;
	AssetTexture *WalkTexture;
	EntityState CurrentState;
	Vector2 *PositionV2;
};

global_variable Entity *PlayerEntity;

// TODO(nick): clean this up and initializegame
inline SDL_Window *
InitializeGameWindow();

inline bool
InitializeAssetPipeline();

// TODO(nick): clean this up and initializegamewindow up
internal WindowState *
InitializeGame();

inline GameState *
InitializeGameState();

AssetTexture *
LoadAsset(SDL_RWops *, SDL_Surface *, SDL_Renderer *);

// TODO(nick): need to create this function to be proper movement
// working
void
GameUpdateAndRender(WindowState *, Entity *);

int
main(int argc, char *argv[])
{
	// TODO(nick): add this maybe to gamestate / windowstate?
	SDL_Event CurrentEvent;

	Window = InitializeGame();
	Game = InitializeGameState();

	if(Window->GameWindow != NULL)
	{
		// game initialized successfully
		while (GameRunning)
		{
			// query for time
			Game->CurrentMS = SDL_GetTicks();
			// TODO(nick): remove - debug only
			printf("Current MS: %d\n", Game->CurrentMS);

			while (SDL_PollEvent(&CurrentEvent))
			{
				// TODO(nick): handle input function
				switch (CurrentEvent.type)
				{
					case SDL_QUIT:
					{
						GameRunning = false;
					} break;

					// TODO(nick): figure out a better way to handle up / release
					// key presses
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
								// TODO(nick):
								// 1) flip texture before 
								// 2) set a flag for state of entity facing direction?
								if (PlayerEntity->CurrentState & (FaceRight))
								{
									PlayerEntity->IdleTexture->Flip = SDL_FLIP_HORIZONTAL;
									PlayerEntity->WalkTexture->Flip = SDL_FLIP_HORIZONTAL;
									PlayerEntity->CurrentState = FaceLeft;
								}


								PlayerEntity->CurrentTexture = PlayerEntity->WalkTexture;
								// TODO(nick): possible change to velocity?
								PlayerEntity->PositionV2->X -= 5;

								printf("arrow left pressed\n");
							} break;

							case SDLK_RIGHT:
							{
								// NOTE(nick): current state is left
								if (PlayerEntity->CurrentState & (FaceLeft))
								{
									PlayerEntity->IdleTexture->Flip = SDL_FLIP_NONE;
									PlayerEntity->WalkTexture->Flip = SDL_FLIP_NONE;
									PlayerEntity->CurrentState = FaceRight;
								}

								PlayerEntity->CurrentTexture = PlayerEntity->WalkTexture;

								PlayerEntity->PositionV2->X += 5;

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
								PlayerEntity->CurrentState = (EntityState)(FaceLeft | Idle);
								PlayerEntity->CurrentTexture = PlayerEntity->IdleTexture;
								printf("arrow left released\n");
							} break;

							case SDLK_RIGHT:
							{
								PlayerEntity->CurrentState = (EntityState)(FaceRight | Idle);
								PlayerEntity->CurrentTexture = PlayerEntity->IdleTexture;
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
	
			// Update and render game
			GameUpdateAndRender(Window, PlayerEntity);

			// update screen
			SDL_RenderPresent(Window->GameRenderer);

			// TODO(nick): for debugging
			Game->CycleEndMS = SDL_GetTicks();

			// TODO(nick: for debugging
			Game->DeltaMS = (Game->CycleEndMS - Game->CurrentMS);

			if (Game->DeltaMS < Frame_Rate_Lock) 
			{
				// TODO(nick): remove variable - debug only or keep in game state
				unsigned int delay = (Frame_Rate_Lock - Game->DeltaMS);
				SDL_Delay(delay);
				printf("Delay: %d\n", delay);
			}

			// TODO(nick): remove - debug only
			printf("Delta MS: %d\n\n\n", Game->DeltaMS);
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

inline SDL_Window *
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

inline bool
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

internal WindowState *
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
				// TODO(nick): 
				// 1) need a better approach to loading game assets
				// 2) instead of single malloc - do a large malloc (or lower level call and control the memory / clean up)
				// 3) add checking to make sure assets load properly - else log some failure message
				// 4) split entity / texture? Current texture could be another struct something like texture info?

				// start loading game assets
				PlayerEntity = (Entity *)malloc(sizeof(Entity));
				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/_0014_Idle-.png", "rb");
				PlayerEntity->IdleTexture = LoadAsset(ReadWriteOperations,
								      CurrentWindowState->GameSurface,
								      CurrentWindowState->GameRenderer);

				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/_0013_Walk.png", "rb");
				PlayerEntity->WalkTexture = LoadAsset(ReadWriteOperations,
								     CurrentWindowState->GameSurface,
								     CurrentWindowState->GameRenderer);

				// NOTE(nick): set default texture on game init
				PlayerEntity->CurrentState = (EntityState)(Idle | FaceRight);
				PlayerEntity->CurrentTexture = PlayerEntity->IdleTexture;

				// TODO(nick): remove static position - figure out starting location
				PlayerEntity->PositionV2 = (Vector2 *)malloc(sizeof(Vector2));
				PlayerEntity->PositionV2->X = 460;
				PlayerEntity->PositionV2->Y = 400;
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

inline GameState *
InitializeGameState()
{
	GameState *CurrentGameState = (GameState *)malloc(sizeof(GameState));
	CurrentGameState->StartMS = SDL_GetTicks();
	CurrentGameState->CurrentMS = 0;
	CurrentGameState->DeltaMS = 0;
	return CurrentGameState;
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

		// TODO(nick): average heigh for asset should be 1.6 meters
		// need to figure out how to determine scaling for assets
		Result = (AssetTexture *)malloc(sizeof(AssetTexture));

		Result->Width = Raw->w;
		Result->Height = Raw->h;

		Result->Texture = Texture;
		 
		SDL_FreeSurface(Raw);
	}

	Result->Rotation = 0.0;
	Result->Flip = SDL_FLIP_NONE; 
	
	return Result;
}

void
GameUpdateAndRender(WindowState *Window, Entity *CurrentEntity)
{
	// render texture(s) to screen
	// TODO(nick):
	// 1) 1st NULL value is clip box 
	//    - srcrect 
	// 2) 2nd NULL value is center point
	//    - center, used for point to determine rotation
	
	SDL_Rect RenderBox = 
	{
		CurrentEntity->PositionV2->X,
		CurrentEntity->PositionV2->Y,
		CurrentEntity->CurrentTexture->Width,
		CurrentEntity->CurrentTexture->Height,
	};

	SDL_RenderCopyEx(Window->GameRenderer,
			 CurrentEntity->CurrentTexture->Texture,
			 NULL,
			 &RenderBox,
			 CurrentEntity->CurrentTexture->Rotation,
			 NULL,
			 CurrentEntity->CurrentTexture->Flip);
}

