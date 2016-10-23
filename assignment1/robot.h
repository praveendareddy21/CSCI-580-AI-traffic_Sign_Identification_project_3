#ifndef ROBOT_H
#define ROBOT_H
#include "matrix.h"
#include <string>

class robot{
   long double bitDiff[5]={0.6561, 0.0729, 0.0081, 0.0009, 0.00001};
public:
  int numObservations;
  string *observations;
  matrix *grid;
  robot(matrix* input, string* obs, int numObs){
    grid = input;
    observations = obs;
    numObservations = numObs;
  }
  void process();
};
#endif
