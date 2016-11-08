#include <iostream>
#include <math.h>
#include "ann.h"

using namespace std;

/*
  The nueral network is made up of a vector of Node vectors, where each node has a vector of weights
  to access a given layer, use network[x] where x is the layer. Accessing each value for the specified
  layer can be achieved with network[x][y] which will evaluate into a Node. Node values can be accessed
  by network[x][y].weights[z] which will evaluate into a long double
*/


void ANN::train(vector<long double> inputs, int label){
  //set the A values for the input layer
  for(int i = 0; i < network[0].size(); i++){
    network[0][i].aValue = inputs[i];
  }

  //for the rest of the layers, calculate A and INj

  //for all of the layers, starting with 1
  for(int i = 1; i < network.size(); i++){
    //for all of the nodes in the layer
    for(int j = 0; j < network[i].size(); j++){
      long double inj = 0;
      //for all of the previous layer's nodes
      for(int k = 0; k < network[i-1].size(); k++){
        inj += network[i-1][k].aValue * network[i-1][k].weights[j];
      }
      inj += dummy;
      aValue = 1/(1+expl(-inj));
    }
  }

}
