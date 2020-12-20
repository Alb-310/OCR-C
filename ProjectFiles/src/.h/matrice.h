#ifndef MATRICE_H_
#define MATRICE_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void binary_array(SDL_Surface* path, int len, int array[len][len]);
void write_matrice(int len, int array[len][len]);
void get_matrice(char letter, char* police, int len, int array[len][len]);
void print_array_double(int len, double array[len][len]);
void print_array_int(int len, int array[len][len]);
int search(char character);

#endif