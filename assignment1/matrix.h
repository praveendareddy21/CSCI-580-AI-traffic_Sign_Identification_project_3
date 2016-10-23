#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using namespace std;

class matrix
{
public:
  long double **cell;
  int m, n;
   matrix(int row, int col){
      m = row;
      n = col;
      cell = new long double*[m];
      for (int i = 0; i < m; ++i){
	       cell[i] = new long double[n];
	        for (int j = 0; j < n; ++j){
	         cell[i][j] = 0;
         }
      }
   }

   ~matrix(){
     //delete [] cell;
   }
  long double highest();
  void smult(long double x);
  long double sumValues();
  void observations();
  void mmult(matrix*, matrix*);
  void fixup();
  bool reachable(int, int, int, int);
  bool adjacent(int, int, int, int);
  void set(int, int, long double);
  bool adjacent(int, int);
  int valids();
  bool inbounds(int, int);
  void print();//for debugging
};

#endif
