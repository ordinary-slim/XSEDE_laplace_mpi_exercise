#include "global.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

void write_vtk(double (*Tout)[COLS+2], int iter) {
  /* Write legacy vtk for postprocessing
  No need for inputs since solution is a global variable
  */
  ofstream outfile;
  string fileName = "out_" + to_string(iter) + ".vtk";
  outfile.open (fileName);

  // HEADER
  outfile << "# vtk DataFile Version 3.0\n";
  // TITLE
  outfile << "My solution to this XSEDE MPI Laplace exercise\n";
  //DATA TYPE/FORMAT
  outfile << "ASCII\n";

  //TOPOLOGY
  outfile << "DATASET STRUCTURED_GRID\n";
  outfile << "DIMENSIONS " << ROWS+2 << " " << COLS+2 << " " << "1\n";
  outfile << "POINTS " << (ROWS+2)*(COLS+2) << " float\n";
  //assuming domain is 1x1
  for (int i = 0; i < (ROWS+2); i++) {
    for (int j = 0; j < (COLS+2); j++) {
      outfile << static_cast<float>(i)/(ROWS+2) << " " << static_cast<float>(j)/(COLS+2) << " 0.0\n";
    }
  }
  //ATTRIBUTES (temperature field)
  outfile << "POINT_DATA " << (ROWS+2)*(COLS+2) << endl;
  outfile << "SCALARS Temperature float 1\n";
  outfile << "LOOKUP_TABLE default\n";
  for (int i = 0; i < (ROWS+2); i++) {
    for (int j = COLS+1; j >= 0 ; j--) {
      outfile << round(Tout[i][j]*1e5) / 1e5 << endl;
    }
  }

  outfile.close();
  cout << "Wrote " << fileName << endl;
}
