#include <cstddef>
#include <iostream>
#include "global.h"
#include <map>

using namespace std;

void initialize (double (*Tinit)[COLS+2], map<int,int> local2global) {
  std::cout << __func__ <<  std::endl;

  for (int i = 0; i < local2global.size()+2; i++) {
    for (int j = 0; j < COLS+2; j++) {
      Tinit[i][j] = 0.0;
    }
  }

  // BCs
  // left  <- 0.0, right <- linear increase
  for (auto& i : local2global) {
    Tinit[i.first][0] = 0.0;
    Tinit[i.first][COLS+1] = (100.0/ROWS)*i.second;
  }

  // top <- 0.0
  if (world_rank==0) {
    for (int j = 0; j < COLS+2; j++) {
      Tinit[1][j] = 0.0;
    }
  }

  // bot <- linear increase
  if (world_rank==world_size-1) {
    for (int j = 0; j < COLS+2; j++) {
      Tinit[local2global.size()][j] = (100.0/COLS)*j;
    }
  }
}
