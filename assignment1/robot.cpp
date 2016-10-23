#include "robot.h"
#include <iostream>
#include <map>
#include <utility>
#include <bitset>
#include <iomanip>
#include <vector>

using namespace std;
/*Converts a string of form NSWE to the integer value
    creating by mapping N to bit 3 S to bit 2 W to bit
    1 and E to bit 0
*/
int robot::toInt(string input){
  int retval = 0;
  if(input.find('N') != string::npos){
    retval += 8;
  }
  if(input.find('S') != string::npos){
    retval += 4;
  }
  if(input.find('W') != string::npos){
    retval += 2;
  }
  if(input.find('E') != string::npos){
    retval += 1;
  }
  return retval;
}

/*Converts a string of form NSWE to the integer value
    creating by mapping N to bit 3 S to bit 2 W to bit
    1 and E to bit 0
*/
int robot::bitComp(int num, string obstacles){
  int count = 0;
  bitset<4>obstacle = toInt(obstacles);
  bitset<4>cell = num;
  for(int i = 0; i < 4; i++){
    if(cell[i] != obstacle[i]){
      count++;
    }
  }
  return count;
}

/*The "main()" for robot. Creates all matrices and does all
    probability calculations
*/
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

  //set the values to their correct probabilities
  trans.fixup();

  //Joint probability matrix
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
  //set the values to correct probabilites
  j0.fixup();

  matrix z(trans.n, 1);

  z.mmult(&trans, &j0);

  matrix ob(trans.n, trans.n);

  matrix e1(trans.n, 1);

  matrix estimation(trans.n, 1);

  matrix J(trans.n, 1);
  J = z;

  matrix y1(trans.n, 1);

  //setting the observation matrix values
  for(int j = 0; j < trans.n; j++){
    if(grid->cell[j/(grid->n)][j%grid->n] == 15){
      ob.cell[j][j] = 0;
    }
    else{
      ob.cell[j][j] = bitDiff[bitComp(grid->cell[j/(grid->n)][j%grid->n], observations[0])];
    }
  }
  //Y = Observation * J
  y1.mmult(&ob, &J);

  J.mmult(&trans, &y1);

  long double E = 1.0/y1.sumValues();
  y1.smult(E);

  long double estimation_probability = y1.highest();

  cout << std::showpoint << std::fixed << setprecision(12) << estimation_probability;
  vector<int> biggests;

  for(int z = 0; z < y1.n; z++){
    for(int x = 0; x < y1.m;  x++){
      if(y1.cell[x][z] == estimation_probability){
        biggests.push_back(x);
      }
    }
  }

  for(int i = 0; i < biggests.size(); i++){
    cout << " " << biggests[i];
  }
  cout << endl;

  //for the non initial o
  for(int i = 1; i < numObservations ;i++){
    //initialize the observation matrix
    for(int j = 0; j < trans.n; j++){
      if(grid->cell[j/(grid->n)][j%grid->n] == 15){
        ob.cell[j][j] = 0;
      }
      else{
        ob.cell[j][j] = bitDiff[bitComp(grid->cell[j/(grid->n)][j%grid->n], observations[i])];
      }
    }
    y1.mmult(&ob, &J);
    J.mmult(&trans, &y1);

    long double E = 1.0/y1.sumValues();
    y1.smult(E);

    long double estimation_probability = y1.highest();

    //print block
    cout << std::showpoint << std::fixed << setprecision(12) << estimation_probability;
    vector<int> biggests;

    for(int z = 0; z < y1.n; z++){
      for(int x = 0; x < y1.m;  x++){
        if(y1.cell[x][z] == estimation_probability){
          biggests.push_back(x);
        }
      }
    }

    for(int i = 0; i < biggests.size(); i++){
      cout << " " << biggests[i];
    }
    cout << endl;
  }
  return;
}
