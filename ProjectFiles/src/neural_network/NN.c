//
//  NN.c
//  OCR
//
//  Created by Adrien Hicaubert on 23/11/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../.h/nn_tools.h"
#include "../.h/struct.h"

char array_char_3[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '!', '"', '$', '%', '&', '\'', '(', ')', ',', '.', ':', ';', '?', '@'};

void print_NN(struct NN *net)
{
  printf("STATE: NN\n-------------\n");
  printf("hiddenWeights:\n");
  for(int i = 0; i < net -> numInputs; ++i)
  {
    for(int j = 0; j < net -> numHiddenNodes; ++j)
    {
      printf("%f | ", net -> hiddenWeights[i][j]);
    }
    printf("\n");
  }

  printf("\n");
  printf("outputWeights:\n");
  for(int j = 0; j < net -> numHiddenNodes; ++j)
  {
    for(int k = 0; k < net -> numOutputs; ++k)
    {
      printf("%f | ",net -> outputWeights[j][k]);
    }
    printf("\n");
  }

  printf("\n");
  printf("hiddenLayerBias:\n");
  for(int j = 0; j < net -> numHiddenNodes; ++j)
  {
    printf("%f | ", net -> hiddenLayerBias[j]);
  }

  printf("\n");
  printf("outputLayerBias:\n");
  for (int k = 0; k < net -> numOutputs; ++k)
  {
    printf("%f | ", net -> outputLayerBias[k]);
  }

  printf("\n");
}

struct NN* InitializeNetwork()
{
  struct NN *net = NULL;
  net = malloc(sizeof(struct NN));
  net -> numInputs = 16;
  net -> numHiddenNodes = 16;
  net -> numOutputs = 75;
  net -> eta = 0.5;
  net -> alpha = 0.6;

  // We fill the matrixes and arrays with random values
  for (int i = 0; i < net -> numInputs; i++) {
        for (int j = 0; j < net -> numHiddenNodes; j++) {
            net -> hiddenWeights[i][j] = ran_num();
	    net -> deltaHidden[j] = 0.0;
	    net -> hiddenLayerBias[j] = ran_num();
        }
    }

  for (int j = 0; j < net -> numHiddenNodes; j++) {
        for (int k = 0; k < net -> numOutputs; k++) {
            
            net -> outputWeights[j][k] = ran_num();
	    net -> deltaOutput[k] = 0.0;
	    net -> outputLayerBias[k] = ran_num();
        }
    }

  return net;
}



static void forwardPropagation(struct NN *net){
    double activation, weight, input, bias/*, eta*, layerValue*/;
    //eta = net -> eta;

    // Logistic regression, we calculate the hidden layer value (y) adding all inputs multiplied by their weights and we add the biais
    // First we do it between Inputs and Hidden Layer

    for (int j = 0; j < net -> numHiddenNodes; j++) {
        activation = 0;
        for (int i = 0; i < net -> numInputs; i++) {
            weight = net -> hiddenWeights[i][j];
            input = net -> characterMatrix[j][i];
            activation += weight * input; // We take each input with his associated weight
	    bias = net -> hiddenLayerBias[j];
            net -> hiddenLayerValue[i][j] = sigmoid(activation + bias);
        }
    }
         
    // Then for the outputs
    for (int k = 0; k < net -> numOutputs; k++) {
        activation = 0;
        for (int j = 0; j < net -> numHiddenNodes; j++) {
            weight = net -> outputWeights[j][k];
	    for (int i = 0; i < net -> numInputs; i++) {
            	input = net -> hiddenLayerValue[i][j];
	    }
            activation += weight * input;
	    bias = net -> outputLayerBias[k];
            net -> outputLayerValue[j][k] = sigmoid(activation + bias);
        }
    }
}

static void backwardPropagation(struct NN *net)
{
  // Correct the network errors by modifying the weights
  // For outputs
  double output, hidden, derivative, goal, error, errorHidden, weight, delta, mse;
  for (int k = 0; k < net -> numOutputs; k++)
  {
    for (int j = 0; j < net -> numHiddenNodes; j++)
    {
   	 output = net -> outputLayerValue[j][k];
    }
    derivative = dSigmoid(output);
    goal = net -> objective[k];
    error = fabs(goal - output);
    mse = error * derivative;
    net -> deltaOutput[k] = mse;
  }

  // For hidden layer
  for (int j = 0; j < net -> numHiddenNodes; j++)
  {
    errorHidden = 0;
    for (int k = 0; k < net -> numOutputs; k++)
    {
      weight = net -> outputWeights[j][k];
      delta = net -> deltaOutput[k];

      errorHidden += weight * delta;
      for(int i = 0; i < net -> numInputs; i++)
      {
      	hidden = net -> hiddenLayerValue[i][j];
      }
      derivative = dSigmoid(hidden);
      net -> deltaHidden[j] = errorHidden * derivative;
    }
  }
}

static void UpdateWeights(struct NN *net)
{
  double  alpha, error1, output1, error2, output2;
  alpha = net -> alpha;

  //Weights between Hidden and Ouput layers
  for (int k = 0; k < net -> numOutputs; k++)
  {
    for (int j = 0; j < net -> numHiddenNodes; j++)
    {
      for(int i = 0; i < net -> numInputs; i++)
      {
      	output2 = net -> hiddenLayerValue[i][j];
      }
      error2 = net -> deltaOutput[k];
      net -> outputWeights[j][k] += error2 * output2 * alpha;
    }
  }

   //Weights between Input and Hidden layers
  for (int j = 0; j < net -> numHiddenNodes; j++)
  {
    for(int i = 0; i < net -> numInputs; i++)
    {
      output1 = net -> characterMatrix[j][i];
      error1 = net -> deltaHidden[j];
      net -> hiddenWeights[i][j] += error1 * output1 * alpha;
    }
  }
}

static void UpdateBiases(struct NN *net)
{
  double alpha, delta, dBias;
  alpha = net -> alpha;
  //Update BiasH
  for (int j = 0; j < net -> numHiddenNodes; j++) {
      delta = net -> deltaHidden[j];
      dBias = alpha * delta;
      net -> hiddenLayerBias[j] += dBias;
  }

  //Update BiasO
  for (int k = 0; k < net -> numOutputs; k++)
  {
    delta = net -> deltaOutput[k];
    dBias = alpha * delta;
    net -> outputLayerBias[k] += dBias;
  }
}

char TRAINING_NN(struct NN *net, int len, double input[len][len], double goal[])
{
  for (size_t loops = 0; loops < 1000; loops++)
  {
    for (int g = 0; g < 75; g++)
    {
      net -> objective[g] = goal[g];
    }

    for(int j = 0; j < net -> numHiddenNodes; j++)
    {
      for(int i = 0; i < net -> numInputs; i++)
        {
          net -> characterMatrix[j][i] = input[j][i];
        }
    }

    forwardPropagation(net);
    int value = output_pos(net);
    backwardPropagation(net);
    UpdateWeights(net);
    UpdateBiases(net);

    return array_char_3[value];
  }
}

int nn(struct NN *net, int len, double matrice[len][len])
{  

  for(int j = 0; j < net -> numHiddenNodes; j++)
  {
    for(int i = 0; i < net -> numInputs; i++)
      {        
        net -> characterMatrix[j][i] = matrice[j][i];
      }
  }

  forwardPropagation(net); // forward

  int value = output_pos(net);
  printf("\n");

  printf("value in nn.c: %i\n", value);
  return value;
}
