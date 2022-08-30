#ifndef global_included
#define global_included

#define ROWS 1000
#define COLS 1000
#define ROWSP2 (COLS+2)
#define COLSP2 (COLS+2)

extern const double maxError;
extern const double maxIter;

extern double T[ROWSP2][COLSP2];
extern double Tprev[ROWSP2][COLSP2];

#endif
