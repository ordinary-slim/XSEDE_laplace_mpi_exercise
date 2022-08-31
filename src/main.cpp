#include <mpi.h>
#include "global.h"
#include <iostream>

using namespace std;

// Global variables definition; declaration in global.h
extern const double maxError = 1e-3;
extern const double maxIter = 4e3;

double T[ROWSP2][COLSP2];
double Tprev[ROWSP2][COLSP2];


//Declarations
void initialize();
void track_progresion(int iter);
void write_vtk(int);


int main () {


  double currError = 100;
  int iter = 0;

  // initialize Tprev
  initialize();

  while ( currError > maxError && iter < maxIter ) {

    //MAIN
    for (int i = 1; i <= ROWS; i++){
      for (int j = 1; j <= COLS; j++){
        T[i][j] = 0.25 * (Tprev[i+1][j] + Tprev[i-1][j] + Tprev[i][j+1] + Tprev[i+1][j-1]);
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
      track_progresion(iter);
      write_vtk(iter);
      cout << "Wrote out.vtk" << endl;
    }

    iter++;
  }

  // pretty print
  printf("Converged in %d iters\n", iter);
  printf("Final error estimate= %.5f\n", currError);
}
