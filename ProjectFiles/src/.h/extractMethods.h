#ifndef EXTRACTMETHODS_H_
#define EXTRACTMETHODS_H_

int split_x(SDL_Surface *img, int init, int dest, char* path);
SDL_Surface* split_y(SDL_Surface *img, int init, int dest, char* path);
void image_rotation(char *path, int angle);

#endif