#include "global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <map>

using namespace std;

void write_vtk(double (*Tout)[COLS+2], map<int,int> local2global) {
  /* Write legacy vtk for postprocessing*/
  ofstream outfile;
  string fileName = "post/out_" + to_string(world_rank) + ".vtk";
  outfile.open (fileName);

  //Extend local2global mapping (for postprocessing only)
  int partitionSize = local2global.size();
  local2global[0] = local2global[1] - 1;
  local2global[partitionSize+1] = local2global[partitionSize] + 1;

  // HEADER
  outfile << "# vtk DataFile Version 3.0\n";
  // TITLE
  outfile << "My solution to XSEDE MPI Laplace exercise (youtube video)\n";
  //DATA TYPE/FORMAT
  outfile << "ASCII\n";

  //TOPOLOGY
  outfile << "DATASET STRUCTURED_GRID\n";
  outfile << "DIMENSIONS " << local2global.size() << " " << COLS+2 << " " << "1\n";
  outfile << "POINTS " << local2global.size()*(COLS+2) << " float\n";
  //assuming domain is 1x1
  double deltaX = 1.0 / (ROWS+1);
  double deltaY = 1.0 / (COLS+1);
  for (int j = 0; j < (COLS+2); j++) {
    for (auto& i : local2global) {
      outfile << static_cast<float>(j)*deltaX << " " << (1 - static_cast<float>(i.second)*deltaY) << " 0.0\n";
    }
  }
  //ATTRIBUTES (temperature field)
  outfile << "POINT_DATA " << local2global.size()*(COLS+2) << endl;
  outfile << "SCALARS Temperature float 1\n";
  outfile << "LOOKUP_TABLE default\n";
  for (int j = 0; j < (COLS+2); j++) {
    for (auto& i : local2global) {
      outfile << round(Tout[i.first][j]*1e5) / 1e5 << endl;
    }
  }

  outfile.close();
  cout << "Wrote " << fileName << endl;
}
