#include <plplot/plplot.h>
#include <stdlib.h>
#include <time.h>

// Function to generate random float values between min and max
float getRandomFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

void plotGraph(PLFLT *x, PLFLT *y1, PLFLT *y2, int n) {
    // Create a new plot
    plinit();

    // Set background color to white
    plscolbg(255, 255, 255);  // Set background color to white (RGB values: 255, 255, 255)

    // Set text and line color to black
    plcol0(0);  // Set the color index 0 to black

    // Set up viewport and window
    plenv(0.0, n + 1, 0.0, 100.0, 0, 0);

    // Set the axis labels
    pllab("Time", "Temperature", "Temperature and Precipitation Over Time");

    // Plot temperature data
    plline(n, x, y1);

    // Set line style for precipitation
    pllsty(2);
    // Plot precipitation data
    plline(n, x, y2);

    plend();
}


int main(int argc, char *argv[]) {
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Parse command-line options
    plparseopts(&argc, argv, PL_PARSE_FULL);

    // Use the provided extended temperature and precipitation data
    int n = 50;
    PLFLT time[50];
    PLFLT temperature[50];
    PLFLT precipitation[50];

    // Generate time array (1 to n)
    for (int i = 0; i < n; ++i) {
        time[i] = i + 1;

        // Generate random temperature values between 20 and 30
        temperature[i] = getRandomFloat(20.0, 30.0);

        // Generate random precipitation chance values between 0 and 100
        precipitation[i] = getRandomFloat(0.0, 100.0);
    }

    // Use the function to plot the graph
    plotGraph(time, temperature, precipitation, n);

    return 0;
}
