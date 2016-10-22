#ifndef MATRIX_H
#define MATRIX_H
#include <vector>

using namespace std;

class Matrix{
  public:
    vector<vector<long double> >cell;
    void update();
    vector<vector<long double> > multiply(Matrix);
    int getRows();
    int getColumns();
  private:
};

#endif
