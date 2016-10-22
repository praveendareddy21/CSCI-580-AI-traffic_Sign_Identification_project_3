#ifndef ROBOT_H
#define ROBOT_H
#include "matrix.h"

class Robot{
public:
  Robot(Matrix grid);
  ~Robot();
  //quick lookup array for bit difference probablility
  //  index is equal to the number of bits different. bitdiff[0] would
  //  be two matching bit patterns
  long double bitdiff[5] = {.6561, 0.0729, 0.0081, 0.0009, 0.0001};
  Matrix gridWorld;
  Matrix transitivity;
  Matrix j0;
  Matrix jp1;
  int countValid();
  void process();
  void localize();
  void initializeTransitive();
private:
};

#endif
