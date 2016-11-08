#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "ann.h"
using namespace std;

int main(int argc, char const *argv[]) {
  if(argc != 8){
    //./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 100
    cout << "Usage: ann [training_input] [input_class_labels] [input_vectors] [test_class_labels] [node_structure] [initial_weights] [number_of_iterations]" << endl;
    exit(1);
  }
  /*command line arguments:
  argv[0]: ./ann
  argv[1]: train_input.txt contains input vectors of the training data set;
  argv[2]: train_output.txt contains class labels for the data points of the training data set;
  argv[3]: test_input.txt contains input vectors of the testing data set;
  argv[4]: test_output.txt contains class labels for the data points of the testing data set;
  argv[5]: structure.txt contains the number of nodes (neurons) in each layer including the input and output layer;
  argv[6]: weights.txt contains the initial weights of all links of the neural networks between the nodes of any two consecutive layers (does not contain the dummy weights ; assume that for each node i; assume that the nodes in the input layer do not have the dummy weights).
  */

  vector<int> structure;

  ANN myANN;

  int temp;
  ifstream myfile;
  myfile.open(argv[5]);
  if(myfile.is_open()){
    while(myfile.good()){
      myfile >> temp;
      structure.push_back(temp);
    }
  }
  else{
    cout << "FILE READ ERROR" << endl;
    exit(1);
  }
  myfile.close();

  //specify the number of layers for the nueral network
  myANN.network.resize(structure.size()-1);
  //specify the number of nodes on each layer
  for(int i = 0; i < myANN.network.size(); i++){
    myANN.network[i].resize(structure[i]);

    if(i < myANN.network.size()-1){
      for(int j = 0; j < myANN.network[i].size(); j++){
        //specify the number of weights LEAVING a given node
        myANN.network[i][j].weights.resize(structure[i+1]);
      }
    }
  }

  //open and read then apply the weights to the ANN
  myfile.open(argv[6]);
  if(myfile.is_open()){
    int i = 0;
    string line;

    while(i < myANN.network.size()-1){

      for(int j = 0; j < myANN.network[i].size(); j++){

        getline(myfile, line);
        int k = 0;
        long double curWeight;
        istringstream ss(line);

        while(k < myANN.network[i][j].weights.size()){
          ss >> curWeight;
          myANN.network[i][j].weights[k] = curWeight;
          k++;
        }
      }
      i++;
    }
  }
  else{
    cout << "FILE READ ERROR" << endl;
    exit(1);
  }
  myfile.close();

  //open and store training set data
  myfile.open(argv[1]);
  vector<vector<long double> > inputs(100, vector<long double>(myANN.network[0].size()));
  if(myfile.is_open()){
    string line;

    //use getline to get the input lines, then >> for the long doubles
    for(int i = 0; i < 100; i++){
      getline(myfile, line);
      istringstream ss(line);
      for(int j = 0; j < myANN.network[0].size(); j++){
        ss >> inputs[i][j];
      }
    }
  }

  else{
    cout << "FILE READ ERROR" << endl;
    exit(1);
  }
  myfile.close();

  myfile.open(argv[2]);
  vector<int> labels(100);
  
  if(myfile.is_open()){
    for(int i = 0; i < labels.size(); i++){
      myfile >> labels[i];
    }
  }
  else{
    cout << "FILE READ ERROR" << endl;
    exit(1);
  }
  myfile.close();

  //train the ANN
  for(int i = 0; i < inputs.size(); i++){
    //run train for each input vector and the associated label
    myANN.train(inputs[i], labels[i]);
  }



  return 0;
}
