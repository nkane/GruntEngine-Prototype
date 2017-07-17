/*	main.cpp
 *
 *	Created By: Nick Kane
 */

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "gameplatform.h"
#include "assets.h"
#include "text.h"
#include "hashset.h"
#include "entity.h"
#include "list.h"
#include "queue.h"
#include "gamestate.h"
#include "windowstate.h"

#include <stdio.h>
#include <stdlib.h>

#define local_persist 	static
#define internal 	static
#define global_variable static

// Entity Game Constants
// <=====================================================================>
global_variable const int Screen_Width = 640;
global_variable const int Screen_Height = 480;
global_variable const int Sdl_Image_Flags = IMG_INIT_PNG;
global_variable const float Frame_Rate_Lock = (1000.0f / 60.0f);
// /=====================================================================/

// Entity Game Window and State
// <=====================================================================>
global_variable WindowState *GlobalWindowState; 
global_variable GameState *GlobalGameState;
global_variable bool GameRunning = true;
global_variable SDL_RWops *ReadWriteOperations;
// /=====================================================================/

// Entity Globals
// <=====================================================================>
global_variable Entity *PlayerEntity;
global_variable Entity *GronkEntity;
global_variable int GlobalEntityArrayIndex = 0;
global_variable Entity *GlobalEntityArray[50];
global_variable Queue_GameEntity *GlobalEntityRenderQueue;
// /=====================================================================/

// Font Globals
// <=====================================================================>
global_variable TTF_Font *ArcadeFont;
global_variable TTF_Font *PokeFont;
global_variable int GlobalTextArrayIndex = 0;
global_variable Text *GlobalTextArray[50];

global_variable Text *TitleScreenGronkeyKong;
global_variable Text *TitleScreenBottom;

global_variable Text *LivesCount;
global_variable Text *CurrentScore;
global_variable Text *HighScore;
global_variable Text *CurrentLevel;

// TODO(nick): complete this queue
global_variable Queue_GameText *GlobalTextRenderQueue;
// /=====================================================================/

internal SDL_Window *
InitializeGameWindow();

internal WindowState *
InitializeWindowState();

internal bool
InitializeAssetPipeline();

internal bool
InitializeGame();

internal GameState *
InitializeGameState();

internal void
ReleaseGameState(GameState *);

AssetTexture *
LoadAssetPNG(GameState *CurrentGameState, SDL_RWops *RWOperations, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer);

AssetTexture *
LoadAssetTTF(GameState *CurrentGameState, TTF_Font *Font, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer, char *text);

void 
GameUpdateAndRender(WindowState *CurrentWindowState, GameState *CurrentGameState, Queue_GameEntity *EntityQueue, Queue_GameText *TextQueue);

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

		// TODO(nick): make enter key flip
		if (GlobalGameState->IsPlaying)
		{
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
								// 1) rework with new hash set
								// 2) flip texture before 
								// 3) set a flag for state of entity facing direction?
								if (PlayerEntity->CurrentState & (FaceRight))
								{
									HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle")->Flip = SDL_FLIP_HORIZONTAL;
									HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk")->Flip = SDL_FLIP_HORIZONTAL;
									PlayerEntity->CurrentState = FaceLeft;
								}

								PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk");
								
								// TODO(nick): possible change to velocity?
								PlayerEntity->PositionV2.X -= 5;
								printf("arrow left pressed\n");
							} break;

							case SDLK_RIGHT:
							{
								// TODO(nick): 
								// 1) rework with new hash set
								// NOTE(nick): current state is left
								if (PlayerEntity->CurrentState & (FaceLeft))
								{
									HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle")->Flip = SDL_FLIP_NONE;
									HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk")->Flip = SDL_FLIP_NONE;
									PlayerEntity->CurrentState = FaceRight;
								}
								
								PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk");
								// TODO(nick): possible change to velocity?
								PlayerEntity->PositionV2.X += 5;
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
								PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle");
								printf("arrow left released\n");
							} break;

							case SDLK_RIGHT:
							{
								PlayerEntity->CurrentState = (EntityState)(FaceRight | Idle);
								PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle");
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
				// clear the screen
				SDL_RenderClear(GlobalWindowState->GameRenderer);

				// TODO(nick): make nodes only for needed stuff (for queue - entity and text)
				//
				// TODO(nick): add some logical step that takes place per level
				// and builds an array of entity nodes?
				// for now static ones work
				Entity_Node PlayerEntityNode = 
				{
					PlayerEntity,
					NULL,
				};

				Entity_Node GruntEntityNode = 
				{
					GronkEntity,
					NULL,
				};

				// NOTE(nick): add entities to render queue
				Queue_Enqueue_GameEntity(GlobalEntityRenderQueue, PlayerEntityNode);
				Queue_Enqueue_GameEntity(GlobalEntityRenderQueue, GruntEntityNode);

				// TODO(nick): make nodes only for needed stuff
				/*
				Text_Node MainTextNode =
				{
					Gronkey,
					NULL,
				};

				Queue_Enqueue_GameText(GlobalTextRenderQueue, MainTextNode);
				*/
				
				GameUpdateAndRender(GlobalWindowState, GlobalGameState, GlobalEntityRenderQueue, GlobalTextRenderQueue);
			}
		}
		else
		{
			// NOTE(nick): player is at title screen
			// TODO(nick):
			// 1) only draw title screen
			// 2) only handle UI commands
			// clear the screen
			SDL_RenderClear(GlobalWindowState->GameRenderer);

			Text_Node GronkeyKong = 
			{
				TitleScreenGronkeyKong,
				NULL,
			};

			Queue_Enqueue_GameText(GlobalTextRenderQueue, GronkeyKong);

			GameUpdateAndRender(GlobalWindowState, GlobalGameState, GlobalEntityRenderQueue, GlobalTextRenderQueue);
		}

		// update screen
		SDL_RenderPresent(GlobalWindowState->GameRenderer);

		// TODO(nick): for debugging
		GlobalGameState->CycleEndMS = SDL_GetTicks();

		// TODO(nick): for debugging
		GlobalGameState->DeltaMS = (GlobalGameState->CycleEndMS - GlobalGameState->CurrentMS);

		if (GlobalGameState->DeltaMS < Frame_Rate_Lock) 
		{
			// TODO(nick): remove variable - debug only or keep in game state
			unsigned int delay = (Frame_Rate_Lock - GlobalGameState->DeltaMS);
			SDL_Delay(delay);
			printf("Delay: %d\n", delay);
		}
	}

	// destory textures
	{	
		SDL_DestroyTexture(HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle")->Texture);
		SDL_DestroyTexture(HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk")->Texture);
		SDL_DestroyTexture(HashSet_Select_AssetTexture(GronkEntity->TextureSet, "Gronk-Idle")->Texture);
	}

	// release fonts
	{
		TTF_CloseFont(ArcadeFont);
		TTF_CloseFont(PokeFont);
	}

	// TODO(nick): stream line some clean up process that is going to unload assest / clean up memory
	// destory renderer
	SDL_DestroyRenderer(GlobalWindowState->GameRenderer);

	// destroy window
	SDL_DestroyWindow(GlobalWindowState->GameWindow);

	// release game state memory blocks
	ReleaseGameState(GlobalGameState);

	// quit SDL ttf
	TTF_Quit();
	
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
		// TODO(nick): write out sizes to config file
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

	if (TTF_Init() == -1)
	{
		printf("Error - SDL_ttf did not initialize properly - SDL_ttf Error: %s\n", TTF_GetError());
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
		SDL_GetWindowSize(GlobalWindowState->GameWindow, &GlobalWindowState->Width, &GlobalWindowState->Height);

		// TODO(nick): toggle between software / hardware renderering
		GlobalWindowState->GameRenderer = SDL_CreateRenderer(GlobalWindowState->GameWindow, 
								      -1, SDL_RENDERER_ACCELERATED);

		// NOTE(nick); sets clear to black
		SDL_SetRenderDrawColor(GlobalWindowState->GameRenderer, 0x00, 0x00, 0x00, 0x00);

		// NOTE(nick): this might not be necessary?
		if (SDL_SetSurfaceBlendMode(GlobalWindowState->GameSurface, SDL_BLENDMODE_BLEND) == 0)
		{
			if (InitializeAssetPipeline())
			{
				// TODO(nick): 
				// 1) need a better approach to loading game assets
				// 2) add checking to make sure assets load properly -
				//    else log some failure message
				// 3) split entity / texture? Current texture could be another struct
				//    something like texture info?

				GlobalGameState = InitializeGameState();
				Assert(GlobalGameState);

				// TODO(nick): 
				// 1) complete hash insert funciton
				unsigned int HashKey = 0;
				// NOTE(nick): player intitialization
				PlayerEntity = (Entity *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
							                 sizeof(Entity));

				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/Grunt-Idle.png", "rb");
				HashSet_Insert_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle", LoadAssetPNG(GlobalGameState, ReadWriteOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));

				ReadWriteOperations = SDL_RWFromFile("./assets/Grunt/Grunt-Walk-1.png", "rb");
				HashSet_Insert_AssetTexture(PlayerEntity->TextureSet, "Grunt-Walk", LoadAssetPNG(GlobalGameState, ReadWriteOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));

				// NOTE(nick): set default texture on game init
				PlayerEntity->CurrentState = (EntityState)(Idle | FaceRight);
				PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(PlayerEntity->TextureSet, "Grunt-Idle");

				PlayerEntity->PositionV2 = DefaultVector2Position();

				GlobalEntityArray[GlobalEntityArrayIndex] = PlayerEntity;
				++GlobalEntityArrayIndex;

				// NOTE(nick): gronk initialization
				GronkEntity = (Entity *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
									sizeof(Entity));

				ReadWriteOperations = SDL_RWFromFile("./assets/Gronk/Gronk_0011_Gronk-Idle-2.png", "rb");
				HashSet_Insert_AssetTexture(GronkEntity->TextureSet, "Gronk-Idle", LoadAssetPNG(GlobalGameState, ReadWriteOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));
				GronkEntity->CurrentState = (EntityState)(Idle);
				GronkEntity->CurrentTexture = HashSet_Select_AssetTexture(GronkEntity->TextureSet, "Gronk-Idle");
				GronkEntity->PositionV2 = DefaultVector2Position();

				GlobalEntityArray[GlobalEntityArrayIndex] = GronkEntity;
				++GlobalEntityArrayIndex;

				// NOTE(nick): game font initialization
				ArcadeFont = TTF_OpenFont("./assets/Fonts/arcade_classic/ARCADECLASSIC.TTF", 24);
				if (!ArcadeFont) 
				{
					printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
				}

				PokeFont = TTF_OpenFont("./assets/Fonts/poke_font/POKE.FON", 24);
				if (!PokeFont)
				{
					printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
				}

				TitleScreenGronkeyKong = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
										 sizeof(Text));
				TitleScreenGronkeyKong->Texture = LoadAssetTTF(GlobalGameState,
							                       ArcadeFont,
							      	      	       GlobalWindowState->GameSurface,
						              	               GlobalWindowState->GameRenderer, 
								               "GRONKEY\n\tKONG\0");
				/*
				GameText = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
								   sizeof(Text));

				ArcadeFont = TTF_OpenFont("./assets/Fonts/arcade_classic/ARCADECLASSIC.TTF", 24);
				if (!ArcadeFont)
				{
					printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
				}
				GameText->Text = LoadAssetTTF(GlobalGameState,
								     ArcadeFont,
								     GlobalWindowState->GameSurface,
								     GlobalWindowState->GameRenderer);
				GameText->PrimaryPositionV2 = DefaultVector2Position();
				
				GameText->SecondaryFont = TTF_OpenFont("./assets/Fonts/poke_font/POKE.FON", 24);
				if (GameText->SecondaryFont)
				{
					printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
				}
				GameText->SecondaryText = LoadAssetTTF(GlobalGameState,
								       GameText->SecondaryFont,
								       GlobalWindowState->GameSurface,
								       GlobalWindowState->GameRenderer);


				GameText->SecondaryPositionV2 = DefaultVector2Position();
				*/

				// NOTE(nick): allocate enough space for the game queue data
				// as well as 50 queue slots
				GlobalEntityRenderQueue = (Queue_GameEntity *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
											      (sizeof(Queue_GameEntity) + (sizeof(Entity_Node) * 50)));
				GlobalEntityRenderQueue->Size = 0;
				GlobalEntityRenderQueue->Limit = 50;

				GlobalTextRenderQueue = (Queue_GameText *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
											  (sizeof(Queue_GameText) + (sizeof(Text_Node))));
				GlobalTextRenderQueue->Size = 0;
				GlobalTextRenderQueue->Limit = 50;
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
	// TODO(nick): reset this to false !!!
	CurrentGameState->IsPlaying = false;

	CurrentGameState->Memory = (GameMemory *)malloc(sizeof(GameMemory));

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
LoadAssetPNG(GameState *CurrentGameState, SDL_RWops *RWOperations, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer)
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
		// TODO(nick): a free texture might need to be called on this?
		Texture = SDL_CreateTextureFromSurface(GameRenderer, Raw);
		if (!Texture)
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

// TODO(nick): 
// 1) pass in color?
AssetTexture *
LoadAssetTTF(GameState *CurrentGameState, TTF_Font *Font, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer, char *text)
{
	AssetTexture *Result = NULL;
	SDL_Texture *Texture = NULL;
	
	// TODO(nick): make colors.h file
	SDL_Color DefaultColor = { 255, 255, 255, 0 };
	SDL_Surface *Raw = TTF_RenderText_Solid(Font, text, DefaultColor);

	if (!Raw)
	{
		printf("ERROR - SDL_ttf could not load text properly - SDL_ttf: %s\n", TTF_GetError());
	}
	else 
	{
		// TODO(nick): a free texture might need to be called on this?
		Texture = SDL_CreateTextureFromSurface(GameRenderer, Raw);
		if (!Texture)
		{
			printf("ERROR - SDL could not create texture - SDL_Error: %s\n", SDL_GetError());
		}

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
GameUpdateAndRender(WindowState *CurrentWindowState, GameState *CurrentGameState, Queue_GameEntity *EntityQueue, Queue_GameText *TextQueue)
{
	// render texture(s) to screen
	// TODO(nick):
	// 1) 1st NULL value is clip box 
	//    - srcrect 
	// 2) 2nd NULL value is center point
	//    - center, used for point to determine rotation
	Entity *CurrentEntity = NULL; 
	while (CurrentEntity = Queue_Dequeue_GameEntity(EntityQueue))
	{
		SDL_Rect PlayerRenderBox = 
		{
			CurrentEntity->PositionV2.X,
			CurrentEntity->PositionV2.Y,
			CurrentEntity->CurrentTexture->Width,
			CurrentEntity->CurrentTexture->Height,
		};

		SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
				 CurrentEntity->CurrentTexture->Texture,
				 NULL,
				 &PlayerRenderBox,
				 CurrentEntity->CurrentTexture->Rotation,
				 NULL,
				 CurrentEntity->CurrentTexture->Flip);
	}

	Text *CurrentText = NULL;
	while (CurrentText = Queue_Dequeue_GameText(TextQueue))
	{
		SDL_Rect TextRenderBox = 
		{
			CurrentText->PositionV2.X + 100,
			CurrentText->PositionV2.Y,
			CurrentText->Texture->Width,
			CurrentText->Texture->Height,
		};

		SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
				 CurrentText->Texture->Texture,
				 NULL,
				 &TextRenderBox,
				 CurrentText->Texture->Rotation,
				 NULL,
				 CurrentText->Texture->Flip);
	}
}

