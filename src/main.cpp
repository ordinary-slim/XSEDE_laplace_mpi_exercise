#include <mpi.h>
#include "global.h"
#include <iostream>
#include <map>
#include <set>

using namespace std;

// Global variables definition; declaration in global.h
extern const double maxError = 1e-2;
extern const double maxIter = 1;
int world_size, world_rank;

//Declarations
void initialize (double (*Tinit)[COLS+2], map<int,int>);
void initialize_mpi(int &world_size, int &world_rank);
void track_progresion(double (*T)[COLS+2], int iter);
void write_vtk(double (*T)[COLS+2], map<int,int>);
void prettyPrint (int itask, double (*Tout)[COLS+2], map<int,int> local2global);

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
  //Build local2global mapping in ea process
  //Mapping only takes into account local data
  //First and last row are ghost rows, m
  map<int, int> local2global;
  int counter = 1;
  for (int i = 0; i < ROWS + 2; i++) {
    if (rPart[i] == world_rank) {
      local2global[counter] = i;
      counter++;
    }
  }
  for (int proc = 0; proc < world_size; proc++) {
    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank==proc){
      for (auto& Map: local2global) {
        printf( "%d -----> %d\n", Map.first, Map.second );
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  //Allocate solution with space for ghost rows
  double T[local2global.size()+2][COLS+2];
  double Tprev[local2global.size()+2][COLS+2];

  std::set<int> BC_Rows = {0, ROWS+1};
  std::set<int> BC_Cols = {0, COLS+1};

  //INITIAL CONDITION
  initialize(Tprev, local2global);
  //copy initial condition to solution array
  for (int i = 0; i < local2global.size()+2; i++) {
    for (int j = 0; j < COLS+2; j++) {
      T[i][j] = Tprev[i][j];
    }
  }

  if (world_rank==0) { printf("Printing T\n");}
  MPI_Barrier(MPI_COMM_WORLD);
  prettyPrint( -1, T,local2global); 

  double localError = 999;
  double globalError = 999;
  double myError = 0.0;
  int iter = 0;
  // MAIN LOOP
  while ( iter < maxIter ) {
    //COMMUNICATE GHOST ROWS
    int up = world_rank - 1;
    int down = world_rank + 1;
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
    for (int proc = 0; proc < world_size; proc++ ) {
      MPI_Barrier(MPI_COMM_WORLD);
      prettyPrint( proc, T,local2global); 
    }


    //COMPUTATION
    int locRow, glbRow;
    for (auto& rowMap: local2global) {
      locRow = rowMap.first;
      glbRow = rowMap.second;
      if (BC_Rows.find( glbRow ) != BC_Rows.end()) { continue; }
      for (int j = 1; j < COLS+1; j++){
        T[locRow][j] = 0.25 * (Tprev[locRow+1][j] + Tprev[locRow-1][j] + Tprev[locRow][j+1] + Tprev[locRow][j-1]);
      }
    }

    localError = 0.0;
    if (world_rank==2){
      myError = abs(T[250][900] - Tprev[250][900]);
    }
    //CHECK CONVERGENCE
    for (auto& pair: local2global) {
      for (int j = 1; j < COLS+1; j++){
        int locRow = pair.first;
        localError   = max( localError, abs(T[locRow][j] - Tprev[locRow][j] ));
        Tprev[locRow][j] = T[locRow][j];
      }
    }
    MPI_Reduce( &localError, &globalError, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);//switch to all reduce?
    MPI_Bcast( &globalError, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //PRETTY PRINT
    if (iter % 100 == 0) {
      if (world_rank==0){
        printf("proc %d, iter %d, loc error %.5f, globalError: %.5f\n", world_rank, iter, localError, globalError);
        //track_progresion(T, iter);
      }
      if (world_rank==2){
        printf("iter %d, T[750][900]= %f, myError: %f, globalError: %.5f\n", iter, T[250][900], myError, globalError);
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
