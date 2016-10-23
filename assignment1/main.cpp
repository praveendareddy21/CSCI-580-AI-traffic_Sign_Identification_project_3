#include "robot.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

//adapted from code below, this gets a count of columns from input file
int getColumns(string fname){
  string line;
  int columns = 0;
  ifstream ifile;
  ifile.open(fname.c_str());
  while(getline(ifile, line)){
    istringstream ss(line);
    string temp;
    while(!ss.eof()){
      getline(ss, temp, ' ');
      columns++;
    }
    return columns;
  }
}

//adapted from code below, this gets a count of lines in input file
int getRows(string fname){
  string line;
  int rows = 0;
  ifstream ifile;
  ifile.open(fname.c_str());
  while(getline(ifile, line)){
    istringstream ss(line);
    string temp;
    while(!ss.eof()){
      getline(ss, temp, ' ');
    }
    rows++;
  }
  return rows;
}

int main(int argc, char const *argv[]) {
  if(argc < 5){
    cout << "usage: ./robot input_file sensory_error observations" << endl;
    return 1;
  }
  int n, m;
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

  m = getRows(fname);
  n = getColumns(fname);
  matrix myMatrix(m, n);

  m = n = 0;

  ifstream ifile;

  ifile.open(fname.c_str());
  if(ifile){
    //get the whole line
    while(getline(ifile, line)){
      //cast it to a stringstream
      istringstream ss(line);
      string temp;
      //while we have not parsed the entire line
      while(!ss.eof()){
        getline(ss, temp, ' ');//using getline to get strings one word at a time, space delimited
        myMatrix.set(m, n, atoi(temp.c_str()));
        n++;
      }
      n = 0;
      m++;
    }
    //the matrix is full now
    robot myRobot(&myMatrix, observations, (argc-3));
    myRobot.process();
  }
  else{cout<<"Bad filename, check permissions \n";}

  ifile.close();
  return 0;
}
