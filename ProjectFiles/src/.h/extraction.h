#ifndef EXTRACTION_H_
#define EXTRACTION_H_

#include <gtk/gtk.h>
#include "../.h/NN.h"
#include "../.h/struct.h"

void extractB(char* argv[]);
void extractL(char* argv[]);
void extractW(char* argv[]);
void extractC(char* argv[], GtkTextBuffer *buffer, struct NN *net);

#endif