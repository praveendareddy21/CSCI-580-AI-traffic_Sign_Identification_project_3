/*
Matthew Dias
CSCI 580
project 2
ann.cpp
*/

#include <iostream>
#include <math.h>
#include <iomanip>
#include "ann.h"

using namespace std;

static long double ALPHA = 0.01;

/*
  The nueral network is made up of a vector of Node vectors, where each node has a vector of weights
  to access a given layer, use network[x] where x is the layer. Accessing each value for the specified
  layer can be achieved with network[x][y] which will evaluate into a Node. Node values can be accessed
  by network[x][y].weights[z] which will evaluate into a long double
*/

//the encoding for the three possible digits that can be recognized by
static long double digits[3][10] = { {0.1, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9},
                                     {0.9, 0.1, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9},
                                     {0.9, 0.9, 0.1, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9} };

/*
Feed forward takes a vector of input nodes and and calculates A values through the nodes until they propagated through
  the entire network
*/
void ANN::feedForward(vector<long double> inputs){

  //set the input layer equal to the input vector
  for(int i = 0; i < network[0].size(); i++){
    network[0][i].aValue = inputs[i];
  }

  //for the rest of the layers, calculate A and INj

  //for all the layers
  for(int i = 1; i < network.size(); i++){
    //for all the nodes in the layer
    for(int j = 0; j < network[i].size(); j++){
      long double inj = network[i][j].dummy;
      //for all nodes in the previous layer
      for(int k = 0; k < network[i-1].size(); k++){
        inj += network[i-1][k].aValue * network[i-1][k].weights[j];
      }
      //the current node's a value is the result of the SAF on inj
      network[i][j].aValue = (long double)1/((long double)1+expl(-inj));
    }
  }
}

/*
aprime returns the input * (1 - itself)
  this is just for convenience and readability
*/
long double ANN::aPrime(long double aVal){
  return aVal*(1-aVal);
}

/*
  Euclid takes a vector from the Avalues of the output layer, and determines the digit most closely represented by those values
*/
int ANN::euclid(vector<long double> outputs){
  long double euc0 = sqrtl(powl((digits[0][0] - outputs[0]),2) + powl((digits[0][1] - outputs[1]),2) + powl((digits[0][2] - outputs[2]),2) );
  long double euc1 = sqrtl(powl((digits[1][0] - outputs[0]),2) + powl((digits[1][1] - outputs[1]),2) + powl((digits[1][2] - outputs[2]),2) );
  long double euc2 = sqrtl(powl((digits[2][0] - outputs[0]),2) + powl((digits[2][1] - outputs[1]),2) + powl((digits[2][2] - outputs[2]),2) );

  long double lowest = 100;
  if(euc2 < lowest){
    lowest = euc2;
  }
  if(euc1 < lowest){
    lowest = euc1;
  }
  if(euc0 < lowest){
    lowest = euc0;
  }

  if(lowest == euc0){
    cout << "0" << endl;
    return 0;
  }
  else if(lowest == euc1){
    cout << "1" << endl;
    return 1;
  }
  else{
    cout << "2" << endl;
    return 2;
  }
}

/*
  Test takes an a vector with a value for each input layer node and propagates it through the ANN until the output layer
  it then uses euclid to determine the most likely digit 0, 1, or 2 and return that value
*/
int ANN::test(vector<long double> inputs){
  feedForward(inputs);
  int outputLayer = network.size()-1;
  vector<long double> output_values = vector<long double>(network[outputLayer].size());
  for(int i = 0; i < network[outputLayer].size(); i++){
    output_values[i] = network[outputLayer][i].aValue;
  }
  return euclid(output_values);
}

/*
  Train takes a vector with a value for each input layer node and propagates it through the ANN, it then calculates error between
the expected outputs and actual outputs and adjust the netwrok's weights accordingly
*/
void ANN::train(vector<long double> inputs, int label){

  feedForward(inputs);

  ////////////////calcualte error////////////////////////////

  //calculate output layer ERROR
  int output = network.size()-1;

  //for all of the output layer nodes:
  for(int i = 0; i < network[output].size(); i++){
    //the delta value for an output node: a(1-a) * (Yj - a)
    network[output][i].delta = aPrime(network[output][i].aValue)*(digits[label][i] - network[output][i].aValue);
  }

  //for all the hidden layers
  for(int i = output-1; i > 0; i--){
    //for all the nodes in the hidden layers
    for(int j = network[i].size()-1; j >= 0; j--){
      long double Edelta = 0;
      for(int k = network[i+1].size()-1; k >= 0; k--){
        Edelta += network[i][j].weights[k] * network[i+1][k].delta;
      }
      network[i][j].delta = aPrime(network[i][j].aValue)*Edelta;
    }
  }

  /*
  //for every layer
  for(int i = 0; i < network.size(); i++){
    //for every node
    for(int j = 0; j < network[i].size(); j++){
      //update each node's dummy weight
      network[i][j].dummy += ALPHA * network[i][j].aValue;
      //for every weight from that node to other nodes
      for(int k = 0; k < network[i][j].weights.size(); k++){
        network[i][j].weights[k] += ALPHA * network[i][j].aValue * network[i+1][k].delta;
      }
    }
  }*/

  /////////////////adjust the weights////////////////////////////
  //for a given layer, i
  for(int i = 0; i < network.size(); i++){
    //for a given node, j
    for(int j = 0; j < network[i].size(); j++){
      //update the dummy weight
      network[i][j].dummy += ALPHA * network[i][j].delta;
      //for a given weight, k
      for(int k = 0; k < network[i][j].weights.size(); k++){
        //the weight for a specific edge is oldWeight + .01 * aValue * destination node's delta
        network[i][j].weights[k] += ALPHA*(network[i][j].aValue)*(network[i+1][k].delta);
        //weight from j to k
      }
    }
  }
}

/*
  print outputs the weights of all the paths from node 0 in layer 0 to every node in layer 1
*/
void ANN::print(){
  cout << showpoint << fixed << setprecision(12);
    for(int k = 0; k < network[0][0].weights.size(); k++){
      cout << network[0][0].weights[k] << " ";
    }
  cout << endl;
}
