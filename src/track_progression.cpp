#include <iostream>
#include "global.h"

void track_progresion(int iter){

  printf( "-- Iteration: %d --\n", iter);
  for (int i = ROWS - 5; i <= ROWS; i++) {
    printf( "[%d, %d]: %5.2f ", i, i, T[i][i]);
  }
  printf("\n");
}
