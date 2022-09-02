#include <mpi.h>
#include "global.h"
#include <iostream>

using namespace std;

// Global variables definition; declaration in global.h
extern const double maxError = 1e-2;
extern const double maxIter = 4e3;

//Declarations
void initialize (double (*Tinit)[COLS+2]);
void track_progresion(double (*T)[COLS+2], int iter);
void write_vtk(double (*T)[COLS+2], int iter);


int main () {

  double T[ROWS+2][COLS+2];
  double Tprev[ROWS+2][COLS+2];

  double currError = 100;
  int iter = 0;

  // initialize Tprev
  initialize(Tprev);

  cout << T[0][0] << endl;
  while ( currError > maxError && iter < maxIter ) {

    //MAIN
    for (int i = 1; i <= ROWS; i++){
      for (int j = 1; j <= COLS; j++){
        T[i][j] = 0.25 * (Tprev[i+1][j] + Tprev[i-1][j] + Tprev[i][j+1] + Tprev[i][j-1]);
      }
    }

    currError = 0.0;
    //CHECK CONVERGENCE
    for (int i = 1; i <= ROWS; i++){
      for (int j = 1; j <= COLS; j++){
        currError   = max( currError, abs(T[i][j] - Tprev[i][j] ));
        Tprev[i][j] = T[i][j];
      }
    }

    //PRINT
    if (iter % 100 == 0) {
      printf("Current error: %.6f", currError);
      track_progresion(T, iter);
    }

    iter++;
  }

  // pretty print
  write_vtk(T, iter);
  printf("Converged in %d iters\n", iter);
  printf("Final error estimate= %.5f\n", currError);
}
