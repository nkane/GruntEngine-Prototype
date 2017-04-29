#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define global_variable static

global_variable const int Screen_Width = 640;
global_variable const int Screen_Height = 480;
global_variable const int Sdl_Image_Flags = IMG_INIT_PNG;

int
main(int argc, char *argv[])
{
	// window to be rendered to
	SDL_Window *window = NULL;

	// surface contained by window
	SDL_Surface *screenSurface = NULL; 

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("ERROR - SDL could not init - SDL_Error: %s\n", SDL_GetError());
	}
	else 
	{
		// create window
		window = SDL_CreateWindow("Prototype Alpha 0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);
		if (window)
		{
			// initialize sdl image
			int sdlImageInit = IMG_Init(Sdl_Image_Flags);

			// get window surface
			screenSurface = SDL_GetWindowSurface(window);

			// fill surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			// update the surface
			SDL_UpdateWindowSurface(window);

			// wait five seconds
			SDL_Delay(5000);
		}
		else
		{
			printf("ERROR - SDL could not create window - SDL_Error: %s\n", SDL_GetError());
		}
	}

	// Destroy window
	SDL_DestroyWindow(window);

	// quit SDL subsystems
	SDL_Quit();

	printf("Hello ...\n");
	return 0;
}
