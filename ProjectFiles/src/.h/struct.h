#ifndef STRUCT_h
#define STRUCT_h

struct NN
{
    int numInputs; //16*16
    int numHiddenNodes; // 16
    int numOutputs; // 75

    double characterMatrix[16][16];
    int objective[75];

    double hiddenWeights[16*16][16];
    double outputWeights[16][76];

    double hiddenLayerBias[16];
    double outputLayerBias[75];
    
    double hiddenLayerValue[16][16];
    double outputLayerValue[16][75];

    double updatedHiddenWeigths[16*16][16];
    double updatedOutputWeigths[16][75];

    double deltaHidden[16];
    double deltaOutput[75];

    double MaxErrorRate;
    double ErrorRate;
    double eta;
    double alpha;

    char *str;
};

#endif