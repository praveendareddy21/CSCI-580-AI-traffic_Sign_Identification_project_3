#include "robot.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

//converts NSWE or a substring thereof into an integer resulting from the binary value of the contents
// bitmasked in that order where N is 0 bit and E is the 8 bit. Possible values are 0-15

int main(int argc, char const *argv[]) {
  if(argc < 5){
    cout << "usage: ./robot input_file sensory_error observations" << endl;
    return 1;
  }

  Matrix gridWorld;
  string fname;
  string line;
  int row = 0;
  int column = 0;
  fname = argv[1];
  long double sensory_error = *argv[2];
  string *observations = new string[argc-3];

  for(int i = 3, j = 0; i < argc; i++ && j++){
    observations[j] = argv[i];
  }

  ifstream ifile;
  ifile.open(fname);
  if(ifile){
    //get the whole line
    while(getline(ifile, line)){
      //cast it to a stringstream
      istringstream ss(line);
      string temp;
      //while we have not parsed the entire line
      while(!ss.eof()){
        getline(ss, temp, ' ');//using getline to get strings one word at a time, space delimited
        gridWorld.cell[row][column] = atoi(temp.c_str());
        column++;
      }
      column = 0;
      row++;
    }
    for(int i = 0; i < gridWorld.getRows(); i++){
      for(int j = 0; j < gridWorld.getColumns(); j++){
        cout << " " << gridWorld.cell[i][j];
      }
      cout << endl;
    }
    exit(0);
    //the matrix is full now

  }
  else{cout<<"Bad filename, check permissions \n";}

  ifile.close();
  return 0;
}
