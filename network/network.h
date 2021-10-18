#ifndef NN_H_
#define NN_H_

#include <SDL/SDL.h>

//struct network
struct net {
  //int number_of_inputs;
  unsigned int numInputs;
  //int number_of_hidden_nodes ;
  unsigned int numHiddenNodes;
  //int number_of_outputs ;
  unsigned int numOutputs;
  //double *input_layer;
  double *inputLayer;
  // number of taining set;
  unsigned int numTrainingSets;

  //double *hidden_layer;
  double *hiddenLayer;
  //double *delta_hidden;
  double *deltaHidden;
  //double *hidden_layer_bias;
  double *hiddenLayerBias;
  //double *hidden_weights;
  double *hiddenWeights;
  //double *delta_hidden_weights;
  double *deltaHiddenWeights;

  //double *output_layer;
  double *outputLayer;
  //double *delta_output;
  double *deltaOutput;
  //double *output_layer_bias;
  double *outputLayerBias;
  //double *output_weights;
  double *outputWeights;
  //double *delta_output_weights;
  double *deltaOutputWeights;

  //double eta;
  double phi;
  //double alpha;
  double alpha;
  //double *goal;
  double *expected;
  //we have two images per imput for one output ex: {{A1,A2}} -> {A}
  //unsigned int *trainingSetOrder;
};
double dsigmoid (double x);

struct net *InitNet(unsigned int in, unsigned int hid, unsigned int out,char *file);

void initialization(struct net *network);

void forward_pass(struct net *network);

void back_propagation(struct net *network);

void updateweightsetbiases(struct net *network);

void TrainNet(void);

char * updatepath(char *filepath,size_t len,size_t index);

void initTrainingInput(char *fileName,struct net *network);

void ExpectedOutput(struct net *network,char c);

void PrintState(char expected, char obtained);

unsigned int *initTrainingSetOrder(unsigned int numTrainingSets);

size_t IndexRet(struct net *network);

char Retrieve(size_t ref);

void freeNet(struct net *network);
void netOut(SDL_Surface *img);

#endif