#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "weight_save_load.h"
//#include "matrice_operations.h"
//#include "matrice_operations.h"
//#include "neural_network_operations.h"


double sigmoid(double x);
double dSigmoid(double x);
double init_weight();
void shuffle(unsigned long *array,unsigned long n);
double * init_training_inputs(void);
double * init_training_outputs(void);
unsigned long * init_trainingsetorder(unsigned long numTrainingSets);

double sigmoid(double x){return 1 / (1 + exp(-x));} //the sigmoid function

int main(int argc,char**argv)
{
    if (argc != 3)
        errx(1,"xor: bad arguments given");

    unsigned long numInputs = 2;
    unsigned long numHiddenNodes = 2;
    unsigned long numOutputs = 1;
    
    //double lr = 0.1f;
    
    //double hiddenLayer[numHiddenNodes];
    double *hiddenLayer = malloc(sizeof(double) * numHiddenNodes);
    //double outputLayer[numOutputs];
    double *outputLayer = malloc(sizeof(double) * numOutputs);

    //double hiddenLayerBias[numHiddenNodes];
    double *hiddenLayerBias = malloc(sizeof(double)* numHiddenNodes);
    //double outputLayerBias[numOutputs];
    double *outputLayerBias = malloc(sizeof(double) * numOutputs);

    //double hiddenWeights[numInputs][numHiddenNodes];
    double *hiddenWheights = malloc(sizeof(double) * numInputs * numHiddenNodes);
    //double outputWeights[numHiddenNodes][numOutputs];
    double *outputWeights = malloc(sizeof(double) * numHiddenNodes * numOutputs);
    
    double *training_inputs = malloc(sizeof(double) * numInputs); 

    //init all
    hiddenWheights = init_matrix_double(numInputs,numHiddenNodes);
    outputWeights = init_matrix_double(numHiddenNodes,numOutputs);
    hiddenLayerBias = init_matrix_double(1,numHiddenNodes);
    outputLayerBias = init_matrix_double(1,numOutputs);

/*  weight-save-load:

    load_weights(hiddenWheights,numInputs,numHiddenNodes,"xor_hiddenWeights");
    load_weights(outputWeights,numHiddenNodes,numOutputs,"xor_outputWeight");
    load_weights(hiddenLayerBias,1,numHiddenNodes,"xor_hiddenLayerBias");
    load_weights(outputLayerBias,1,numOutputs,"xor_outputLayerBias");*/
    
    double training_input1 = (double) strtoul(argv[1],NULL,10);
    double training_input2 = (double) strtoul(argv[2],NULL,10);

    training_inputs[0] = training_input1;
    training_inputs[1] = training_input2;

    //printf("input1: %f input2: %f\n",training_input1,training_input2);
//===================================================================== forward pass
    for (unsigned long j=0; j<numHiddenNodes; j++) 
    {
        double activation = hiddenLayerBias[j];
        for (unsigned long k=0; k<numInputs; k++) 
        {
            //activation+=training_inputs[i][k]*hiddenWeights[k][j];
            activation += 
                ((training_inputs[k])*(hiddenWheights[k*numHiddenNodes+j]));//ok
            //printf("hiddenWheights:%f\n",hiddenWheights[k*numHiddenNodes+j]);
        }
        hiddenLayer[j] = sigmoid(activation);
    }

    for (unsigned long j=0; j<numOutputs; j++) 
    {
        double activation = outputLayerBias[j];
        for (unsigned long k=0; k<numHiddenNodes; k++) 
        {
            //outputWeights[k][j]
            activation+=(hiddenLayer[k]*(outputWeights[k*numOutputs + j]));//ok
            //printf("outputWeights:%f\n",outputWeights[k*numOutputs + j]);
        }
        outputLayer[j] = sigmoid(activation);
    }
//=====================================================================

    printf("Input: %f,%f Output: %f\n",training_inputs[0],training_inputs[1],outputLayer[0]);

    free(hiddenLayer);
    free(outputLayer);
    free(hiddenLayerBias);
    free(outputLayerBias);
    free(hiddenWheights);
    free(outputWeights);
    //free(training_inputs);

    return EXIT_SUCCESS;
}