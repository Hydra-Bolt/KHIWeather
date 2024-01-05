// plotter.h

#ifndef PLOTTING_H
#define PLOTTING_H

#include <plplot/plplot.h>

PLFLT* convertDoubleArrayToPLFLTArray(double *inputArray, int size);

void plotGraph(double *x, double *y, char *x_label, char *y_label, int n);

#endif // PLOTTER_H
