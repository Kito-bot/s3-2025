#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "weight_save_load.h"
#include "matrice_operations.h"
//#include "neural_network_operations.h"


double sigmoid(double x);
double dSigmoid(double x);
double init_weight();
void shuffle(unsigned long *array,unsigned long n);
double * init_training_inputs(void);
double * init_training_outputs(void);
unsigned long * init_trainingsetorder(unsigned long numTrainingSets);

double sigmoid(double x){return 1 / (1 + exp(-x));} //the sigmoid function

//double dsigmoid (double x){return (x * (1 - x));} //the derivative of the sigmoid fun

double init_weight(){return ((double)rand())/((double)RAND_MAX);} //starting with arbitrary wheights

void shuffle(unsigned long *array, unsigned long n) //a suffle function
{
    if (n > 1)
    {

        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            unsigned long t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
        
    }
}

double * init_training_inputs(void) //for xor input
{
    unsigned long numTrainingSets = 4, numInputs = 2;
    double *arr = malloc(sizeof(double) * numTrainingSets * numInputs);
    *(arr + 0 * numInputs + 0) = 0.0f;
    *(arr + 0 * numInputs + 1) = 0.0f;

    *(arr + 1 * numInputs + 0) = 1.0f;
    *(arr + 1 * numInputs + 1) = 0.0f;

    *(arr + 2 * numInputs + 0) = 0.0f;
    *(arr + 2 * numInputs + 1) = 1.0f;

    *(arr + 3 * numInputs + 0) = 1.0f;
    *(arr + 3 * numInputs + 1) = 1.0f;

    return arr;   
}

double *init_training_outputs(void) //for xor output
{
    int numTrainingSets = 4, numOutputs = 1;
    double *arr = malloc(sizeof(double) * numTrainingSets * numOutputs);
    *(arr + 0 * numOutputs) = 0.0f;
    *(arr + 1 * numOutputs) = 1.0f;
    *(arr + 2 * numOutputs) = 1.0f;
    *(arr + 3 * numOutputs) = 0.0f;
    
    return arr;
}

unsigned long * init_trainingsetorder(unsigned long numTrainingSets)
{
    unsigned long *arr = malloc(sizeof(unsigned long)*numTrainingSets);
    for (unsigned long i = 0; i < numTrainingSets; i++)
    {
        arr[i] = i;
    }
    return arr;
    
}

int main(void) //for xor training
{   
    unsigned long numInputs = 2;
    unsigned long numHiddenNodes = 2;
    unsigned long numOutputs = 1;
    
    double lr = 0.1f;
    
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

    unsigned long numTrainingSets = 4;
    
    //double training_inputs[numTrainingSets][numInputs] = 
    //{ {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} }; 
    //double training_outputs[numTrainingSets][numOutputs] = 
    //{ {0.0f},{1.0f},{1.0f},{0.0f} };
    double *training_inputs = init_training_inputs();
    double *training_outputs = init_training_outputs();

    //============================================= INIT WHEIGHTS ARBITRARLY
    for (unsigned long i=0; i<numInputs; i++) 
    {
        for (unsigned long j=0; j<numHiddenNodes; j++)
         {
            //hiddenWeights[i][j] = init_weight();
            //*(hiddenWheights + i * numHiddenNodes + j ) = init_weight();
            hiddenWheights[i*numHiddenNodes+j] = init_weight();
        }
    }
    for (unsigned long i=0; i<numHiddenNodes; i++) 
    {
        hiddenLayerBias[i] = init_weight();
        for (unsigned long j=0; j<numOutputs; j++) 
        {
            //outputWeights[i][j] = init_weight();
            //*(outputWeights + i * numOutputs + j) = init_weight();
            outputWeights[i*numOutputs+j] = init_weight();
        }
    }

    for (unsigned long i=0; i<numOutputs; i++) {
        //outputLayerBias[i] = init_weight();
        //*(outputLayerBias + i * 1 + 0) = init_weight();
        outputLayerBias[i] = init_weight();
    }
    
    // ===========================================
    
    //int trainingSetOrder[] = {0,1,2,3}; //can expand to numTrainingSets - 1
    unsigned long *trainingSetOrder = init_trainingsetorder(numTrainingSets);

    // 1 400 700 10000 10000000
    printf("training:\n");
    for (unsigned long n=0; n < 1000; n++) 
    {
        shuffle(trainingSetOrder,numTrainingSets);
        for (unsigned long x=0; x<numTrainingSets; x++) 
        {
        
            unsigned long i = trainingSetOrder[x];
            // Forward pass
            
            for (unsigned long j=0; j<numHiddenNodes; j++) 
            {
                double activation=hiddenLayerBias[j];
                for (unsigned long k=0; k<numInputs; k++) 
                {
                    //activation+=training_inputs[i][k]*hiddenWeights[k][j];
                    activation += 
                        ((training_inputs[i*numInputs + k])*
                         (hiddenWheights[k*numHiddenNodes+j]));//ok
                }
                hiddenLayer[j] = sigmoid(activation);
            }
            
            for (unsigned long j=0; j<numOutputs; j++) 
            {
                double activation=outputLayerBias[j];
                for (unsigned long k=0; k<numHiddenNodes; k++) 
                {
                    //outputWeights[k][j]
                    activation+=(hiddenLayer[k]*(outputWeights[k*numOutputs + j]));//ok
                }
                outputLayer[j] = sigmoid(activation);
            }
            
            /*printf("Input: %f,%f Output: %f Erxpected_output: %f\n",training_inputs[i*numInputs],
                    training_inputs[i*numInputs+1],
                    outputLayer[0],training_outputs[i*numOutputs]);*/
            if (n % 150){
                printf("|");
            }
            // Backprop
            
            //double deltaOutput[numOutputs];
            double *deltaOutput = malloc(sizeof(double) * numOutputs);
            for (unsigned long j=0; j<numOutputs; j++) 
            {
                //double errorOutput = (training_outputs[i][j]-outputLayer[j]);
                double errorOutput = (training_outputs[i*numOutputs+j]-outputLayer[j]);//ok
                //deltaOutput[j] = errorOutput*dSigmoid(outputLayer[j]);
                deltaOutput[j] = errorOutput*((outputLayer[j])*(1 - outputLayer[j]));
            }
            
            //double deltaHidden[numHiddenNodes];
            double *deltaHidden = malloc(sizeof(double) * numHiddenNodes);
            for (unsigned long j=0; j<numHiddenNodes; j++) 
            {
                double errorHidden = 0.0f;
                for(unsigned long k=0; k<numOutputs; k++) 
                {
                    //errorHidden+=deltaOutput[k]*outputWeights[j][k];
                    errorHidden += (deltaOutput[k]*(outputWeights[j*numOutputs+k]));//ok                
                }
                //deltaHidden[j] = errorHidden*dSigmoid(hiddenLayer[j]);
                deltaHidden[j] = errorHidden*((hiddenLayer[j])*(1 - hiddenLayer[j]));
            }
            // updates weightsetbias ----->
            for (unsigned long j=0; j<numOutputs; j++) 
            {
                outputLayerBias[j] += deltaOutput[j]*lr;
                for (unsigned long k=0; k<numHiddenNodes; k++) 
                {
                    //outputWeights[k][j]+=hiddenLayer[k]*deltaOutput[j]*lr;
                    outputWeights[k*numOutputs+j] += (hiddenLayer[k]*deltaOutput[j]*lr); //ok
                }
            }
            
            for (unsigned long j=0; j<numHiddenNodes; j++) 
            {
                hiddenLayerBias[j] += deltaHidden[j]*lr;
                for(unsigned long k=0; k<numInputs; k++) 
                {
                    //hiddenWeights[k][j]+=training_inputs[i][k]*deltaHidden[j]*lr;
                    hiddenWheights[k*numHiddenNodes+j] +=
                        (training_inputs[i*numInputs+k]*deltaHidden[j]*lr); //ok
                }
            }
            
            free(deltaOutput);
            free(deltaHidden);
        }
    }
    
    save_weights(hiddenWheights,numInputs,numHiddenNodes,"xor_hiddenWeights");
    save_weights(outputWeights,numHiddenNodes,numOutputs,"xor_outputWeight");
    save_weights(hiddenLayerBias,1,numHiddenNodes,"xor_hiddenLayerBias");
    save_weights(outputLayerBias,1,numOutputs,"xor_outputLayerBias");

    free(hiddenLayer);
    free(outputLayer);
    free(hiddenLayerBias);
    free(outputLayerBias);
    free(hiddenWheights);
    free(outputWeights);
    free(training_inputs);
    free(training_outputs);
    free(trainingSetOrder);
    
    return EXIT_SUCCESS;
}