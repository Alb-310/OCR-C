//
//  nn_tools.h
//  OCR
//
//  Created by Adrien Hicaubert on 30/11/2020.
//

#ifndef nn_tools_h
#define nn_tools_h

#include <stdio.h>

void shuffle(int *array, size_t n);

double sigmoid(double z);
double dSigmoid(double z);

double ran_num();

int output_pos();

char RetrieveChar(int val);

struct NN* ExtractData ();

void saveData(struct NN *net);

#endif /* nn_tools_h */
