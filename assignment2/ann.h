#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using namespace std;

class Node{
public:
  vector<long double> weights;
  long double aValue;
  long double delta;
};

class ANN{
public:
  vector< vector<Node> > network;
};

#endif
