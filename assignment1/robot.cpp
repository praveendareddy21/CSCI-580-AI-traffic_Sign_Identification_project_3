#include "robot.h"
#include <iostream>
#include <map>
#include <utility>

using namespace std;


void robot::process(){

  //initialize the transitivity matrix
  matrix trans(grid->m*grid->n,grid->m*grid->n);
  for(int i = 0; i < grid->m; i++){
    for(int j = 0; j < grid->n; j++){
      //top
      if(grid->inbounds(i-1, j) && grid->reachable(i-1, j, i, j)){
        trans.cell[i*grid->n+j][(i-1)*grid->n+j] = 1.0;
      }
      //bottom
      if(grid->inbounds(i+1, j) && grid->reachable(i+1, j, i, j)){
        trans.cell[i*grid->n+j][(i+1)*grid->n+j] = 1.0;
      }
      //left
      if(grid->inbounds(i, j-1) && grid->reachable(i,j-1,i,j)){
        trans.cell[i*grid->n+j][i*grid->n+j-1] = 1.0;
      }
      //right
      if(grid->inbounds(i, j+1) && grid->reachable(i, j+1, i, j))
        trans.cell[i*grid->n+j][i*grid->n+j+1] = 1.0;
    }
  }
  trans.fixup();

  matrix j0(trans.n, 1);
  int count = 0;

  for(int i = 0; i < grid->m; i++){
    for(int j = 0; j < grid->n; j++){
      if(grid->cell[i][j] != 15){
        j0.cell[count][0] = 1;
      }
      count++;
    }
  }
  j0.fixup();

  matrix px1(trans.n, 1);

  px1.mmult(&trans, &j0);
  px1.print();

  for(int i = 0; i < numObs; i++){
    
  }


  return;
}
