#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>
#include <string.h>
#include <SDL/SDL.h>

#include "weight_save_load.h"
#include "tools.h"
#include "network.h"
#include "../process/binarization.h"


/*double dsigmoid (double x){
    return x * (1 - x);
} //the derivative of the sigmoid fun*/
struct net *InitNet(unsigned int in, unsigned int hid, unsigned int out,char *file)
{
    struct net *network = malloc(sizeof(struct net));
    if (network == NULL)
    {
        errx(1,"Not enough memory!");
    } 
    
    network->numInputs = in;
    network->numHiddenNodes = hid;
    network->numOutputs = out;
    network->numTrainingSets = 62; 
    network->inputLayer = calloc(network->numInputs,sizeof(double));

    network->hiddenLayer = calloc(network->numHiddenNodes,sizeof(double));
    network->deltaHidden = calloc(network->numHiddenNodes,sizeof(double));
    network->hiddenLayerBias = calloc(network->numHiddenNodes,sizeof(double));
    network->hiddenWeights = calloc(network->numInputs * network->numHiddenNodes,sizeof(double));
    network->deltaHiddenWeights = calloc(network->numInputs * network->numHiddenNodes,sizeof(double));

    network->outputLayer = calloc(network->numOutputs,sizeof(double));
    network->deltaOutput = calloc(network->numOutputs,sizeof(double));
    network->outputLayerBias = calloc(network->numOutputs,sizeof(double));
    network->outputWeights = calloc(network->numHiddenNodes * network->numOutputs,sizeof(double));
    network->deltaOutputWeights = calloc(network->numHiddenNodes * network->numOutputs,sizeof(double));

    network->expected = calloc(network->numOutputs,sizeof(double));
    network->phi = 0.5f;
    network->alpha = 0.9f;

    
    //network->trainingSetOrder = initTrainingSetOrder(network->numTrainingSets);

    // deal with that ------------>
    FILE *tmp = fopen(file,"r");
    if(tmp != NULL)
    {
        fclose(tmp);
        load_net(network,file);
    }
    else
    {
        //fclose(tmp);
        initialization(network);
    }
    //fclose(tmp);
    return network;
}

void initialization(struct net *network)
{
    for (size_t i=0; i<(size_t)network->numInputs; i++) 
    {
        for (size_t j=0; j<(size_t)network->numHiddenNodes; j++)
        {
            //hiddenWeights[i][j] = init_weight();
            //*(hiddenWheights + i * numHiddenNodes + j ) = init_weight();
            network->hiddenWeights[i*network->numHiddenNodes+j] = init_weight();
        }
    }
    for (size_t i=0; i<(size_t)network->numHiddenNodes; i++) 
    {
        network->hiddenLayerBias[i] = init_weight();
        for (size_t j=0; j<(size_t)network->numOutputs; j++) 
        {
            //outputWeights[i][j] = init_weight();
            //*(outputWeights + i * numOutputs + j) = init_weight();
            network->outputWeights[i*network->numOutputs+j] = init_weight();
            network->outputLayerBias[j] = init_weight();
        }
    }
}
//training_input = inputLayer
//void forward_pass(struct net *network,unsigned int order)
void forward_pass(struct net *network) //simple forward pass for simgle values char imput
{
    //size_t i; //= network->trainingSetOrder[order];
    for (size_t j=0; j<(size_t)network->numHiddenNodes; j++) 
    {
        double activation=network->hiddenLayerBias[j];
        for (size_t k=0; k<(size_t)network->numInputs; k++) 
        {
            //activation+=training_inputs[i][k]*hiddenWeights[k][j];
            //(network->inputLayer[i*network->numInputs + k])          
            //i = network->trainingSetOrder[order];
            activation += 
                ((network->inputLayer[k])*
                    (network->hiddenWeights[k*network->numHiddenNodes+j]));//ok
        
            
        }
        network->hiddenLayer[j] = sigmoid(activation);
    }
    
    for (size_t j=0; j<(size_t)network->numOutputs; j++) 
    {
        double activation=network->outputLayerBias[j];
        for (size_t k=0; k<(size_t)network->numHiddenNodes; k++) 
        {
            //outputWeights[k][j]
            activation += (network->hiddenLayer[k]*
            (network->outputWeights[k*network->numOutputs + j]));//ok
        }
        network->outputLayer[j] = sigmoid(activation);
    }
}

void back_propagation(struct net *network)
{
    //double deltaOutput[numOutputs];
    //double *deltaOutput = malloc(sizeof(double) * network->numOutputs);
    //size_t i ;//= network->trainingSetOrder[order];
    for (unsigned int o=0; o<network->numOutputs; o++) 
    {
        double errorOutput;
        //i = network->trainingSetOrder[order];
        //double errorOutput = (training_outputs[i][j]-outputLayer[j]);
        errorOutput = 
            (network->expected[o]-
                network->outputLayer[o]);//ok
        //printf("%lf\n",errorOutput);
        //deltaOutput[j] = errorOutput*dSigmoid(outputLayer[j]);
        network->deltaOutput[o] = 
            errorOutput*(network->outputLayer[o]*(1 - network->outputLayer[o]));
    }
    //printf("%c\n",'y');
    //double deltaHidden[numHiddenNodes];
    //double *deltaHidden = malloc(sizeof(double) * network->numHiddenNodes);
    for (unsigned int j=0; j <network->numHiddenNodes; j++) 
    {
        double errorHidden = 0.0f;
        for(unsigned int k=0; k<network->numOutputs; k++) 
        {
            //errorHidden+=deltaOutput[k]*outputWeights[j][k];
            errorHidden += (network->deltaOutput[k]*(network->outputWeights[j*network->numOutputs+k]));//ok
            //printf("%lf\n",errorHidden);
            //printf("%lf\n",network->deltaOutput[k]);
            //printf("%lf\n",network->outputWeights[j*network->numOutputs+k]); 
            //printf("%d\n",k);        
        }
        //deltaHidden[j] = errorHidden*dSigmoid(hiddenLayer[j]);
        network->deltaHidden[j] = errorHidden*(network->hiddenLayer[j]*(1 - network->hiddenLayer[j]));
    }
}

void updateweightsetbiases(struct net *network)
{
    for (size_t j=0; j<(size_t)network->numOutputs; j++) 
    {
        network->outputLayerBias[j] += network->deltaOutput[j] * network->phi;
        for (size_t k=0; k<(size_t)network->numHiddenNodes; k++) 
        {
            //outputWeights[k][j]+=hiddenLayer[k]*deltaOutput[j]*lr;
            network->outputWeights[k*network->numOutputs+j] += 
                (network->hiddenLayer[k]*network->deltaOutput[j]*network->phi) + 
                    (network->alpha * network->deltaOutputWeights[k*network->numOutputs+j]);
            
            network->deltaOutputWeights[k*network->numOutputs+j] = 
                network->phi * network->deltaOutput[j] * network->hiddenLayer[k];
        }
    }
    
    for (size_t j=0; j<(size_t)network->numHiddenNodes; j++) 
    {
        network->hiddenLayerBias[j] += network->deltaHidden[j]*network->phi;
        for(size_t k=0; k<(size_t)network->numInputs; k++) 
        {
            //hiddenWeights[k][j]+=training_inputs[i][k]*deltaHidden[j]*lr;
            network->hiddenWeights[k*network->numHiddenNodes+j] +=
                (network->expected[k]*network->deltaHidden[j]*network->phi) +
                    (network->alpha * network->deltaHiddenWeights[k*network->numHiddenNodes+j]); //ok
            
            network->deltaHiddenWeights[k*network->numHiddenNodes+j] = 
                (network->phi*network->deltaHidden[j]*network->inputLayer[k]);
        }
    }
}

void TrainNet(void)
{
    char *file = "ocr_save.txt";
    struct net *network = InitNet(30*30,40,62,file);
    char *filepath = "bin/00.bin\0";
    char expected_result[62] = {'0','1','2','3','4','5','6','7','8','9','a',
    'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t',
    'u','v','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R',
    'S','T','U','V','W','X','Y','Z'};
    int nb = 2500;
    for (size_t number = 0; number < (size_t)nb; number++)
    {
            for (size_t i = 0; i < 62; i++)
            {
                filepath = updatepath(filepath,(size_t)strlen(filepath),i);
                ExpectedOutput(network,expected_result[i]);
                //InputFromTXT(filepath,network);
                initTrainingInput(filepath,network);
                forward_pass(network);
                //size_t index = IndexRet(network);
                //printf("%c",'x');
                //char c = Retrieve(index);
                //printf("%c",'x');
                //PrintState(expected_result[i],c);
                back_propagation(network);
                //printf("%c",'x');
                updateweightsetbiases(network);
            }
    }
    save_net(network,file);
    freeNet(network);
}

void freeNet(struct net *network)
{
    free(network->inputLayer);
    free(network->hiddenLayer);
    free(network->deltaHidden);
    free(network->hiddenLayerBias);
    free(network->hiddenWeights);
    free(network->deltaHiddenWeights);
    free(network->outputLayer);
    free(network->deltaOutput);
    free(network->outputLayerBias);
    free(network->outputWeights);
    free(network->deltaOutputWeights);
    free(network);
}

char * updatepath(char *filepath,size_t len,size_t index)
{
    char *newpath = malloc(len*sizeof(char));
    if (index < 10)
    {
        for (size_t i = 0; i < len; i++) 
        {
            if (i != 4)
            {
                newpath[i] = filepath[i];
            }
            else
            {
                newpath[i] = (char)(index + 48);
            }
        }
    }
    else
    {
        size_t uni = index % 10;
        size_t dis = index / 10;

        for (size_t i = 0; i < len; i++) 
        {
            if (i != 5) 
            {
                newpath[i] = filepath[i];
            }
        }

        newpath[4] = (char)(dis + 48);
        newpath[5] = (char)(uni + 48);
    }

    return newpath;
}

void netOut(SDL_Surface *img) //just pour un char
{
    struct net *network = InitNet(30*30,40,62,"ocr_save.txt");
    int* mat = binarization(img);
    save_mat(mat,(size_t)30,(size_t)30,"tmp.bin");
    initTrainingInput("tmp.bin",network);
}

void initTrainingInput(char *fileName,struct net *network)
{
    FILE *file = fopen(fileName,"r");
    size_t max = 30*30;
    for (size_t i = 0; i < max; i++)
    {
        for (size_t j = 0; j < 1; j++)
        {
            int tmp;
            fscanf(file,"%d",&tmp);
            (network->inputLayer[i]) = (double)tmp;
        }
        fscanf(file,"\n");
    }
    fclose(file);
}

void ExpectedOutput(struct net *network,char c)
{
    if(c >= 'A' && c <= 'Z')
      network->expected[(int)(c) - 65] = 1;

    else if(c >= 'a' && c <= 'z')
      network->expected[((int)(c) - 97) + 26] = 1;

    else if (c >= '0' && c <= '9')
        network->expected[((int)(c) - 48) + 52] = 1;

}

void PrintState(char expected, char obtained)
{

    printf("Char entered: %c | Char recoginized: %c ",
                                                    expected,
                                                    obtained);
    if (expected == obtained) {
        printf("=> OK\n");
    }
    else{
        printf("=> KO\n");
    }

}

unsigned int *initTrainingSetOrder(unsigned int numTrainingSets)
{
    unsigned int *arr = malloc(sizeof(unsigned int)*numTrainingSets);
    for (unsigned int i = 0; i < numTrainingSets; i++)
    {
        arr[i] = i;
    }
    return arr;
}

size_t IndexRet(struct net *network)
{
    size_t ref = 0;
    for (size_t i = 1; i < network->numOutputs; i++)
    {
        if (network->outputLayer[i] > network->outputLayer[ref])
        {
            ref = i;
        }
    }
    
    return ref;
}

char Retrieve(size_t ref)
{
    char c = 'c';
    if (ref <= 25)
    {
        c = ref + 65;
    }   
    else if (ref > 25 && ref <= 51)
    {
        c = (ref + 97 - 26);
    }
    else if (ref > 51 && ref <= 61)
    {
        c = ref + 48 - 52;
    }

    return c;
}
