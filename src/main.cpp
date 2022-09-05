#include <mpi.h>
#include "global.h"
#include <iostream>
#include <map>

using namespace std;

// Global variables definition; declaration in global.h
extern const double maxError = 1e-2;
extern const double maxIter = 4e3;
int world_size, world_rank;

//Declarations
void initialize (double (*Tinit)[COLS+2], map<int,int>);
void initialize_mpi(int &world_size, int &world_rank);
void track_progresion(double (*T)[COLS+2], int iter);
void write_vtk(double (*T)[COLS+2], map<int,int>);

int main () {

  initialize_mpi(world_size, world_rank);

  //Build row partition in proc 0
  //Similar to calling a partitionner at rank 0
  int rPart[ROWS+2];
  int rowsPerProc = (ROWS+2) / world_size;
  if (world_rank==0) {
    for (int i = 0; i < ROWS+2; i++) {
      rPart[i] = i / rowsPerProc;
      if (rPart[i] > world_size-1 ) {
        rPart[i] = world_size - 1;
      }
    }
  }
  //BCAST row partition
  MPI_Bcast(rPart, ROWS+2, MPI_INT, 0, MPI_COMM_WORLD);
  //Build local2global mapping in ea partition
  //Could do it in two loops with pure arrays or in 1 loop with dynamic arrays
  map<int, int> local2global;
  int counter = 1;
  for (int i = 0; i < ROWS + 2; i++) {
    if (rPart[i] == world_rank) {
      local2global[counter] = i;
      counter++;
    }
  }
  //Allocate solution with space for ghost rows
  double T[local2global.size()+2][COLS+2];
  double Tprev[local2global.size()+2][COLS+2];

  //INITIAL CONDITION
  initialize(Tprev, local2global);
  //copy initial condition to solution array
  for (int i = 0; i < local2global.size()+2; i++) {
    for (int j = 0; j < COLS+2; j++) {
      T[i][j] = Tprev[i][j];
    }
  }

  // POST
  write_vtk(T, local2global);

  //Finalize MPI
  MPI_Finalize();
}
