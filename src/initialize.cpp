#include <cstddef>
#include <iostream>
#include "global.h"

void initialize (double **Tprev) {
  std::cout << __func__ <<  std::endl;

  int i, j;

  // Testing
  std::cout << Tprev[0][0];

  /*
  for (int i = 0; i <= rows + 1; i++) {
    for (j = 0; j <= cols; j++) {
      Tprev[i][j] = 0.0;
    }
  }

  // BCs
  // left  <- 0.0
  // right <- linear increase
  for (i = 0; i <= rows+1; i++) {
    Tprev[i][0] = 0.0;
    Tprev[i][cols+1] = (100.0/rows)*i;
  }

  // top <- 0.0
  // bot <- linear increase
  for (j = 0; j <= rows+1; j++) {
    Tprev[0][j] = 0.0;
    Tprev[rows+1][j] = (100.0/cols)*j;
  }
  */
}
