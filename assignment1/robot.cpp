#include "robot.h"

Robot::Robot(Matrix grid){
  gridWorld = grid;
}

Robot::~Robot(){

}

int Robot::countValid(){
  int valids = 0;
  for(int i = 0; i < gridWorld.getRows(); i++){
    for(int j = 0; i < gridWorld.getColumns(); j++){
      if(gridWorld.cell[i][j] != 15){valids++;}
    }
  }
  return valids;
}

void Robot::localize(){
  return;
}

void Robot::initializeTransitive(){
  return;
}
