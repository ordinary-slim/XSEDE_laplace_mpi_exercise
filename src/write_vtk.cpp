#include "global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <map>

using namespace std;

void write_vtk(double (*Tout)[COLS+2], map<int,int> local2global) {
  /* Write legacy vtk for postprocessing
  No need for inputs since solution is a global variable
  */
  ofstream outfile;
  string fileName = "post/out_" + to_string(world_rank) + ".vtk";
  outfile.open (fileName);

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
  for (auto& i : local2global) {
    for (int j = 0; j < (COLS+2); j++) {
      outfile << static_cast<float>(i.second)/(ROWS+2) << " " << static_cast<float>(j)/(COLS+2) << " 0.0\n";
    }
  }
  //ATTRIBUTES (temperature field)
  outfile << "POINT_DATA " << local2global.size()*(COLS+2) << endl;
  outfile << "SCALARS Temperature float 1\n";
  outfile << "LOOKUP_TABLE default\n";
  for (auto& i : local2global) {
    for (int j = COLS+1; j >= 0 ; j--) {
      outfile << round(Tout[i.first][j]*1e5) / 1e5 << endl;
    }
  }

  outfile.close();
  cout << "Wrote " << fileName << endl;
}
