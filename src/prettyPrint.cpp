#include "global.h"
#include <iostream>
#include <mpi.h>

using namespace std;

void prettyPrint (double (*Tout)[COLS+2], map<int,int> local2global) {
  MPI_Barrier(MPI_COMM_WORLD);
  for (int proc = 0; proc < world_size; proc ++) {
    if (world_rank == proc) {
      for (auto& i : local2global ) {
        for (int j = 0; j < COLS+2; j++) {
          printf("%3.2f\t", Tout[i.first][j]);
        }
        cout << endl;
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
}
