#include <stdio.h>
#include <gtk/gtk.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../.h/extractMethods.h"
#include "../.h/matrice.h"
#include "../.h/NN.h"

char array_char_4[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '!', '"', '$', '%', '&', '\'', '(', ')', ',', '.', ':', ';', '?', '@'};

// Block extraction
void extractB(char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *image = IMG_Load(argv[0]); //Load image

    //Begin: pixel manipulation

    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	Uint32 * pixels = (Uint32 *)image->pixels;


    // initialisation 
	int c = 0;
	int n = 0;
	int init_x;
	int dest_x;
	int padding = 10;
	bool init = false;
	bool extract = false;

    // count black pixels
	for (int i = 0; i < image->h; i++)
        {
        	int countBP = 0;

            for (int j = 0; j < image->w; j++)
            {               
            	SDL_Color color;
                SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                // if pixels[i * image->w + j] is black, we increment countBP
                if (color.r == 0 && color.g == 0 && color.b == 0)
                	countBP++;                    
            }

            if(countBP >= 60 && !init)
            {
            	init = true;

            	if(i - padding < 0)
            		init_x = 0;
            	else
            		init_x = i-padding;
            }

            // if we encounter a line with more 10 black pixels, it's a line with words.
            // then c is reset to 0
            if(countBP > 10)
            	c = 0;

            // if there are less then 10 pixels in a line, maybe it's the end of a block
            if ((countBP <= 10 && init))
            {
                // if we entered the same case 15 times in a row, it's the end of a block
            	if (c > 20 || (i == image->h - 1))
            	{
            		init = false;
            		dest_x = i;
            		extract = true;
            		c = 0;
            	}
            	c++;
            }

            // extract action
            if (extract)
            	{
            		char path[] = "cache/blocs/extract_B";
            		char number[500];
            		char ext[] = ".bmp";

            		sprintf(number, "%d", n);

            		strcat(path, number);  
            		strcat(path, ext);

            		split_x(image, init_x, dest_x, path);
            		extract = false;

            		n++;            		
            	}
        }	
}
// Lines extraction (similar to extractB)
void extractL(char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *image = IMG_Load(argv[0]); //Load image

    //Begin: pixel manipulation

    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	Uint32 * pixels = (Uint32 *)image->pixels;

	int c = 0;
	int n = 0;
	int init_x;
	int dest_x;
	int padding = 5;
	bool init = false;
	bool extract = false;


	for (int i = 0; i < image->h; i++)
        {
        	int countBP = 0;

            for (int j = 0; j < image->w; j++)
            {               
            	SDL_Color color;
                SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                if (color.r == 0 && color.g == 0 && color.b == 0)
                	countBP++;                    
            }

            if(countBP >= 60 && !init)
            {
            	init = true;
            	init_x = i-padding;

                if(init_x < 0)
                    init_x = 0;
            }

            if(countBP > 10)
            	c = 0;

            if ((countBP <= 5 && init))
            {
            	if (c >= 0 || (i == image->h - 1))
            	{
            		init = false;
            		i += padding/2;
            		dest_x = i;
            		extract = true;
            		c = 0;
            	}
            	c++;
            }

            if (extract)
            	{
            		char path[] = "cache/lines/extract_L";
            		char number[500];
            		char ext[] = ".bmp";

            		sprintf(number, "%d", n);

            		strcat(path, number);  
            		strcat(path, ext);

            		split_x(image, init_x, dest_x, path);
            		extract = false;
            		n++;         		
            	}
        }	
}
// Word extraction (similar to extractB)
void extractW(char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *image = IMG_Load(argv[0]); //Load image

    //Begin: pixel manipulation

    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	Uint32 * pixels = (Uint32 *)image->pixels;

	int c = 0;
	int init_y;
	int n = 0;
	int dest_y;
	int padding = 3;
	bool init = false;
	bool extract = false;


	for (int j = 0; j < image->w; j++)
        {
        	int countBP = 0;

            for (int i = 0; i < image->h; i++)
            {               
            	SDL_Color color;
                SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                if (color.r == 0 && color.g == 0 && color.b == 0)
                	countBP++;                    
            }

            if(countBP >= 2 && !init)
            {
            	init = true;
            	if(j - padding < 0)
            		init_y = 0;
            	else
            		init_y = j-padding;
            }

            if(countBP > 2)
            	c = 0;

            if ((countBP == 0 && init))
            {
            	if (c > 2 || (j == image->w - 1))
            	{
            		init = false;
            		dest_y = j;
            		extract = true;
            		c = 0;
            	}
            	c++;
            }

            if (extract)
            	{
            		char path[] = "cache/words/extract_W";
            		char number[500];
            		char ext[] = ".bmp";

            		sprintf(number, "%d", n);

            		strcat(path, number);  
            		strcat(path, ext);

            		split_y(image, init_y, dest_y, path);
            		extract = false;

            		n++;            		
            	}
        }	
}

// Characters extraction (similar to extractB)
void extractC(char* argv[], GtkTextBuffer *buffer, struct NN *net)
{
	SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *image = IMG_Load(argv[0]); //Load image

    //Begin: pixel manipulation

    SDL_Surface * originalImage = image;
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(originalImage);

	Uint32 * pixels = (Uint32 *)image->pixels;

	int c = 0;
	int n = 0;
	int init_y;
	int dest_y;
	int padding = 1;
	bool init = false;
	bool extract = false;

	for (int j = 0; j < image->w; j++)
        {
        	int countBP = 0;

            for (int i = 0; i < image->h; i++)
            {               
            	SDL_Color color;
                SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

                if (color.r == 0 && color.g == 0 && color.b == 0)
                	countBP++;                    
            }

            if(countBP > 0 && !init)
            {
            	init = true;
            	if(j - padding < 0)
            		init_y = 0;
            	else
            		init_y = j-padding;
            }

            if(countBP > 2)
            	c = 0;

            if ((countBP == 0 && init))
            {
            	if (c >= 0 || (j == image->w - 1))
            	{
            		init = false;
            		dest_y = j;
            		extract = true;
            		c = 0;
            	}
            	c++;
            }

            if (extract)
            	{
            		char path[] = "cache/characters/extract_C";
            		char number[500];
            		char ext[] = ".bmp";

            		sprintf(number, "%d", n);

            		strcat(path, number);  
            		strcat(path, ext);

					//split_y(image, init_y, dest_y + padding, path);

            		SDL_Surface* charImg = split_y(image, init_y, dest_y + padding, path);

					int len = 16;
					int array[len][len];

					//fill the array with 0
					for(int v = 0; v < len; v++)
					{
						for(int x = 0; x < len; x++)
						{
							array[v][x] = 0;
						}
					}

					binary_array(charImg, len, array);

					double array_2[len][len];

					//fill the array with 0
					for(int v = 0; v < len; v++)
					{
						for(int x = 0; x < len; x++)
						{
							array_2[v][x] = (double) array[v][x];
						}
					}

					int value = nn(net, len, array_2);

					char c = array_char_4[value];
					printf("value in extraction.c: %i\n", value);
					printf("c: %c\n", c);
					char *charac = &c;

                    gtk_text_buffer_insert_at_cursor(buffer, charac, 1); 

					//write_matrice(len, array);

					//printf("%ld\n", sizeof(buffer));
					//printf("%ld\n", sizeof(net));

            		extract = false; 
					n++;   
    		
            	}
        }
}

