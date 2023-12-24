#include "plot/pbPlots.h"
#include "plot/supportLib.h"

void DrawGraph(double* xData, double* yData, size_t numPoints, const char* title, const char* xLabel, const char* yLabel);

int main() {
    double timePoints[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    double temperatures[] = {20.5, 18.3, 25.0, 12.8, 27.2, 22.6, 16.7, 19.8, 23.4, 14.5, 28.1, 17.2, 21.9, 26.3, 13.4, 24.7, 15.6, 29.5, 11.1, 30.0, 20.0, 18.9, 22.0, 16.3};


    DrawGraph(timePoints, temperatures, sizeof(timePoints) / sizeof(timePoints[0]), "Temperature Plot", "Time", "Temperature (Celsius)");

    return 0;
}

void DrawGraph(double* xData, double* yData, size_t numPoints, const char* title, const char* xLabel, const char* yLabel) {
    StartArenaAllocator();

    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = CreateStringReference(L"", 0);

    ScatterPlotSeries *dataSeries = GetDefaultScatterPlotSeriesSettings();
    dataSeries->xs = xData;
    dataSeries->ys = yData;
    dataSeries->xsLength = numPoints;
    dataSeries->ysLength = numPoints;
    dataSeries->color = CreateRGBColor(1, 0, 0);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 800;
    settings->height = 600;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    settings->title = L"title";
    settings->xLabel = L"xLabel";
    settings->yLabel = L"yLabel";
    settings->scatterPlotSeries = &dataSeries;
    settings->scatterPlotSeriesLength = 1;

    _Bool success = DrawScatterPlot(canvasReference, settings->width, settings->height,
                                    xData, numPoints, yData, numPoints, errorMessage);

    if (success) {
        size_t length;
        ByteArray *pngData = ConvertToPNG(canvasReference->image);
        WriteToFile(pngData, "graph_plot.png");
        DeleteImage(canvasReference->image);
    } else {
        fprintf(stderr, "Error: ");
        for (int i = 0; i < errorMessage->stringLength; i++) {
            fprintf(stderr, "%c", errorMessage->string[i]);
        }
        fprintf(stderr, "\n");
    }

    FreeAllocations();
}
