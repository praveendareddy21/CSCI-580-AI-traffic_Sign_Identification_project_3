/*
Matthew Dias
CSCI 580
ann.h

Header file describing the functions and declaring member variables for the ANN class
*/

#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using namespace std;

class Node{
public:
  vector<long double> weights;
  long double aValue;
  long double delta;
  long double dummy = .01;
};

class ANN{
public:
  ANN(){};
  ~ANN(){};
  vector< vector<Node> > network;
  long double aPrime(long double);
  void feedForward(vector<long double>);
  int euclid(vector<long double>);
  int test(vector<long double>);
  void train(vector<long double>, int);
  void print();
};

#endif
