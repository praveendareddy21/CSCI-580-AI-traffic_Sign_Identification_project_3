#include "matrix.h"
#include <vector>
#include <iostream>
#include <bitset>

using namespace std;

void matrix::set(int m, int n, int value){
	cell[m][n] = value;
}

void matrix::print(){
  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      cout << cell[i][j] << " ";
    }
    cout << endl;
  }
}

/*
void RMatrix::CrossProduct(RMatrix aMtx, RMatrix bMtx) {
  // Local Variable Declaration
  long double cellTotal = 0;

  // Calculate CrossProduct
  for(int r = 0; r < aMtx.GetNumberRows(); r++) {
    for (int c = 0; c < bMtx.GetNumberColumns(); c++) {
      for (int i = 0; i < aMtx.GetNumberColumns(); i++) {
        cellTotal += aMtx.GetCellValue(r, i)*bMtx.GetCellValue(i, c);
      }
      this->SetCellValue(r, c, cellTotal);
      cellTotal = 0;
    }
  }
}
*/
void matrix::mmult(matrix* m1, matrix* m2){
	for(int i = 0; i < m1->m; i++){
		for(int j = 0; j < m2->n; j++){
			for(int k = 0; k < m1->n; k++){
				cell[i][j] += m1->cell[i][k] * m2->cell[k][j];
			}
		}
	}
}

void matrix::fixup(){
	for(int i = 0; i < n; i++){
		long double counter = 0;
		for(int j = 0; j < m; j++){
			if(cell[j][i])
				counter++;
		}
		for(int k = 0; k < m; k++){
			if(cell[k][i])
				cell[k][i] = 1.0/counter;
		}
		counter = 0;
	}
}

bool matrix::inbounds(int row, int col){
	if(row >= m)
		return false;
	if(row < 0)
		return false;
	if(col >= n)
		return false;
	if(col < 0)
		return false;
	return true;
}

bool matrix::adjacent(int m1, int n1, int m2, int n2){
  if(m1 != m2 && n1 != n2){
    return false;
  }
  return true;
}

/* takes four integers which are provided as two pairs. If the resulting values
    are directly adjacent and there is no obstacle between them returns true
    otherwise returns false
*/
bool matrix::reachable(int m1, int n1, int m2, int n2){
  //if they are not next to eachother, there is no direct path between them
  if(!adjacent(m1, n1, m2, n2)){
    return false;
  }
  //if they are on the same row
  if(m1 == m2){
    //if 1 is on the west
    if(n1 < n2){
      bitset<4> temp = cell[m1][n1];
      bool first = !temp[0];
      temp = cell[m2][n2];
      bool second = !temp[1];
      return(first && second);
    }
    //if 1 is on the east
    else{
      bitset<4> temp = cell[m2][n2];
      bool first = !temp[0];
      temp = cell[m1][n1];
      bool second = !temp[1];
      return(first && second);
    }
  }
  //if they are on the same comlumn
  else{
    //if 1 is north of 2
    if(m1 < m2){
      bitset<4> temp = cell[m1][n1];
      bool first = !temp[2];
      temp = cell[m2][n2];
      bool second = !temp[3];
      return(first && second);
    }
    //if 1 is south of 2
    else{
      bitset<4> temp = cell[m1][n1];
      bool first = !temp[3];
      temp = cell[m2][n2];
      bool second = !temp[2];
      return(first && second);
    }
  }
}

//returns the number of non-15(blocked) cells
int matrix::valids(){
  int valid = 0;
  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      if(cell[i][j] != 15)
        valid++;
    }
  }
  return valid;
}
