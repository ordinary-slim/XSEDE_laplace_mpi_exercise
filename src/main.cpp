#include <mpi.h>
#include "global.h"
#include <iostream>
#include <map>

using namespace std;

// Global variables definition; declaration in global.h
extern const double maxError = 1e-2;
extern const double maxIter = 4000;
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

  double localError = 100;
  double globalError = 999;
  int iter = 0;
  int locRow, glbRow;
  // MAIN LOOP
  while ( globalError > maxError && iter < maxIter ) {
    //COMMUNICATE GHOST ROWS
    int up = world_rank - 1;
    int down = world_rank + 1;
    if (world_size>1) {
      //Downward
      if (world_rank != (world_size-1)) {
        MPI_Send(T[local2global.size()], COLS+2, MPI_DOUBLE, down, 0, MPI_COMM_WORLD);
      }
      if (world_rank != 0) {
        MPI_Recv(T[0], COLS+2, MPI_DOUBLE, up, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      //Upwards
      if (world_rank != 0) {
        MPI_Send(T[1], COLS+2, MPI_DOUBLE, up, 0, MPI_COMM_WORLD);
      }
      if (world_rank != (world_size-1)) {
        MPI_Recv(T[local2global.size()+1], COLS+2, MPI_DOUBLE, down, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }

    //COMPUTATION
    for (auto& rowMap: local2global) {
      locRow = rowMap.first;
      glbRow = rowMap.second;
      if ((glbRow == 0)||(glbRow == ROWS+1)) { continue; }// skip BC rows
      for (int j = 1; j <= COLS; j++){
        T[locRow][j] = 0.25 * (Tprev[locRow+1][j] + Tprev[locRow-1][j] + Tprev[locRow][j+1] + Tprev[locRow][j-1]);
      }
    }

    localError = 0.0;
    //CHECK CONVERGENCE
    for (auto& rowMap: local2global) {
      for (int j = 1; j <= COLS; j++){
        locRow = rowMap.first;
        localError   = max( localError, abs(T[locRow][j] - Tprev[locRow][j] ));
        Tprev[locRow][j] = T[locRow][j];
      }
    }
    MPI_Allreduce( &localError, &globalError, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    //PRETTY PRINT
    if (world_rank==0){
      if (iter % 100 == 0) {
        printf("proc %d, iter %d, loc error %.5f, globalError: %.5f\n", world_rank, iter, localError, globalError);
        //track_progresion(T, iter);
      }
    }

    iter++;
  }
  // POST
  printf("Exited after %d iters.\n", iter);
  write_vtk(T, local2global);

  //Finalize MPI
  MPI_Finalize();
}
