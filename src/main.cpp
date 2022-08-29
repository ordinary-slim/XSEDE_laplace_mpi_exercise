#include <mpi.h>
#include <iostream>

using namespace std;

// Global variables definition; declaration in global.h
extern const int cols = 1000;
extern const int rows = 1000;

extern const double maxError = 1e-3;
extern const double maxIter = 1e3;

//Declarations
void initialize (double **Tprev);
void track_progresion(int iter);


int main () {

  double Tprev;
  Tprev = new double *[rows+2];

  double T[cols+2][rows+2];

  double currError = 100;
  int iter = 0;

  // initialize Tprev
  initialize(Tprev);

  /*
  while ( currError > maxError && iter < maxIter ) {

    //MAIN
    for (int i = 1; i <= rows; i++){
      for (int j = 1; j <= cols; j++){
        T[i][j] = 0.25 * (Tprev[i+1][j] + Tprev[i-1][j] + Tprev[i][j+1] + Tprev[i+1][j-1]);
      }
    }

    currError = 0.0;
    //CHECK CONVERGENCE
    for (int i = 1; i <= rows; i++){
      for (int j = 1; j <= cols; j++){
        currError   = max( currError, abs(T[i][j] - T[i][j] ));
        Tprev[i][j] = T[i][j];
      }
    }

    //PRINT
    if (iter % 100 == 0) {
      track_progresion(iter);
    }

    iter++;
  }
    */
}
