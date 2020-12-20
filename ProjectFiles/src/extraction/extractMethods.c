#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../.h/tuple.h"
#include <SDL2/SDL2_rotozoom.h>

//Crop the image on x-axis
int split_x(SDL_Surface *img, int init, int dest, char* path)
{
	// get the height of the new image
	int height = dest - init;

	SDL_Init(SDL_INIT_VIDEO);

	// create new image
	SDL_Surface *dest_surface = SDL_CreateRGBSurface(0, img->w, height, 32, 0, 0, 0, 0);

    //Begin: pixel manipulation

    SDL_Surface * originalImage = dest_surface;
    dest_surface = SDL_ConvertSurfaceFormat(dest_surface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	SDL_Rect srcrect;

	srcrect.x = 0;
	srcrect.y = init;
	srcrect.w = img->w;
	srcrect.h = height;

	// we crop the initial image into the new image
	SDL_BlitSurface(img, &srcrect, dest_surface, NULL);

	SDL_SaveBMP(dest_surface, path);


return 0;
}

//Crop the image on y-axis
SDL_Surface* split_y(SDL_Surface *img, int init, int dest, char* path)
{
	int weight = dest - init;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface *dest_surface = SDL_CreateRGBSurface(0, weight, img->h, 32, 0, 0, 0, 0);

    //Begin: pixel manipulation

    SDL_Surface * originalImage = dest_surface;
    dest_surface = SDL_ConvertSurfaceFormat(dest_surface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	SDL_Rect srcrect;

	srcrect.x = init;
	srcrect.y = 0;
	srcrect.w = weight;
	srcrect.h = img->h;

	SDL_BlitSurface(img, &srcrect, dest_surface, NULL);

	SDL_SaveBMP(dest_surface, path);


return dest_surface;
}

void image_rotation(char *path, int angle)
{
    SDL_Init(SDL_INIT_VIDEO);

	SDL_Surface *image = IMG_Load(path); //Load image

	SDL_Surface *dest_surface = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0, 0, 0, 0);

    dest_surface = rotateSurface90Degrees(image, angle);

    SDL_SaveBMP(dest_surface, path);
    SDL_Quit();
}