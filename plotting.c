#include <plplot/plplot.h>
#include <stdlib.h>

PLFLT* convertDoubleArrayToPLFLTArray(double *inputArray, int size) {
    PLFLT *outputArray = (PLFLT *)malloc(size * sizeof(PLFLT));

    if (outputArray != NULL) {
        for (int i = 0; i < size; ++i) {
            outputArray[i] = (PLFLT)inputArray[i];
        }
    }

    return outputArray;
}

void plotGraph(double *x, double *y, char *x_label, char *y_label, int n) {
    // Convert double arrays to PLFLT arrays
    PLFLT *plflt_x = convertDoubleArrayToPLFLTArray(x, n);
    PLFLT *plflt_y = convertDoubleArrayToPLFLTArray(y, n);
    double min_y = y[0];
    double max_y = y[0];
    for (int i = 1; i < n; ++i) {
        if (y[i] < min_y) min_y = y[i];
        if (y[i] > max_y) max_y = y[i];
    }
    // Create a new plot
    char out[50];
    snprintf(out, sizeof(out), "./Graphs/%s.jpg", y_label);
    plsdev("jpgqt");
    c_plsetopt("DEV", "jpgqt"); // Set the device to JPEG
    c_plsetopt("o", out); // Set the output file name

    plinit();

    // Set background color to white
    c_plscolbg(255, 255, 255); 

    // Set text and line color to black
    plcol0(0); 

    // Set up viewport and window
    plenv(0.0, n + 1, min_y-3, max_y+3, 0, 0);

    // Set the axis labels
    char plot_label[100]; 

    snprintf(plot_label, sizeof(plot_label), "The plot of %s with respect to %s", y_label, x_label);

    pllab(x_label, y_label, plot_label);
    // Plot data
    plline(n, plflt_x, plflt_y);

    // Free the allocated memory
    free(plflt_x);
    free(plflt_y);

    plend();
}