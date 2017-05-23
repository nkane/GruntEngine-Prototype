/*	main.cpp
 *
 *	Created By: Nick Kane
 */

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "gameplatform.h"
#include "assets.h"
#include "entity.h"
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

global_variable WindowState *GlobalWindowState; 
global_variable GameState *GlobalGameState;
global_variable bool GameRunning = true;

// TODO(nick): this could be moved elsewhere?
global_variable SDL_RWops *ReadWriteOperations;

global_variable Entity *PlayerEntity;

// TODO(nick): clean this up and initializegame
internal SDL_Window *
InitializeGameWindow();

internal WindowState *
InitializeWindowState();

internal bool
InitializeAssetPipeline();

// TODO(nick): clean this up and initializegamewindow up
// this probably shouldn't be returning a window pointer
internal bool
InitializeGame();

internal GameState *
InitializeGameState();

internal void
ReleaseGameState(GameState *);

AssetTexture *
LoadAsset(GameState *, SDL_RWops *, SDL_Surface *, SDL_Renderer *);

// TODO(nick): need to create this function to be proper movement working
void
GameUpdateAndRender(WindowState *, GameState *, Entity *);

int
main(int argc, char *argv[])
{
	// TODO(nick): add this maybe to gamestate / windowstate?
	SDL_Event CurrentEvent;
	
	if (!InitializeGame())
	{
		// TODO(nick): logging / other stuff
		return -1;
	}

	// game initialized successfully
	while (GameRunning)
	{
		// query for time
		GlobalGameState->CurrentMS = SDL_GetTicks();
		// TODO(nick): remove - debug only
		printf("Current MS: %d\n", GlobalGameState->CurrentMS);

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

							// TODO(nick): figure out wtf I am doing with these
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
		SDL_RenderClear(GlobalWindowState->GameRenderer);

		// Update and render game
		GameUpdateAndRender(GlobalWindowState, GlobalGameState, PlayerEntity);

		// update screen
		SDL_RenderPresent(GlobalWindowState->GameRenderer);

		// TODO(nick): for debugging
		GlobalGameState->CycleEndMS = SDL_GetTicks();

		// TODO(nick: for debugging
		GlobalGameState->DeltaMS = (GlobalGameState->CycleEndMS - GlobalGameState->CurrentMS);

		if (GlobalGameState->DeltaMS < Frame_Rate_Lock) 
		{
			// TODO(nick): remove variable - debug only or keep in game state
			unsigned int delay = (Frame_Rate_Lock - GlobalGameState->DeltaMS);
			SDL_Delay(delay);
			printf("Delay: %d\n", delay);
		}

		// TODO(nick): remove - debug only
		printf("Delta MS: %d\n\n\n", GlobalGameState->DeltaMS);
	}
	

	// TODO(nick): stream line some clean up process that is going to unload assest / clean up memory
	// Destroy window
	SDL_DestroyWindow(GlobalWindowState->GameWindow);

	ReleaseGameState(GlobalGameState);

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

internal WindowState *
InitializeWindowState()
{
	// TODO(nick): move to game permanent storage?
	WindowState *CurrentWindowState = (WindowState *)malloc(sizeof(WindowState));
	return CurrentWindowState;
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

internal bool
InitializeGame()
{
	// TODO(nick): clean up of windowstate memory is needed
	GlobalWindowState = InitializeWindowState();
	GlobalWindowState->GameWindow = InitializeGameWindow();

	if (GlobalWindowState->GameWindow)
	{
		GlobalWindowState->GameSurface = SDL_GetWindowSurface(GlobalWindowState->GameWindow);

		// TODO(nick): toggle between software / hardware renderering
		GlobalWindowState->GameRenderer = SDL_CreateRenderer(GlobalWindowState->GameWindow, 
								      -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(GlobalWindowState->GameRenderer, 0x00, 0x00, 0x00, 0x00);

		// NOTE(nick): this might not be necessary?
		if (SDL_SetSurfaceBlendMode(GlobalWindowState->GameSurface, SDL_BLENDMODE_BLEND) == 0)
		{
			if (InitializeAssetPipeline())
			{
				// TODO(nick): 
				// 1) need a better approach to loading game assets
				// 2) instead of single malloc -
				//    do a large malloc (or lower level call and control the memory / clean up)
				// 3) add checking to make sure assets load properly -
				//    else log some failure message
				// 4) split entity / texture? Current texture could be another struct
				//    something like texture info?

				GlobalGameState = InitializeGameState();
				Assert(GlobalGameState);
				
				// start loading game assets
				PlayerEntity = (Entity *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
							                 sizeof(Entity));

				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/_0014_Idle-.png", "rb");
				PlayerEntity->IdleTexture = LoadAsset(GlobalGameState,
								      ReadWriteOperations,
								      GlobalWindowState->GameSurface,
								      GlobalWindowState->GameRenderer);

				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/_0013_Walk.png", "rb");
				PlayerEntity->WalkTexture = LoadAsset(GlobalGameState,
							              ReadWriteOperations,
								      GlobalWindowState->GameSurface,
								      GlobalWindowState->GameRenderer);

				// NOTE(nick): set default texture on game init
				PlayerEntity->CurrentState = (EntityState)(Idle | FaceRight);

				PlayerEntity->CurrentTexture = PlayerEntity->IdleTexture;

				// TODO(nick): 
				// 1) remove static position - figure out starting location
				
				// TODO(nick): debug this - access violation occuring
				PlayerEntity->PositionV2 = (Vector2 *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
										      sizeof(Vector2));
				//PlayerEntity->PositionV2 = (Vector2 *)malloc(sizeof(Vector2));
				PlayerEntity->PositionV2->X = 460;
				PlayerEntity->PositionV2->Y = 400;
			}
			else
			{
				printf("ERROR - Asset pipeline failed - SDL_ERROR: %s - SDL_Img_ERROR: %s\n",
				SDL_GetError(), IMG_GetError());
				return false;
			}
		}
		else
		{
			printf("ERROR!\n");
			return false;
		}
	}
	else
	{
		// TODO(nick): proper logging / clean exit
		printf("ERROR - SDL could not create window - SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

inline GameState *
InitializeGameState()
{
	GameState *CurrentGameState = (GameState *)malloc(sizeof(GameState));
	CurrentGameState->StartMS = SDL_GetTicks();
	CurrentGameState->CurrentMS = 0;
	CurrentGameState->DeltaMS = 0;

	CurrentGameState->Memory = (GameMemory *)malloc(sizeof(GameMemory));
	CurrentGameState->CurrentTexture = NULL;

	Assert(CurrentGameState->Memory);
	
	// TODO(nick): change to inline function
	CurrentGameState->Memory->PermanentStorage = (MemoryBlock *)malloc(Megabytes(64));
	CurrentGameState->Memory->PermanentStorage->Size = Megabytes(64);
	CurrentGameState->Memory->PermanentStorage->CurrentBytes = sizeof(MemoryBlock);
	CurrentGameState->Memory->PermanentStorage->Next = CurrentGameState->Memory->PermanentStorage + sizeof(MemoryBlock);

	CurrentGameState->Memory->TransientStorage = (MemoryBlock *)malloc(Megabytes(20));
	CurrentGameState->Memory->TransientStorage->Size = Megabytes(20);
	CurrentGameState->Memory->TransientStorage->Next = NULL;

	Assert(CurrentGameState->Memory->PermanentStorage);
	Assert(CurrentGameState->Memory->TransientStorage);

	return CurrentGameState;
}

internal void
ReleaseGameState(GameState *CurrentGameState)
{
	if (CurrentGameState->Memory->PermanentStorage)
	{
		free(CurrentGameState->Memory->PermanentStorage);
	}

	if (CurrentGameState->Memory->TransientStorage)
	{
		free(CurrentGameState->Memory->TransientStorage);
	}

	if (CurrentGameState->Memory)
	{
		free(CurrentGameState->Memory);
	}

	if (CurrentGameState) 
	{
		free(CurrentGameState);
	}
}

AssetTexture *
LoadAsset(GameState *CurrentGameState, SDL_RWops *RWOperations, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer)
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

		// TODO(nick): 
		// 1) average heigh for asset should be 1.6 meters
		// need to figure out how to determine scaling for assets
		Result = (AssetTexture *)PushMemoryChunk(CurrentGameState->Memory->PermanentStorage, sizeof(AssetTexture));

		Result->Width = Raw->w;
		Result->Height = Raw->h;
		Result->Rotation = 0.0f;
		Result->Flip = SDL_FLIP_NONE; 
		Result->Texture = Texture;
		 
		SDL_FreeSurface(Raw);
	}

	return Result;
}

void
GameUpdateAndRender(WindowState *CurrentWindowState, GameState *CurrentGameState, Entity *CurrentEntity)
{
	// render texture(s) to screen
	// TODO(nick):
	// 1) 1st NULL value is clip box 
	//    - srcrect 
	// 2) 2nd NULL value is center point
	//    - center, used for point to determine rotation
	
	SDL_Rect RenderBox = 
	{
		// TODO(nick): figure out a better way to handle all of this ....
		CurrentEntity->PositionV2->X,
		CurrentEntity->PositionV2->Y,
		CurrentEntity->CurrentTexture->Width,
		CurrentEntity->CurrentTexture->Height,
	};

	SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
			 CurrentEntity->CurrentTexture->Texture,
			 NULL,
			 &RenderBox,
			 CurrentEntity->CurrentTexture->Rotation,
			 NULL,
			 CurrentEntity->CurrentTexture->Flip);
}

