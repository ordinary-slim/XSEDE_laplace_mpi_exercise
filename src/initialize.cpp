#include <cstddef>
#include <iostream>
#include "global.h"

void initialize (double (*Tinit)[COLS+2]) {
  std::cout << __func__ <<  std::endl;

  for (int i = 0; i <= ROWS + 1; i++) {
    for (int j = 0; j <= COLS; j++) {
      Tinit[i][j] = 0.0;
    }
  }

  // BCs
  // left  <- 0.0, right <- linear increase
  for (int i = 0; i <= ROWS+1; i++) {
    Tinit[i][0] = 0.0;
    Tinit[i][COLS+1] = (100.0/ROWS)*i;
  }

  // top <- 0.0, bot <- linear increase
  for (int j = 0; j <= ROWS+1; j++) {
    Tinit[0][j] = 0.0;
    Tinit[ROWS+1][j] = (100.0/COLS)*j;
  }
}
