#include <stdlib.h>
#include <stdio.h>
#include <hpdf.h>
#include "plotting.h"

FILE *anomaliesFile = NULL;
FILE *reportFile = NULL;
FILE *tempAnomaly = NULL;

void initializeFiles()
{
    anomaliesFile = fopen("anomalies.log", "a");
    if (anomaliesFile == NULL)
    {
        perror("Error opening anomalies.log");
        exit(EXIT_FAILURE);
    }

    tempAnomaly = fopen("emailanomalies.txt", "a");
    if (tempAnomaly == NULL)
    {
        perror("Error opening the temporary anomaly file.");
        exit(EXIT_FAILURE);
    }

    reportFile = fopen("report.txt", "a");
    if (reportFile == NULL)
    {
        perror("Error opening report.txt");
        exit(EXIT_FAILURE);
    }
}

void closeFiles()
{
    if (anomaliesFile != NULL)
    {
        fclose(anomaliesFile);
    }

    if (reportFile != NULL)
    {
        fclose(reportFile);
    }

    if (tempAnomaly != NULL)
    {
        fclose(tempAnomaly);
    }
}

void printAnomaly(const char *parameter, int day, int hour, double value)
{
    // printf("Anomaly Detected - Day %d, Hour %d: %s Spike at %.2f\n", day, hour, parameter, value);
    fprintf(anomaliesFile, "Anomaly Detected - Day %d, Hour %d: %s Spike at %.2f\n", day, hour, parameter, value);
    fprintf(tempAnomaly, "Anomaly Detected - Day %d, Hour %d: %s Spike at %.2f\n", day, hour, parameter, value);
}

void printReportLine(const char *parameter, int day, double average, double maxVal, double minVal)
{
    printf("Day %d: Average %s: %.2f, Peak %s: %.2f, Lowest %s: %.2f\n", day, parameter, average, parameter, maxVal, parameter, minVal);
    char reportline[200];
    snprintf(reportline, sizeof(reportline), "Day %d: Average %s: %.2f, Peak %s: %.2f, Lowest %s: %.2f\n", day, parameter, average, parameter, maxVal, parameter, minVal);
    fprintf(reportFile, "%s", reportline);
}

void analyze_generic(double *dataArray, int arraySize, double *timeArray, char *parameter, double threshold, HPDF_Doc pdf)
{
    initializeFiles();
    printf("-----------------------------------------------\n");
    printf("%s Analysis:\n\n", parameter);
    fprintf(reportFile, "%s Analysis:\n", parameter);
    fprintf(tempAnomaly, "%s Anomlies:\n", parameter);
    fprintf(anomaliesFile, "%s Anomlies:\n", parameter);

    HPDF_Page page = HPDF_AddPage(pdf);
    if (!page)
    {
        fprintf(stderr, "Error: Cannot add page\n");
        HPDF_Free(pdf);
        return;
    }

    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 40);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 180, 780, parameter);
    HPDF_Page_EndText(page);

    font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);
    HPDF_Page_BeginText(page);

    if (arraySize % 24 != 0)
    {
        printf("Error: Array size is not a multiple of 24\n");
        return;
    }

    double maxVal = dataArray[0];
    double minVal = dataArray[0];
    double accum = 0;

    for (int i = 0; i < arraySize / 24; i++)
    {
        double sum = 0;

        for (int j = 0; j < 24; j++)
        {
            double value = dataArray[(i * 24) + j];
            sum += value;

            if (value > maxVal)
            {
                maxVal = value;
            }

            if (value < minVal)
            {
                minVal = value;
            }

            if (value > threshold)
            {
                printAnomaly(parameter, i + 1, j + 1, value);
            }
        }

        double average = sum / 24;
        printReportLine(parameter, i + 1, average, maxVal, minVal);
        accum += average;

        char reportline[200];
        snprintf(reportline, sizeof(reportline), "Day %d: Average %s: %.2f, Peak %s: %.2f, Lowest %s: %.2f\n", i + 1, parameter, average, parameter, maxVal, parameter, minVal);
        HPDF_Page_TextOut(page, 50, 750 - 16 * i + 1, reportline);
    }

    double overallAverage = accum / (arraySize / 24);
    char reportline[200];
    snprintf(reportline, sizeof(reportline), "The Total Forecast Analysis for %s: \n", parameter);
    HPDF_Page_TextOut(page, 50, 750 - 17 * (arraySize / 24) + 1, reportline);
    snprintf(reportline, sizeof(reportline), "Average %s: %.2f, Peak %s: %.2f, Lowest %s: %.2f\n\n\n", parameter, overallAverage, parameter, maxVal, parameter, minVal);
    HPDF_Page_TextOut(page, 50, 750 - 17 * (arraySize / 24 + 1) + 1, reportline);

    HPDF_Page_EndText(page);

    plotGraph(timeArray, dataArray, "Time", parameter, arraySize);

    snprintf(reportline, sizeof(reportline), "./Graphs/%s.jpg", parameter);
    HPDF_Image image = HPDF_LoadJpegImageFromFile(pdf, reportline);
    if (image)
    {
        HPDF_REAL imageWidth = HPDF_Image_GetWidth(image);
        HPDF_REAL imageHeight = HPDF_Image_GetHeight(image);
        HPDF_REAL desiredWidth = 400.0;
        HPDF_REAL scaleFactor = desiredWidth / imageWidth;
        HPDF_Page_DrawImage(page, image, 90, 450 - 20 * (arraySize / 24) + 1, imageWidth * scaleFactor, imageHeight * scaleFactor);
    }

    closeFiles();
}

void analyze_temperature(double *tempArray, int arraySize, double *timeArray, HPDF_Doc pdf)
{
    analyze_generic(tempArray, arraySize, timeArray, "Temperature", 40.0, pdf);
}

void analyze_humidity(double *humidArray, int arraySize, double *timeArray, HPDF_Doc pdf)
{
    analyze_generic(humidArray, arraySize, timeArray, "Humidity", 90.0, pdf);
}

void analyze_wind(double *windArray, int arraySize, double *timeArray, HPDF_Doc pdf)
{
    analyze_generic(windArray, arraySize, timeArray, "Wind Speed", 40.0, pdf);
}

void analyze_rain(double *precepArray, int arraySize, double *timeArray, HPDF_Doc pdf)
{
    analyze_generic(precepArray, arraySize, timeArray, "Precipitation", 80.0, pdf);
}

void analyze_clouds(double *cloudArray, int arraySize, double *timeArray, HPDF_Doc pdf)
{
    analyze_generic(cloudArray, arraySize, timeArray, "Cloud Coverage", 90.0, pdf);
}
