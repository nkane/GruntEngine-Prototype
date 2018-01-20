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
#include "strings.h"
#include "entity.h"
#include "list.h"
#include "queue.h"
#include "gamestate.h"
#include "windowstate.h"
#include "shapes.h"
#include "level.h"
#include "animation.h"

#include <stdio.h>
#include <stdlib.h>

#define local_persist   static
#define internal 	    static
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

//// Level Globals
// <=====================================================================>
//
global_variable const int Tile_Height = 16;
global_variable const int Tile_Width = 12;
global_variable bool LoadNextLevel = false;
// TODO(nick): check hashset.h, not sure what this is 256?
global_variable HashSet_AssetTexture GlobalLevelTextures[256];
global_variable Level* GlobalLevelArray[4];
global_variable Level* GlobalCurrentLoadedLevel;
// /=====================================================================/

// Entity Globals
// <=====================================================================>
global_variable HashSet_AssetTexture GlobalEntityTextureSet[128];
global_variable Entity *GlobalEntityArray[50];
global_variable Queue_GameEntity *GlobalEntityRenderQueue;
// player
global_variable Entity *PlayerEntity;
global_variable Animation *PlayerAnimations[10];
// enemies
global_variable Entity *GretelEntity;
global_variable int GlobalEntityArrayIndex = 0;
// /=====================================================================/

// Font Globals
// <=====================================================================>
global_variable TTF_Font *ArcadeFont_Large;
global_variable TTF_Font *ArcadeFont_Medium;
global_variable TTF_Font *ArcadeFont_Small;
global_variable TTF_Font *PokeFont_Large;
global_variable TTF_Font *PokeFont_Medium;
global_variable TTF_Font *PokeFont_Small;
global_variable int GlobalTextArrayIndex = 0;
global_variable Text *GlobalTextArray[50];

// Debug Info
// TODO(nick): add the following:
// 1) memory usage
// 2) sleep estimate
// 3) fps estimate
global_variable char *DEBUG_StringBuffer;
global_variable Text *DEBUG_PlayerPositionInfo;
global_variable Text *DEBUG_EnemyPositionInfo;

// HUD
// TODO(nick): look in to creating a different view port for hud port?
global_variable Text *HUDHighScore;
global_variable Text *HUDHighScoreValue;
global_variable Text *HUDLivesCount;
global_variable Text *HUDCurrentScore;
global_variable Text *HUDCurrentLevel;

// TODO(nick): segment into scene text?
// Title Screen
global_variable Text *TitleScreenGretelKong_1;
global_variable Text *TitleScreenGretelKong_2;
global_variable Text *TitleScreenBottom;

global_variable Text *LivesCount;
global_variable Text *CurrentScore;
global_variable Text *HighScore;
global_variable Text *CurrentLevel;

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
ReleaseGameState(GameState *CurrentGameState);

AssetTexture *
LoadAssetPNG(GameState *CurrentGameState, SDL_RWops *RWOperations, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer);

AssetTexture *
LoadAssetTTF(GameState *CurrentGameState, TTF_Font *Font, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer, char *text, SDL_Color *Color);

void
UpdateAssetTTF(SDL_Renderer *GameRenderer, Text *CurrentTextAsset, char *text, SDL_Color *Color);

// TODO(nick):
// 1) have this return a new allocated level?
// 2) global level array
// 3) "cache" already loaded levels
Level *
LoadLevel(GameState *CurrentGameState, SDL_RWops *RWOperations, char *fileName);

bool
CheckCollision(Entity *EntityArray[50], Level *CurrentLevel, int checkIndex);

void 
GameUpdateAndRender(WindowState *CurrentWindowState, GameState *CurrentGameState, Queue_GameEntity *EntityQueue, Queue_GameText *TextQueue, Level *CurrentLoadedLevel);

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
        
        while (SDL_PollEvent(&CurrentEvent))
        {
            // TODO(nick):
            // 1) pull this out to state handling, input handling, and frame selection function
            // 2) player input for movement shouldn't happen on title screen
            // 3) better handle input function
            // 4) have handle collision check here
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
                        case SDLK_RETURN:
                        {
                            if (!GlobalGameState->IsPlaying)
                            {
                                GlobalGameState->IsPlaying = true;
                                LoadNextLevel = true;
                            }
                        } break;
                                
                        case SDLK_w: 
                        case SDLK_UP: 
                        {
                            PlayerEntity->PositionV2.Y -= 2;
				            PlayerEntity->CollisionBox.y -= 2;
                            if (CheckCollision(GlobalEntityArray, GlobalCurrentLoadedLevel, PlayerEntity->Id))
                            {
                                PlayerEntity->PositionV2.Y += 2;
                                PlayerEntity->CollisionBox.y += 2;
                            }
                        } break;
                        
                        case SDLK_s:
                        case SDLK_DOWN:
                        {
                            PlayerEntity->PositionV2.Y += 2;
                            PlayerEntity->CollisionBox.y += 2;
                            if (CheckCollision(GlobalEntityArray, GlobalCurrentLoadedLevel, PlayerEntity->Id))
                            {
                                PlayerEntity->PositionV2.Y -= 2;
                                PlayerEntity->CollisionBox.y -= 2;
                            }
                        } break;
                        
                        case SDLK_a:
                        case SDLK_LEFT:
			            {
                           PlayerEntity->CurrentState = (EntityState)(Walking);
                           if (PlayerEntity->CurrentFaceDirection & (FaceRight))
                           {
                               FlipAnimations(PlayerAnimations, LeftFlip);
                               PlayerEntity->CurrentFaceDirection = (EntityFaceDirection)(FaceLeft);
                           }
                           PlayerEntity->CurrentTexture = SelectPlayerAnimationFrame(PlayerEntity, PlayerAnimations);

                           // TODO(nick):
                           // 1) change to velocity? real vector math!
                           PlayerEntity->PositionV2.X -= 2;
                           PlayerEntity->CollisionBox.x -= 2;
                           if (CheckCollision(GlobalEntityArray, GlobalCurrentLoadedLevel, PlayerEntity->Id))
                           {
                               PlayerEntity->PositionV2.X += 2;
                               PlayerEntity->CollisionBox.x += 2;
                           }
                       } break;
                                   
                       case SDLK_d:
                       case SDLK_RIGHT:
                       {
                           PlayerEntity->CurrentState = (EntityState)(Walking);
                           if (PlayerEntity->CurrentFaceDirection & (FaceLeft))
                           {
                               FlipAnimations(PlayerAnimations, RightFlip);
                               PlayerEntity->CurrentFaceDirection = (EntityFaceDirection)(FaceRight);
                           }
                           PlayerEntity->CurrentTexture = SelectPlayerAnimationFrame(PlayerEntity, PlayerAnimations);

                           // TODO(nick): 
                           // 1) change to velocity? real vector math!
                           PlayerEntity->PositionV2.X += 2;
                           PlayerEntity->CollisionBox.x += 2;
                           if (CheckCollision(GlobalEntityArray, GlobalCurrentLoadedLevel, PlayerEntity->Id))
                           {
                               PlayerEntity->PositionV2.X -= 2;
                               PlayerEntity->CollisionBox.x -= 2;
                           }
                        } break;
                        
                        case SDLK_SPACE: 
                        {
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
                        case SDLK_w:
                        case SDLK_UP: 
                        {
                        } break;
                        
                        case SDLK_s:
                        case SDLK_DOWN:
                        {
                        } break;
                        
                        case SDLK_a:
                        case SDLK_LEFT:
                        {
                           PlayerEntity->CurrentState = (EntityState)(Idle);
                           PlayerEntity->CurrentTexture = SelectPlayerAnimationFrame(PlayerEntity, PlayerAnimations);
                        } break;
                        
                        case SDLK_d:
                        case SDLK_RIGHT:
                        {
                           PlayerEntity->CurrentState = (EntityState)(Idle);
                           PlayerEntity->CurrentTexture = SelectPlayerAnimationFrame(PlayerEntity, PlayerAnimations);
                        } break;
                
                        case SDLK_SPACE: 
                        {
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
            
            // NOTE(nick): top HUD is always displayed
            // always enqueue HUD text values
            {
                Text_Node HUDHighScoreNode
                {
                    HUDHighScore,
                    NULL,
                };
                
                Text_Node HUDHighScoreValueNode
                {
                    HUDHighScoreValue,
                    NULL,
                };
                
                Text_Node HUDLivesCountNode
                {
                    HUDLivesCount,
                    NULL,
                };
                
                Text_Node HUDCurrentScoreNode
                {
                    HUDCurrentScore,
                    NULL,
                };
                
                Text_Node HUDCurrentLevelNode
                {
                    HUDCurrentLevel,
                    NULL,
                };
                
                Queue_Enqueue_GameText(GlobalTextRenderQueue, HUDHighScoreNode);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, HUDHighScoreValueNode);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, HUDLivesCountNode);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, HUDCurrentScoreNode);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, HUDCurrentLevelNode);
                
                // TODO(nick): set a debug mode flag check here
                Text_Node DEBUGInfo_PlayerPositionNode
                {
                    DEBUG_PlayerPositionInfo,
                    NULL,
                    
                };
                
                Text_Node DEBUGInfo_EnemyPositionNode
                {
                    DEBUG_EnemyPositionInfo,
                    NULL,
                };
                
                Queue_Enqueue_GameText(GlobalTextRenderQueue, DEBUGInfo_PlayerPositionNode);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, DEBUGInfo_EnemyPositionNode);
            }
            
            if (GlobalGameState->IsPlaying)
            {
                // TODO(nick) IMPORTANT(nick):
                // 1) finish initial loading up ...
                // 2) create an array that will have levels loaded
                // 3) check array first before attempting to load level
                if (LoadNextLevel)
                {
                    if (GlobalLevelArray[0] == NULL)
                    {
                        GlobalLevelArray[0] = LoadLevel(GlobalGameState, ReadWriteOperations, "./data/level_one.gdat");
                        GlobalCurrentLoadedLevel = GlobalLevelArray[0];
                    }
                    LoadNextLevel = false;
                    // TODO(nick):
                    // 1) switch current level to next level
                }
                
                // TODO(nick):
                // 1) make nodes only for needed stuff (for queue - entity and text)
                // 2) add some logical step that takes place per level
                //    and builds an array of entity nodes?
                //    for now static ones work
                Entity_Node PlayerEntityNode = 
                {
                    PlayerEntity,
                    NULL,
                };
                
                GretelEntity->PositionV2 = DefaultVector2CenterScreen(GlobalWindowState->Width, GlobalWindowState->Height);
                Entity_Node GretelEntityNode = 
                {
                    GretelEntity,
                    NULL,
                };
                
                // NOTE(nick): add entities to render queue
                Queue_Enqueue_GameEntity(GlobalEntityRenderQueue, PlayerEntityNode);
                Queue_Enqueue_GameEntity(GlobalEntityRenderQueue, GretelEntityNode);
                
                GameUpdateAndRender(GlobalWindowState, GlobalGameState, GlobalEntityRenderQueue, GlobalTextRenderQueue, GlobalCurrentLoadedLevel);
            }
            else
            {
                // NOTE(nick): player is at title screen
                // TODO(nick):
                // 1) only draw title screen
                // 2) only handle UI commands
                SDL_RenderClear(GlobalWindowState->GameRenderer);
                
                Entity_Node GretelEntityNode = 
                {
                    GretelEntity,
                    NULL,
                };
                
                Queue_Enqueue_GameEntity(GlobalEntityRenderQueue, GretelEntityNode);
                
                Text_Node GretelKong_P1 = 
                {
                    TitleScreenGretelKong_1,
                    NULL,
                };
                
                Text_Node GretelKong_P2 =
                {
                    TitleScreenGretelKong_2,
                    NULL,
                };
                
                Queue_Enqueue_GameText(GlobalTextRenderQueue, GretelKong_P1);
                Queue_Enqueue_GameText(GlobalTextRenderQueue, GretelKong_P2);
                GameUpdateAndRender(GlobalWindowState, GlobalGameState, GlobalEntityRenderQueue, GlobalTextRenderQueue, GlobalCurrentLoadedLevel);
            }
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
            //printf("Delay: %d\n", delay);
        }
    }
    
    // IMPORTANT(nick): clean up definitely needs to be handled ASAP!
    // destory textures
    {	
        SDL_DestroyTexture(HashSet_Select_AssetTexture(GlobalEntityTextureSet, "Grunt-Idle")->Texture);
        SDL_DestroyTexture(HashSet_Select_AssetTexture(GlobalEntityTextureSet, "Grunt-Walk-1")->Texture);
        SDL_DestroyTexture(HashSet_Select_AssetTexture(GlobalEntityTextureSet, "Gretel-Idle")->Texture);
    }
    
    // release fonts
    {
        TTF_CloseFont(ArcadeFont_Large);
        TTF_CloseFont(ArcadeFont_Medium);
        TTF_CloseFont(ArcadeFont_Small);
        TTF_CloseFont(PokeFont_Large);
        TTF_CloseFont(PokeFont_Medium);
        TTF_CloseFont(PokeFont_Small);
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
                
                // TODO(nick) - IMPORTANT!
                // 1) Time to change the hash code to actually handle collision, and change the type to pointers instead of structs!
                // TODO(nick):
                // 1) need a better way to load assets
                // 2) possibly have some type of file encoding that relates the entity to the loaded image?
                // 3) consider just having a global hashset that stores all game textures instead
                //    and having a list of key (texture name) - value (global game texture hashset index or id)
                char *playerTextureList[32][2] =
                {
                    { "Grunt-Idle" 		, "./assets/Grunt/Grunt-Idle.png" },
                    { "Grunt-SS-0"  		, "./assets/Grunt/Grunt-SS.png" },
                    { "Grunt-SS-Empty-Hand"	, "./assets/Grunt/Grunt-SS-Empty-Hand.png" },
                    { "Grunt-Walk-1" 		, "./assets/Grunt/Grunt-Walk-1.png" },
                    { "Grunt-Walk-2" 		, "./assets/Grunt/Grunt-Walk-2.png" },
                    { "Grunt-SS-Walk-1" 	, "./assets/Grunt/Grunt-SS-Walk-1.png" },
                    { "Grunt-SS-Walk-2" 	, "./assets/Grunt/Grunt-SS-Walk-2.png" },
                    { "Grunt-SS-Walk-3" 	, "./assets/Grunt/Grunt-SS-Walk-3.png" },
                    { "Grunt-Jump-1" 		, "./assets/Grunt/Grunt-Jump-1.png" },
                    { "Grunt-Jump-2" 		, "./assets/Grunt/Grunt-Jump-2.png" },
                    { "Grunt-Climb-1" 		, "./assets/Grunt/Grunt-Climb-1.png" },
                    { "Grunt-Climb-2" 		, "./assets/Grunt/Grunt-Climb-2.png" },
                    { "Grunt-Climb-3" 		, "./assets/Grunt/Grunt-Climb-3.png" }, 
                    { "Grunt-Dead-1" 		, "./assets/Grunt/Grunt-Dead-1.png" },
                    { "Grunt-Dead-2" 		, "./assets/Grunt/Grunt-Dead-2.png" },
                    { "Grunt-Dead-3" 		, "./assets/Grunt/Grunt-Dead-3.png" },
                    { "Grunt-Dead-4" 		, "./assets/Grunt/Grunt-Dead-4.png" },
                    { "Grunt-Dead-5" 		, "./assets/Grunt/Grunt-Dead-5.png" },
                };
                
                for (int i = 0, j = 0; i < 32; ++i)
                {
                    if (playerTextureList[i][j])
                    {
                        ReadWriteOperations = SDL_RWFromFile(playerTextureList[i][j + 1], "rb");
                        HashSet_Insert_AssetTexture(GlobalEntityTextureSet, playerTextureList[i][j],
                                                    LoadAssetPNG(GlobalGameState, ReadWriteOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));
                    }
                    else
                    {
                        // TODO(nick):
                        // 1) after initial loading of all textures - build sprite animations for each entity
                        BuildPlayerAnimations(PlayerEntity, GlobalEntityTextureSet, PlayerAnimations, GlobalGameState->Memory->PermanentStorage);
                        break;
                    }
                }
                
                // NOTE(nick): set default texture on game init
                PlayerEntity->Id = GlobalEntityArrayIndex;
                PlayerEntity->CurrentState = (EntityState)(Idle);
                PlayerEntity->CurrentFaceDirection = (EntityFaceDirection)(FaceRight);
                PlayerEntity->CurrentTexture = HashSet_Select_AssetTexture(GlobalEntityTextureSet, "Grunt-Idle");
                PlayerEntity->PositionV2 = DefaultVector2CenterScreen(GlobalWindowState->Width, GlobalWindowState->Height);
                PlayerEntity->PositionV2.X -= 100;
                
                int collisionBoxWidth = 0;
                int collisionBoxHeight = 0;
                
                collisionBoxWidth = ((PlayerEntity->CurrentTexture->Width / 4) * 2);
                collisionBoxHeight = ((PlayerEntity->CurrentTexture->Height / 4) * 2);
                PlayerEntity->CollisionBox = 
                {
                    PlayerEntity->PositionV2.X + (PlayerEntity->CurrentTexture->Width / 4),
                    PlayerEntity->PositionV2.Y + (PlayerEntity->CurrentTexture->Height / 4),
                    collisionBoxWidth,
                    collisionBoxHeight,
                };
                
                GlobalEntityArray[GlobalEntityArrayIndex] = PlayerEntity;
                ++GlobalEntityArrayIndex;
                
                // NOTE(nick): gretel initialization
                GretelEntity = (Entity *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                         sizeof(Entity));
                char *gretelTextureList[32][2] = 
                {
                    { "Gretel-Idle",            "./assets/Gretel/Gretel-Idle.png" },
                    { "Gretel-Angry-1",         "./assets/Gretel/Gretel-Angry-1.png" },
                    { "Gretel-Angry-2",         "./assets/Gretel/Gretel-Angry-2.png" },
                    { "Gretel-Jump-1",          "./assets/Gretel/Gretel-Jump-1.png" },
                    { "Gretel-Jump-2",          "./assets/Gretel/Gretel-Jump-2.png" },
                    { "Gretel-Shock-1",         "./assets/Gretel/Gretel-Shock-1.png" },
                    { "Gretel-Shock-2",         "./assets/Gretel/Gretel-Shock-2.png" },
                    { "Gretel-Hold-Green-Beer", "./assets/Gretel/Gretel-Hold-Green-Beer.png" },
                    { "Gretel-Hold-Red-Beer",   "./assets/Gretel/Gretel-Hold-Red-Beer.png" },
                    { "Gretel-Beer-Toss-Empty", "./assets/Gretel/Gretel-Beer-Toss-Empty.png" },
                    { "Gretel-Beer-Toss-Green", "./assets/Gretel/Gretel-Beer-Toss-Green.png" },
                    { "Gretel-Beer-Toss-Red",   "./assets/Gretel/Gretel-Beer-Toss-Red.png" },
                    { "Gretel-Climb-1",         "./assets/Gretel/Gretel-Climb-1.png" },
                    { "Gretel-Climb-2",         "./assets/Gretel/Gretel-Climb-2.png" },
                    { "Gretel-Defeat-1",        "./assets/Gretel/Gretel-Defeat-1.png" },
                    { "Gretel-Defeat-2",        "./assets/Gretel/Gretel-Defeat-2.png" },
                    { "Gretel-Fall",            "./assets/Gretel/Gretel-Fall.png" },
                    { "Gretel-Growl",           "./assets/Gretel/Gretel-Growl.png" },
                };
                
                for (int i = 0, j = 0; i < 32; ++i)
                {
                    if (gretelTextureList[i][j])
                    {
                        ReadWriteOperations = SDL_RWFromFile(gretelTextureList[i][j + 1], "rb");
                        HashSet_Insert_AssetTexture(GlobalEntityTextureSet, gretelTextureList[i][j],
                                                    LoadAssetPNG(GlobalGameState, ReadWriteOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));
                    }
                }
                
                GretelEntity->Id = GlobalEntityArrayIndex;
                GretelEntity->CurrentState = (EntityState)(Idle);
                GretelEntity->CurrentTexture = HashSet_Select_AssetTexture(GlobalEntityTextureSet, "Gretel-Idle");
                GretelEntity->PositionV2 = DefaultVector2CenterScreen(GlobalWindowState->Width, GlobalWindowState->Height);
                
                collisionBoxWidth = ((GretelEntity->CurrentTexture->Width / 4) * 2);
                collisionBoxHeight = ((GretelEntity->CurrentTexture->Height / 4) * 2);
                GretelEntity->CollisionBox = 
                {
                    GretelEntity->PositionV2.X + (GretelEntity->CurrentTexture->Width / 4),
                    GretelEntity->PositionV2.Y + (GretelEntity->CurrentTexture->Height / 4),
                    collisionBoxWidth,
                    collisionBoxHeight,
                };
                
                GlobalEntityArray[GlobalEntityArrayIndex] = GretelEntity;
                ++GlobalEntityArrayIndex;
                
                // NOTE(nick): game font initialization
                ArcadeFont_Small = TTF_OpenFont("./assets/Fonts/arcade_classic/ARCADECLASSIC.TTF", 10);
                if (!ArcadeFont_Small)
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                ArcadeFont_Medium = TTF_OpenFont("./assets/Fonts/arcade_classic/ARCADECLASSIC.TTF", 24);
                if (!ArcadeFont_Medium) 
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                ArcadeFont_Large = TTF_OpenFont("./assets/Fonts/arcade_classic/ARCADECLASSIC.TTF", 80);
                if (!ArcadeFont_Large)
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                PokeFont_Small = TTF_OpenFont("./assets/Fonts/poke_font/POKE.FON", 10);
                if (!PokeFont_Small)
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                PokeFont_Medium = TTF_OpenFont("./assets/Fonts/poke_font/POKE.FON", 35);
                if (!PokeFont_Medium)
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                PokeFont_Large = TTF_OpenFont("./assets/Fonts/poke_font/POKE.FON", 80);
                if (!PokeFont_Large)
                {
                    printf("ERROR - failed to load TTF file - SDL_ttf Error: %s\n", TTF_GetError());
                }
                
                TitleScreenGretelKong_1 = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                                  sizeof(Text));
                TitleScreenGretelKong_2 = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                                  sizeof(Text));
                // Color - RGBA
                // TODO(nick): global color palette?
                SDL_Color Blue = { 0, 0, 255, 0 };
                SDL_Color Red = { 255, 0, 0, 0 };
                SDL_Color White = { 255, 255, 255, 0 };
                
                TitleScreenGretelKong_1->Texture = LoadAssetTTF(GlobalGameState,
                                                                ArcadeFont_Large,
                                                                GlobalWindowState->GameSurface,
                                                                GlobalWindowState->GameRenderer, 
                                                                "GRONKEY",
                                                                &Blue);
                // TODO(nick): center of screen and then offset
                //TitleScreenGretelKong_1->PositionV2 = DefaultVector2Position();
                TitleScreenGretelKong_1->PositionV2 = DefaultVector2CenterScreen(GlobalWindowState->Width, GlobalWindowState->Height);
                TitleScreenGretelKong_1->PositionV2.Y -= 175;
                TitleScreenGretelKong_1->PositionV2.X -= (TitleScreenGretelKong_1->Texture->Width / 2);
                
                TitleScreenGretelKong_2->Texture = LoadAssetTTF(GlobalGameState,
                                                                ArcadeFont_Large,
                                                                GlobalWindowState->GameSurface,
                                                                GlobalWindowState->GameRenderer,
                                                                "KONG",
                                                                &Blue);
                // TODO(nick): center part 2 based on positioning of part 1
                // TODO(nick): figure out a better way of handling the text positioning
                // NOTE(nick): 5 is an offset to allow texture to sit a bit closer
                TitleScreenGretelKong_2->PositionV2.X = TitleScreenGretelKong_1->PositionV2.X + (TitleScreenGretelKong_1->Texture->Width / 4);
                TitleScreenGretelKong_2->PositionV2.Y = (TitleScreenGretelKong_1->PositionV2.Y + (TitleScreenGretelKong_1->Texture->Height / 2) + 15);
                
                HUDHighScore = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                       sizeof(Text));
                HUDHighScoreValue = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                            sizeof(Text));
                HUDHighScore->Texture = LoadAssetTTF(GlobalGameState,
                                                     ArcadeFont_Medium,
                                                     GlobalWindowState->GameSurface,
                                                     GlobalWindowState->GameRenderer,
                                                     "HIGH  SCORE",
                                                     &Red);
                HUDHighScore->PositionV2 = DefaultVector2CenterScreen((GlobalWindowState->Width - HUDHighScore->Texture->Width), 0);
                // TODO(nick): keep a current highscore value and write out as a string
                HUDHighScoreValue->Texture = LoadAssetTTF(GlobalGameState,
                                                          ArcadeFont_Medium,
                                                          GlobalWindowState->GameSurface,
                                                          GlobalWindowState->GameRenderer,
                                                          "000000",
                                                          &White);
                HUDHighScoreValue->PositionV2.X = (HUDHighScore->PositionV2.X + (HUDHighScore->Texture->Width / 4));
                HUDHighScoreValue->PositionV2.Y = (HUDHighScore->PositionV2.Y + (HUDHighScore->Texture->Height / 2) + 5);
                
                HUDLivesCount = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                        sizeof(Text));
                HUDCurrentScore = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                          sizeof(Text));
                // TODO(nick): keep track of current lives count - texture needs to be rendered on update
                HUDLivesCount->Texture = LoadAssetTTF(GlobalGameState,
                                                      ArcadeFont_Medium,
                                                      GlobalWindowState->GameSurface,
                                                      GlobalWindowState->GameRenderer,
                                                      "0 UP",
                                                      &Red);
                HUDLivesCount->PositionV2 = DefaultVector2Position();
                HUDCurrentScore->Texture = LoadAssetTTF(GlobalGameState,
                                                        ArcadeFont_Medium,
                                                        GlobalWindowState->GameSurface,
                                                        GlobalWindowState->GameRenderer,
                                                        "000000",
                                                        &White);
                HUDCurrentScore->PositionV2 = DefaultVector2Position();
                HUDCurrentScore->PositionV2.Y += HUDLivesCount->Texture->Height;
                
                HUDCurrentLevel = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                          sizeof(Text));
                HUDCurrentLevel->Texture = LoadAssetTTF(GlobalGameState,
                                                        ArcadeFont_Medium,
                                                        GlobalWindowState->GameSurface,
                                                        GlobalWindowState->GameRenderer,
                                                        "L 00",
                                                        &Blue);
                HUDCurrentLevel->PositionV2 = DefaultVector2CenterScreen(GlobalWindowState->Width, 0);
                HUDCurrentLevel->PositionV2.X += 100;
                
                // NOTE(nick): debug info text
                // TODO(nicK): have this toggle on a key press or something similar
                DEBUG_StringBuffer = (char *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                             (sizeof(char) * 50));
                
                DEBUG_PlayerPositionInfo = (Text *)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                                   sizeof(Text));
                sprintf(DEBUG_StringBuffer, "Player Position x %d y %d ", PlayerEntity->PositionV2.X, PlayerEntity->PositionV2.Y);
                DEBUG_PlayerPositionInfo->Texture = LoadAssetTTF(GlobalGameState,
                                                                 PokeFont_Small,
                                                                 GlobalWindowState->GameSurface,
                                                                 GlobalWindowState->GameRenderer,
                                                                 DEBUG_StringBuffer,
                                                                 &White);
                DEBUG_PlayerPositionInfo->PositionV2.X = 400;
                DEBUG_PlayerPositionInfo->PositionV2.Y = 400;
                
                DEBUG_EnemyPositionInfo = (Text*)PushMemoryChunk(GlobalGameState->Memory->PermanentStorage,
                                                                 sizeof(Text));
                sprintf(DEBUG_StringBuffer, "Enemy Position x %d y: %d", GretelEntity->PositionV2.X, GretelEntity->PositionV2.Y);
                DEBUG_EnemyPositionInfo->Texture = LoadAssetTTF(GlobalGameState,
                                                                PokeFont_Small,
                                                                GlobalWindowState->GameSurface,
                                                                GlobalWindowState->GameRenderer,
                                                                DEBUG_StringBuffer,
                                                                &White);
                DEBUG_EnemyPositionInfo->PositionV2.X = 400;
                DEBUG_EnemyPositionInfo->PositionV2.Y = 425;
                
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

AssetTexture *
LoadAssetTTF(GameState *CurrentGameState, TTF_Font *Font, SDL_Surface *GameSurface, SDL_Renderer *GameRenderer, char *text, SDL_Color *Color)
{
    AssetTexture *Result = NULL;
    SDL_Texture *Texture = NULL;
    
    if (!Color)
    {
        SDL_Color White = { 255, 255, 255, 0 };
        Color = &White;
    }
    
    SDL_Surface *Raw = TTF_RenderText_Solid(Font, text, *Color);
    
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
UpdateAssetTTF(SDL_Renderer *GameRenderer, Text *CurrentTextAsset, TTF_Font *Font, char *text, SDL_Color *Color)
{
    if (!Color)
    {
        SDL_Color White = { 255, 255, 255, 0 };
        Color = &White;
    }
    SDL_Surface *Raw = TTF_RenderText_Solid(Font, text, *Color);
    if (!Raw)
    {
        printf("ERROR - SDL_ttf could not load text properly - SDL_ttf: %s\n", TTF_GetError());
    }
    else
    {
        // TODO(nick): a free texture might need to be called on this?
        if (CurrentTextAsset->Texture->Texture)
        {
            SDL_DestroyTexture(CurrentTextAsset->Texture->Texture);
        }
        CurrentTextAsset->Texture->Texture = SDL_CreateTextureFromSurface(GameRenderer, Raw);
    }
}


// TODO(nick): 
// 1) only load level if it has not been loaded - should not be checked every frame
// 2) IMPORTANT(nick): need to think about scaling of entities / tiles
Level *
LoadLevel(GameState *CurrentGameState, SDL_RWops *RWOperations, char *fileName)
{
    Level *LoadedLevel = NULL;
    FILE *LevelFile = fopen(fileName, "rb");
    if (LevelFile)
    {
        LoadedLevel = (Level *)PushMemoryChunk(CurrentGameState->Memory->PermanentStorage,
                                               sizeof(Level));
        LoadedLevel->TileList.IsEmpty = true;
        StringCopyOverwrite(LoadedLevel->FileName, fileName, array_len(LoadedLevel->FileName));
        // TODO(nick): clear this up ...
        char stringBuffer[2048] = { 0 };
        char actualAssetName[64] = { 0 };
        char assetPath[256] = { 0 };
        char assetBuffer[3] = { 0 };
        int i = 0;
        int charCount = 0;
        int currentKey = 0;
        int tileIndex = 0;
        int rowIndex = 0;
        int columnIndex = 0;
        // read each line
        while (fgets(stringBuffer, sizeof(stringBuffer), LevelFile))
        {
            // read two characters
            while (*(stringBuffer + i) != '\0')
            {
                if (*(stringBuffer + i) == ' '  || 
                    *(stringBuffer + i) == '\r' ||
                    *(stringBuffer + i) == '\n')
                {
                    *(assetBuffer + charCount) = '\0';
                    charCount = 0;
                    // TODO(nick):
                    // 1) make load asset and pass code to function
                    // 2) check if asset needs to be loade
                    if (*(assetBuffer + 0) != '0' || *(assetBuffer + 1) != '0')
                    {
                        
                        DecodeAssetName(assetBuffer, actualAssetName, array_len(actualAssetName));
                        // TODO(nick): 
                        // 1) IMPORTANT(nick): hash collision finally happed
                        currentKey = SimpleHash(actualAssetName);
                        if (GlobalLevelTextures[currentKey].Value == NULL)
                        {
                            // TODO(nick): create simple string compare
                            StringClear(assetPath, array_len(assetPath));
                            StringConcatenate(assetPath, "./assets/level/");
                            StringConcatenate(assetPath, actualAssetName);
                            StringConcatenate(assetPath, ".png");
                            RWOperations = SDL_RWFromFile(assetPath, "rb");
                            HashSet_Insert_AssetTexture(GlobalLevelTextures, assetBuffer, LoadAssetPNG(CurrentGameState, RWOperations, GlobalWindowState->GameSurface, GlobalWindowState->GameRenderer));
                        }
                        Tile *CurrentTile = (Tile *)PushMemoryChunk(CurrentGameState->Memory->PermanentStorage,
                                                                    sizeof(Tile));
                        CurrentTile->Id = tileIndex;
                        CurrentTile->IsStatic = false;
                        // TODO(nick): actualAssetName should determine whether or not a tile is collidable
                        CurrentTile->IsCollidable = IsCollidable(assetBuffer);
                        CurrentTile->CurrentTexture = HashSet_Select_AssetTexture(GlobalLevelTextures, assetBuffer);
                        
                        // T
                        
                        // NOTE(nick): rowIndex zero maps to top left corner
                        CurrentTile->PositionV2 = 
                        {
                            CurrentTile->CurrentTexture->Width * columnIndex,
                            CurrentTile->CurrentTexture->Height * rowIndex,
                        };
                        // TODO(nick): figure out positioning / collision
                        CurrentTile->CollisionBox = 
                        {
                            CurrentTile->CurrentTexture->Width * columnIndex,
                            CurrentTile->CurrentTexture->Height * rowIndex,
                            CurrentTile->CurrentTexture->Width,
                            CurrentTile->CurrentTexture->Height,
                        };
                        TileList_Node *CurrentTileNode = (TileList_Node *)PushMemoryChunk(CurrentGameState->Memory->PermanentStorage,
                                                                                          sizeof(TileList_Node));
                        CurrentTileNode->Value = CurrentTile;
                        CurrentTileNode->Next = NULL;
                        TileList_Add(&LoadedLevel->TileList, CurrentTileNode);
                    }
                    ++tileIndex;
                    if (*(stringBuffer + i) == '\r' ||
                        *(stringBuffer + i) == '\n')
                    {
                        i = 0;
                        columnIndex = 0;
                        ++rowIndex;
                        break;
                    }
                    ++columnIndex;
                }
                else
                {
                    *(assetBuffer + charCount) = *(stringBuffer + i);
                    ++charCount;
                }
                ++i;
            }
        }
        fclose(LevelFile);
    }
    else
    {
        printf("ERROR: Cannot load level file -> %s\n", fileName);
    }
    return LoadedLevel;
}

bool
CheckCollision(Entity *EntityArray[50], Level *CurrentLevel, int checkIndex)
{
    int i = 0;
    Entity *checkEntity = EntityArray[checkIndex];
    Entity *currentEntity = EntityArray[i]; 
    
    Rectangle CheckEntityCollisionBox = {};
    Rectangle CurrentEntityCollisionBox = {};
    
    // NOTE(nick):
    // duplication of line calculations here
    // figure out a better way to handle rectangle collision
    CheckEntityCollisionBox.BottomLine[0].X = checkEntity->CollisionBox.x;
    CheckEntityCollisionBox.BottomLine[0].Y = checkEntity->CollisionBox.y;
    CheckEntityCollisionBox.BottomLine[1].X = checkEntity->CollisionBox.x + checkEntity->CollisionBox.w;
    CheckEntityCollisionBox.BottomLine[1].Y = checkEntity->CollisionBox.y;
    
    CheckEntityCollisionBox.TopLine[0].X = checkEntity->CollisionBox.x;
    CheckEntityCollisionBox.TopLine[0].Y = checkEntity->CollisionBox.y + checkEntity->CollisionBox.h;
    CheckEntityCollisionBox.TopLine[1].X = checkEntity->CollisionBox.x + checkEntity->CollisionBox.w;
    CheckEntityCollisionBox.TopLine[1].Y = checkEntity->CollisionBox.y + checkEntity->CollisionBox.h;
    
    // NOTE(nicK):
    // check all collision between entities first
    while (currentEntity != NULL)
    {
        if (i != checkIndex)
        {
            if (currentEntity)
            {
                CurrentEntityCollisionBox.BottomLine[0].X = currentEntity->CollisionBox.x;
                CurrentEntityCollisionBox.BottomLine[0].Y = currentEntity->CollisionBox.y;
                CurrentEntityCollisionBox.BottomLine[1].X = currentEntity->CollisionBox.x + currentEntity->CollisionBox.w;
                CurrentEntityCollisionBox.BottomLine[1].Y = currentEntity->CollisionBox.y;
                
                CurrentEntityCollisionBox.TopLine[0].X = currentEntity->CollisionBox.x;
                CurrentEntityCollisionBox.TopLine[0].Y = currentEntity->CollisionBox.y + currentEntity->CollisionBox.h;
                CurrentEntityCollisionBox.TopLine[1].X = currentEntity->CollisionBox.x + currentEntity->CollisionBox.w;
                CurrentEntityCollisionBox.TopLine[1].Y = currentEntity->CollisionBox.y + currentEntity->CollisionBox.h;
                
                // check horizontal collision
                if ((CheckEntityCollisionBox.BottomLine[0].Y <= CurrentEntityCollisionBox.TopLine[0].Y) &&
                    (CheckEntityCollisionBox.TopLine[0].Y >= CurrentEntityCollisionBox.BottomLine[0].Y))
                {
                    // check right side collision from CheckEntity perspective
                    if (CheckEntityCollisionBox.BottomLine[1].X >= CurrentEntityCollisionBox.BottomLine[0].X &&
                        CheckEntityCollisionBox.BottomLine[0].X <= CurrentEntityCollisionBox.BottomLine[1].X)
                    {
                        return true;
                    }
                    // check left side collision from CheckEntity perspective
                    else if (CheckEntityCollisionBox.BottomLine[0].X > CurrentEntityCollisionBox.BottomLine[1].X)
                    {
                        if (CheckEntityCollisionBox.BottomLine[0].X <= CurrentEntityCollisionBox.BottomLine[1].X)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        ++i;
        currentEntity = EntityArray[i];
    }
    
    
    // IMPORTANT(nick):
    // certain tiles are not collidable, but they instead allow a specific function (i.e., ladders for "climbing")
    // NOTE(nick):
    // check all collision between entity and level tiles
    TileList_Node *CurrentNode = NULL;
    Tile *CurrentTile = NULL; Rectangle CurrentTileCollisionBox = {};
    if (CurrentLevel)
    {
        CurrentNode = CurrentLevel->TileList.Head;
        CurrentTile = CurrentNode->Value;
    }
    while (CurrentNode != NULL) 
    {
        if (CurrentTile->IsCollidable)
        {
            CurrentTileCollisionBox.BottomLine[0].X = CurrentTile->CollisionBox.x;
            CurrentTileCollisionBox.BottomLine[0].Y = CurrentTile->CollisionBox.y;
            CurrentTileCollisionBox.BottomLine[1].X = CurrentTile->CollisionBox.x + CurrentTile->CollisionBox.w;
            CurrentTileCollisionBox.BottomLine[1].Y = CurrentTile->CollisionBox.y;
            
            CurrentTileCollisionBox.TopLine[0].X = CurrentTile->CollisionBox.x;
            CurrentTileCollisionBox.TopLine[0].Y = CurrentTile->CollisionBox.y + CurrentTile->CollisionBox.h;
            CurrentTileCollisionBox.TopLine[1].X = CurrentTile->CollisionBox.x + CurrentTile->CollisionBox.w;
            CurrentTileCollisionBox.TopLine[1].Y = CurrentTile->CollisionBox.y + CurrentTile->CollisionBox.h;
            
            // check horizontal collision
            if ((CheckEntityCollisionBox.BottomLine[0].Y <= CurrentTileCollisionBox.TopLine[0].Y) &&
                (CheckEntityCollisionBox.TopLine[0].Y >= CurrentTileCollisionBox.BottomLine[0].Y))
            {
                // check right side collision from CheckEntity perspective
                if (CheckEntityCollisionBox.BottomLine[1].X >= CurrentTileCollisionBox.BottomLine[0].X &&
                    CheckEntityCollisionBox.BottomLine[0].X <= CurrentTileCollisionBox.BottomLine[1].X)
                {
                    return true;
                }
                // check left side collision from CheckEntity perspective
                else if (CheckEntityCollisionBox.BottomLine[0].X > CurrentTileCollisionBox.BottomLine[1].X)
                {
                    if (CheckEntityCollisionBox.BottomLine[0].X <= CurrentTileCollisionBox.BottomLine[1].X)
                    {
                        return true;
                    }
                }
            }
        }
        if ((CurrentNode = CurrentNode->Next) != NULL)
        {
            CurrentTile = CurrentNode->Value;
        }
    }
    
    return false;
}

void 
GameUpdateAndRender(WindowState *CurrentWindowState, GameState *CurrentGameState, Queue_GameEntity *EntityQueue, Queue_GameText *TextQueue, Level *CurrentLoadedLevel)
{
    // render texture(s) to screen
    // TODO(nick):
    // 1) 1st NULL value is clip box 
    //    - srcrect 
    // 2) 2nd NULL value is center point
    //    - center, used for point to determine rotation
    
    // NOTE(nick): debug info
    {
        sprintf(DEBUG_StringBuffer, "Player Collision x %d y %d ", PlayerEntity->CollisionBox.x, PlayerEntity->CollisionBox.y);
        UpdateAssetTTF(CurrentWindowState->GameRenderer,
                       DEBUG_PlayerPositionInfo,
                       PokeFont_Small,
                       DEBUG_StringBuffer,
                       NULL);
        sprintf(DEBUG_StringBuffer, "Enemy Collision x %d y: %d", GretelEntity->CollisionBox.x, GretelEntity->CollisionBox.y);
        UpdateAssetTTF(CurrentWindowState->GameRenderer,
                       DEBUG_EnemyPositionInfo,
                       PokeFont_Small,
                       DEBUG_StringBuffer,
                       NULL);
    }
    
    // TODO(nick):
    // 1) render level first and check collision between entities and level
    TileList_Node *CurrentTileNode = NULL;
    SDL_Surface *tempTileCollisionSurface;
    SDL_Texture *tempTileCollisionTexture;
    if (CurrentLoadedLevel)
    {
        CurrentTileNode = CurrentLoadedLevel->TileList.Head;
        while (CurrentTileNode)
        {
            Tile *CurrentTile = CurrentTileNode->Value;
            SDL_Rect CurrentRenderBox =
            {
                CurrentTile->PositionV2.X,
                CurrentTile->PositionV2.Y,
                CurrentTile->CurrentTexture->Width,
                CurrentTile->CurrentTexture->Height,
            };
            SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
                             CurrentTile->CurrentTexture->Texture,
                             NULL,
                             &CurrentRenderBox,
                             CurrentTile->CurrentTexture->Rotation,
                             NULL,
                             CurrentTile->CurrentTexture->Flip);
            CurrentTileNode = CurrentTileNode->Next;
            // NOTE(nick): debug collision box render
            {
                tempTileCollisionSurface = SDL_CreateRGBSurface(0, CurrentTile->CollisionBox.w, CurrentTile->CollisionBox.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
                if (tempTileCollisionSurface == NULL)
                {
                    SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
                }
                SDL_FillRect(tempTileCollisionSurface, NULL, SDL_MapRGBA(tempTileCollisionSurface->format, 0, 255, 0, 64));
                tempTileCollisionTexture = SDL_CreateTextureFromSurface(CurrentWindowState->GameRenderer, tempTileCollisionSurface);
                SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
                                 tempTileCollisionTexture,
                                 NULL,
                                 &CurrentTile->CollisionBox,
                                 0,
                                 NULL,
                                 SDL_FLIP_NONE);
                SDL_FreeSurface(tempTileCollisionSurface);
                SDL_DestroyTexture(tempTileCollisionTexture);
            }
        }
    }
    
    Entity *CurrentEntity = NULL; 
    SDL_Surface *tempEntityCollisionSurface;
    SDL_Texture *tempEntityCollisionTexture;
    while (CurrentEntity = Queue_Dequeue_GameEntity(EntityQueue))
    {
        // TODO(nick):
        // 1) if collision is detected, do not move position
        SDL_Rect CurrentRenderBox = 
        {
            CurrentEntity->PositionV2.X,
            CurrentEntity->PositionV2.Y,
            CurrentEntity->CurrentTexture->Width,
            CurrentEntity->CurrentTexture->Height,
        };
        SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
                         CurrentEntity->CurrentTexture->Texture,
                         NULL,
                         &CurrentRenderBox,
                         CurrentEntity->CurrentTexture->Rotation,
                         NULL,
                         CurrentEntity->CurrentTexture->Flip);
        
        // NOTE(nick): debug collision box render
        {
            // TODO(nick):
            // 1) clean the debugging stuff up a bit to just create one texture instead of creating one per frame and destroying it
            tempEntityCollisionSurface = SDL_CreateRGBSurface(0, CurrentEntity->CollisionBox.w, CurrentEntity->CollisionBox.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
            if (tempEntityCollisionSurface == NULL)
            {
                SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
            }
            SDL_FillRect(tempEntityCollisionSurface, NULL, SDL_MapRGBA(tempEntityCollisionSurface->format, 255, 0, 0, 64));
            tempEntityCollisionTexture = SDL_CreateTextureFromSurface(CurrentWindowState->GameRenderer, tempEntityCollisionSurface);
            SDL_RenderCopyEx(CurrentWindowState->GameRenderer,
                             tempEntityCollisionTexture,
                             NULL,
                             &CurrentEntity->CollisionBox,
                             0,
                             NULL,
                             SDL_FLIP_NONE); 
            SDL_FreeSurface(tempEntityCollisionSurface);
            SDL_DestroyTexture(tempEntityCollisionTexture);
        }
        
    }
    
    Text *CurrentText = NULL;
    while (CurrentText = Queue_Dequeue_GameText(TextQueue))
    {
        SDL_Rect TextRenderBox = 
        {
            CurrentText->PositionV2.X,
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
