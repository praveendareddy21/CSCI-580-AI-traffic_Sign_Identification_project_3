#include "matrix.h"
#include <vector>
#include <iostream>

using namespace std;

int Matrix::getColumns(){
  return cell.size();
}

int Matrix::getRows(){
  return int(cell[0].size());
}


//4x3 * 3x2 = 4x2
//[a  b  c]       [q  w  e]      [aq+br+cu aw+bt+ci ae+by+co]
//[x  y  z]    *  [r  t  y]   =  [xq+yr+zu zw+yt+zi xe+yy+zo]
//[j  k  l]       [u  i  o]      [jq+kr+lu jw+kt+li je+ky+lo]
vector< vector<long double> > Matrix::multiply(Matrix m1){
  //if the matrices cant be multiplied
  if(getColumns() != m1.getRows()){
    cout << "incompatable matrices" << endl;
    return m1.cell;
  }
  //if the matrix can be multiplied
  else{
    vector<vector<long double> > result;
    //for all of the columns
    for (int i = 0; i < getColumns(); i++){
      //for all of the rows
      for (int j = 0; j < m1.getColumns(); j++){
        //add the product of the column * row to the cell value
        for (int k = 0; k < getRows(); k++){
          result[i][j] += cell[i][k] * m1.cell[k][j];
          cout << "result[ "<< i <<"][" << j<< "]" << endl;
        }
      }
    }
    return result;
  }
}
