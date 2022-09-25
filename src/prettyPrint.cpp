#include "global.h"
#include <iostream>
#include <mpi.h>

using namespace std;

void prettyPrint (int itask, double (*Tout)[COLS+2], map<int,int> local2global) {
  MPI_Barrier(MPI_COMM_WORLD);
  switch (itask) {
    case -1://Print all procs local data
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
      break;
    default://print local+ghost data of proc given by itask
      if (world_rank == itask) {
          printf("----PROC %d---------\n", itask);
          for (int row = 0; row < local2global.size()+2; row++ ) {
            for (int j = 0; j < COLS+2; j++) {
              printf("%3.2f\t", Tout[row][j]);
            }
            cout << endl;
          }
      }
      MPI_Barrier(MPI_COMM_WORLD);
      break;
  }
}
