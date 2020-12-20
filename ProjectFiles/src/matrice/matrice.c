#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TAILLE_MAX 1000 // Tableau de taille 1000

char array_char[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '!', '"', '$', '%', '&', '\'', '(', ')', ',', '.', ':', ';', '?', '@'};

char* array_police[] = {"arial", "calibri", "tnr", "apple-c", "bradley-h", "courier", "comic"};

void print_array_double(int len, double array[len][len])
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			printf("%i", (int) array[i][j]);
		}
		printf("\n");	
	}

	printf("\n");		
	
}

void print_array_int(int len, int array[len][len])
{
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			printf("%d", array[i][j]);
		}
		printf("\n");	
	}

	printf("\n");		
	
}

void read_matrice(int len, int array[len][len], int index)
{
    FILE* file = NULL;
    file = fopen("../matrice/set-matrices.txt", "r");
    int current_char = 0;
    int c = -1;
    
    if (file != NULL)
    {
        // Boucle de lecture d'une des matrices
        do
        {
            current_char = fgetc(file);

            if(current_char == 'x')
            {
                c++;
                fgetc(file);
                if (index == c)
                {
                    for(int i = 0; i < len; i++)
                    {
                        for(int j = 0; j < len + 1; j++)
                        {
                            int digit = fgetc(file);
                            
                            if (isdigit(digit))
                            {
                                array[i][j] = digit - '0';
                            }                                                      
                        }
                    }
                    break;
                }
                
            } 

        } while (current_char != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
 
        fclose(file);
    }

    else
    {
        printf("Erreur");
    }
    
}

int search(char character)
{
    for(int i = 0; i < 77; i++)
    {
        if(array_char[i] == character)
            return i + 1;
    }

    return -1;
}

void get_matrice(char letter, char* police, int len, int array[len][len])
{
    // [0-76] [77-153] [154-230] [307-384] [385-461] [462-538]

    int index = -1;
    int character = search(letter);

    if (strcmp(police, "arial") == 0)
        index = 0;  

    else if (strcmp(police, "calibri") == 0)
        index = 1;       

    else if (strcmp(police, "tnr") == 0)
        index = 2; 

    else if (strcmp(police, "apple-c") == 0)
        index = 3; 

    else if (strcmp(police, "bradley-h") == 0)
        index = 4; 

    else if (strcmp(police, "courier") == 0)
        index = 5; 

    else if (strcmp(police, "comic") == 0)
        index = 6; 
    
    if (index != -1 || character != -1)
    {
        int new_index = character + (77 * index);
        read_matrice(len, array, new_index);
    }

    else
    {
        printf("Police or letter specified do not exist.");
    }
}

int c = 0;

void write_matrice(int len, int array[len][len])
{
    FILE* fichier = NULL;
    fichier = fopen("cache/set-matrices.txt", "a");

    fputc('x', fichier); 
    fputc('\n', fichier); 
    c++;  
    for(int i = 0; i < len; i++)
    {
        for(int j = 0; j < len; j++)
        {
            fprintf(fichier, "%d", array[i][j]);
        }

        fputc('\n', fichier);
    }

    fclose(fichier);
}
 
void binary_array(SDL_Surface* image, int len, int array[len][len])
{
    SDL_Init(SDL_INIT_VIDEO);

    //Binarisatprintf("%ld\n", sizeof());ion of the image in a 2D Array
    Uint32 * pixels = (Uint32 *)image->pixels;

    int firstB = -1;
    int pw = (len/2) - (image->w/2);

    if(pw < 0)
        pw = 0;

    for (int a = 0; a < len; a++)
    {
        for(int b = 0; b < len; b++)
        {
            SDL_Color color;
            SDL_GetRGB(pixels[a * image->w + b], image->format, &color.r, &color.g, &color.b);

            if (color.r == 0 && color.g == 0 && color.b == 0)
            {
                firstB = a;
                break;
            }
        }

        if(firstB != -1)
            break;
    }

    int imageH = 0;

    for(int i = 0; i < image->h; i++)
    {
        for(int j = 0; j < image->w; j++)
        {
            SDL_Color color;
            SDL_GetRGB(pixels[i * image->w + j], image->format, &color.r, &color.g, &color.b);

            if (color.r == 0 && color.g == 0 && color.b == 0)
            {
                imageH++;
                break;
            }
        }
    }

    int ph = (len/2) - (imageH/2 + 1);   
    if(ph < 0)
        ph = 0;

     for (int i = 0 ; i < len; i++)
     {
        for (int j = 0; j < len; j++)
        {
            if(j > image->w || i > image->h || j+pw > len || i+ph > len)
                break;

            SDL_Color color;

            int x = i + firstB;

            if(x > image->h)
                break;

            SDL_GetRGB(pixels[x * image->w + j], image->format, &color.r, &color.g, &color.b);

            if (color.r == 0 && color.g == 0 && color.b == 0)
                array[i+ph][j+pw] = 1;
        }

    }

    //End: pixel manipulation
    SDL_Quit();
}