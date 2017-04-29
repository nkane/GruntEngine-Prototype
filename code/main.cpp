#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define global_variable static

global_variable const int Screen_Width = 640;
global_variable const int Screen_Height = 480;
global_variable const int Sdl_Image_Flags = IMG_INIT_PNG;

global_variable SDL_RWops *ReadWriteOperations;

// TODO(nick): move in to a .h file
struct Entity
{
	// TODO(nick): think about how to store entity assets
	SDL_Surface *CurrentImage;
};

global_variable Entity *PlayerEntity;

SDL_Window *
InitializeGame();

int
main(int argc, char *argv[])
{
	// window to be rendered to
	SDL_Window *Window = NULL;

	// surface contained by window
	SDL_Surface *ScreenSurface = NULL; 

	if((Window = InitializeGame()) != NULL)
	{
		// game initialized successfully
		ScreenSurface = (SDL_Surface *)malloc(sizeof(SDL_Surface));
		ScreenSurface = SDL_GetWindowSurface(Window);

		// blit test asset
		SDL_BlitSurface(PlayerEntity->CurrentImage, NULL, ScreenSurface, NULL);

		SDL_UpdateWindowSurface(Window);
		
		SDL_Delay(10000);

		// TODO(nick): enter game loop
	}
	else
	{
		// TODO(nick): proper logging / clean exit
		return -1;
	}

	// TODO(nick): stream line some clean up process that is going to unload assest / clean up memory
	// Destroy window
	SDL_DestroyWindow(Window);

	// quit SDL image
	IMG_Quit();

	// quit SDL subsystems
	SDL_Quit();

	return 0;
}

SDL_Window *
InitializeGame()
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
		// create window
		Window = SDL_CreateWindow("Prototype Alpha 0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);
		if (Window)
		{
			// initialize sdl image
			int sdlImageInit = IMG_Init(Sdl_Image_Flags);

			if ((sdlImageInit & Sdl_Image_Flags) != Sdl_Image_Flags)
			{
				// TODO(nick): proper logging / clean exit
				printf("ERROR - SDL_image did not initialize properly - IMG_Error: %s\n", IMG_GetError());
			}

			// TODO(nick): instead of single malloc - do a large malloc (or lower level call and control the memory / clean up
			// start loading game assets
			PlayerEntity = (Entity *)malloc(sizeof(Entity));
			// TODO(nick): add checking to make sure assets load properly - else log some failure message
			ReadWriteOperations = SDL_RWFromFile("./assets/test_asset.png", "rb");
		 	PlayerEntity->CurrentImage = IMG_LoadPNG_RW(ReadWriteOperations);

			// get window surface
		}
		else
		{
			// TODO(nick): proper logging / clean exit
			printf("ERROR - SDL could not create window - SDL_Error: %s\n", SDL_GetError());
		}
	}
	
	return Window;
}

