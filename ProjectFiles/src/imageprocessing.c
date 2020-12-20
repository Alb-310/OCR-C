#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Contrast filter
Uint8 contrast(Uint8 c, float n)
    {
        if(c <= 255 / 2)
            return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, n));
        else
            return 255 - contrast(255 - c, n);
    }

// Black and white filter
Uint32 bnw(SDL_Surface *surface, Uint8 r, Uint8 g, Uint8 b)
    {
        int bright = 0;
        float c = 1.5;

        if (r > 255/c)
            bright++;
        if (g > 255/c)
            bright++;
        if (b > 255/c)
            bright++;

        if (bright < 2)
            return SDL_MapRGB(surface->format, 0, 0, 0);
        else
            return SDL_MapRGB(surface->format, 255, 255, 255);

    }

// Remove impurity of the picture
Uint32 impurity(SDL_Surface *surface, Uint32 * pixels, int i, int j)
    {
        SDL_Color color_x;
        SDL_Color color_y;
        int x = 0;
        int y = 0;
        
        for(int k = 1; k <= 50; k++)
        {   
            // We check if there are pixels are black on the x-axis
            if(k <= 25)
            {
                if(j+k < surface->w)
                {
                    SDL_GetRGB(pixels[i * surface->w + j + k], surface->format, &color_x.r, &color_x.g, &color_x.b);
                    if (color_x.r == 0 && color_x.g == 0 && color_x.b == 0)
                        x++;
                    else
                        x = 0;
                }
            }
            
            if(i+k < surface->h)
            {
                // We check if there are pixels are black on the y-axis
                SDL_GetRGB(pixels[(i + k) * surface->w + j], surface->format, &color_y.r, &color_y.g, &color_y.b);
      
                if (color_y.r == 0 && color_y.g == 0 && color_y.b == 0)
                        y++;
                else
                    y = 0;
            }
        }
        
        // Set pixels in a white color to erase them (if thers are too many black pixels) on the x-axis
        if(x >= 50 && j + x < surface->w)
        {
            for(int l = 1; l <= x; l++)
                pixels[i * surface->w + j + l] = SDL_MapRGB(surface->format, 255, 255, 255);

            return SDL_MapRGB(surface->format, 255, 255, 255);
        }

        // same on the y-axis
        if(y >= 60 && i + y < surface->h)
        {
            for(int l = 1; l <= y; l++)
                pixels[(i + l) * surface->w + j] = SDL_MapRGB(surface->format, 255, 255, 255);

            return SDL_MapRGB(surface->format, 255, 255, 255);
        }
        
        // else we return same pixel
        return pixels[i * surface->w + j];
    }
 

// Main function which apply filters 
int Image_Processing(char* argv[], int m)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *image = IMG_Load(argv[0]); //Load image

    //Begin: pixel manipulation

    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

    Uint32 * pixels = (Uint32 *)image->pixels;

    if (m == 1 || m == 3)
    {
        for (int i = 0; i < image->h; i++)
        {
            for (int j = 0; j < image->w; j++)
            {
                // 1: set grayscale
                if (m == 1)
                {
                    SDL_Color color;
                    SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                    Uint8 grey = (color.r + color.g + color.b) / 3;

                    pixels[i * image->w + j] = SDL_MapRGB(image->format, grey, grey, grey);
                }

                // 3: remove impurity
                else
                {
                    SDL_Color color;
                    SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                    pixels[i * image->w + j] = impurity(image, pixels, i, j);
                }
            }
        }
    }

    else
    {
        if (m == 2)
        {
            // 2: set black and white
            for (int i = 0; i < image->h; i++)
            {
                for (int j = 0; j < image->w; j++)
                {
                    SDL_Color color;
                    SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                    pixels[i * image->w + j] = bnw(image, color.r, color.g, color.b);
                }
            }
        }

        else
        {
            // 4: Contrast
            for (int i = 0; i < image->h; i++)
            {
                for (int j = 0; j < image->w; j++)
                {
                    SDL_Color color;
                    SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                    float coef = 1;

                    color.r = contrast(color.r, coef);
                    color.g = contrast(color.g, coef);
                    color.b = contrast(color.b, coef);

                    pixels[i * image->w + j] = SDL_MapRGB(image->format, color.r, color.g, color.b);
                }
            }
        }
    }
    //End: pixel manipulation
    
    //Save result file
    SDL_SaveBMP(image, "cache/result.bmp"); 

    SDL_Quit();

    return 0;
}


    


