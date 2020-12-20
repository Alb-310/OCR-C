//
//  NN.h
//  OCR
//
//  Created by Adrien Hicaubert on 23/11/2020.
//

#ifndef NN_h
#define NN_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>
#include <time.h>
#include "../.h/struct.h"

struct NN* InitializeNetwork();

char TRAINING_NN(struct NN *net, int len, double input[len][len], double goal[]);
int nn(struct NN *net, int len, double matrice[len][len]);
void print_NN(struct NN *net);

#endif /* NN_h */
