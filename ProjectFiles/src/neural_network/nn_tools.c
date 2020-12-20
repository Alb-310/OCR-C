//
//  nn_tools.c
//  OCR
//
//  Created by Adrien Hicaubert on 30/11/2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "../.h/NN.h"

// Shuffle function, useful for random traning sets choices
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}


// Sigmoid function, optimized for our XOR NN, but not for the OCR NN (we will use Softmax then)
double sigmoid(double z){return 1 / (1 + exp(-z));}
double dSigmoid(double z){return z * (1 - z);} // Sigmoide derivative, useful for back-propagation later

// Function that returns a random number between 0 and 1
double ran_num(){return((double)rand())/((double)RAND_MAX);}


// Returns the position of the biggest output value
int output_pos(struct NN *net)
{
     double max = 0;
     int posMax = 0;
     for (int j = 0; j < net -> numHiddenNodes; j++)
     {
      for (int o = 0; o < net -> numOutputs; o++)
      {
        //printf("output: %f\n", net -> outputLayerValue[j][o]);
        printf("outputLayerValue[%i]: %f\n", o, net -> outputLayerValue[j][o]);
        if (max < net -> outputLayerValue[j][o])
        {
          posMax = o;
          max = net -> outputLayerValue[j][o];
        }
      }
     }
      return posMax;
}


char RetrieveChar(int val)
{
  char c;

  if(val <= 25)
  {
    c = val + 65;
  }
  else if(val > 25 && val <= 51)
  {
    c = val + 97 - 26;
  }
  else if(val > 51 && val <= 61)
  {
    c = val + 48 - 52;
  }
  else
  {
    switch(val)
    {
      case 62:
        c = '!';
        break;
      case 63:
        c = '"';
        break;
      case 64:
        c = '$';
        break;
      case 65:
        c = '%';
        break;
      case 66:
        c = '&';
        break;
      case 67:
        c = '\'';
        break;
      case 68:
        c = '(';
        break;
      case 69:
        c = ')';
        break;
      case 70:
        c = ',';
        break;
      case 71:
        c = '.';
        break;
      case 72:
        c = ':';
        break;
      case 73:
        c = ';';
        break;
      case 74:
        c = '?';
        break;
      case 75:
        c = '@';
        break;
      default:
        exit(1);
        break;
    }
  }
  return c;
}

void saveData(struct NN *net)
{
  FILE* weightIH = fopen("training_files/weightIH.w", "w");
  for(int i = 0; i < net -> numInputs; ++i)
  {
    for(int j = 0; j < net -> numHiddenNodes; ++j)
    {
      fprintf(weightIH, "%f\n", net -> hiddenWeights[i][j]);
    }
  }
  fclose(weightIH);

  FILE* weightHO = fopen("training_files/weightHO.w", "w");
  for(int j = 0; j < net -> numHiddenNodes; ++j)
  {
    for(int k = 0; k < net -> numOutputs; ++k)
    {
      fprintf(weightHO, "%f\n",net -> outputWeights[j][k]);
    }
  }
  fclose(weightHO);

  FILE* biasH = fopen("training_files/biasH.b", "w");
  for(int j = 0; j < net -> numHiddenNodes; ++j)
  {
    fprintf(biasH, "%f\n", net -> hiddenLayerBias[j]);
  }
  fclose(biasH);

  FILE* biasO = fopen("training_files/biasO.b", "w");
  for (int k = 0; k < net -> numOutputs; ++k)
  {
    fprintf(biasO, "%f\n",net -> outputLayerBias[k]);
  }
  fclose(biasO);
}

struct NN* ExtractData ()
{
  //CREATE NN
  struct NN *nn = malloc(sizeof(struct NN));
  nn -> numInputs = 16; //size of imgs
  nn -> numHiddenNodes = 16;
  nn -> numOutputs = 75; 
  nn -> str = malloc(sizeof(char)*1200);
  nn -> str = "\0";

  int sizeMax = 15;
  char *line = calloc(15, sizeof(char));
  //WeightIH
  FILE* weightIH = fopen("src/neural_network/training_files/weightIH.w", "r");
  for(int i = 0; i < nn -> numInputs; ++i)
  {
    for(int h = 0; h < nn -> numHiddenNodes; ++h)
    {
      fgets(line, sizeMax, weightIH);
      strtok(line, "\n");
      nn -> hiddenWeights[i][h] = atof(line);
    }
  }
  fclose(weightIH);

  //Weight HO
  FILE* weightHO = fopen("src/neural_network/training_files/weightHO.w", "r");
  for(int h = 0; h < nn -> numHiddenNodes; ++h)
  {
    for(int o = 0; o < nn -> numOutputs; ++o)
    {
        fgets(line, sizeMax, weightHO);
        strtok(line, "\n");
        nn -> outputWeights[h][o] = atof(line);
    }
  }
  fclose(weightHO);
  
  FILE* biasH = fopen("src/neural_network/training_files/biasH.b", "r");
    for(int j = 0; j < nn -> numHiddenNodes; ++j)
    {
        fgets(line,sizeMax,biasH);
        strtok(line, "\n");
        nn->hiddenLayerBias[j]=atof(line);
    }
    fclose(biasH);
  
   FILE* biasO = fopen("src/neural_network/training_files/biasO.b", "r");
    for (int k = 0; k < nn -> numOutputs; ++k)
    {
        fgets(line,sizeMax,biasO);
        strtok(line, "\n");
        nn->outputLayerBias[k]=atof(line);
    }
    fclose(biasO);

    return nn;
}
