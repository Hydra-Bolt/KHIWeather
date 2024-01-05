#include <stdlib.h>
#include <stdio.h>
#include "plotting.h"

FILE *anomaliesFile = NULL;  // Declare the global variable

void initializeAnomaliesFile() {
    anomaliesFile = fopen("anomalies.txt", "a");
    if (anomaliesFile == NULL) {
        perror("Error opening anomalies.txt");
        // Handle the error as needed
    }
}
void analyze_temperature(double *tempArray, int arraySize, double *timeArray)
{
    initializeAnomaliesFile();
    printf("Temperature Analysis:\n");

    if (arraySize % 24 != 0)
    {
        printf("Error: Array size is not a multiple of 24\n");
        return;
    }

    // Define a threshold for temperature spikes
    double temperatureThresholdUpper = 40.0; // Adjust this threshold as needed
    double temperatureThresholdLower = -10.0;
    for (int i = 0; i < arraySize / 24; i++)
    {
        double maxTemp = tempArray[i * 24];
        double minTemp = tempArray[i * 24];
        double sum = 0;

        // Variable to track anomaly
        // int anomalyIndex = -1;

        for (int j = 0; j < 24; j++)
        {
            double tempValue = tempArray[(i * 24) + j];
            sum += tempValue;

            if (tempValue > maxTemp)
            {
                maxTemp = tempValue;
            }

            if (tempValue < minTemp)
            {
                minTemp = tempValue;
            }

            // Check for temperature spikes
            if (tempValue > temperatureThresholdUpper || tempValue < temperatureThresholdLower)
            {
                printf("Anomaly Detected - Day %d, Hour %d: Temperature Spike at %.2f degrees Celsius\n", i + 1, j + 1, tempValue);
                fprintf(anomaliesFile, "Anomaly Detected - Day %d, Hour %d: Temperature Spike at %.2f degrees Celsius\n", i + 1, j + 1, tempValue);

            }
        }

        double average = sum / 24;
        printf("Day %d: Average Temperature %.2f, Peak Temperature %.2f, Lowest Temperature %.2f\n", i + 1, average, maxTemp, minTemp);
    }
    plotGraph(timeArray, tempArray, "Time", "Temperature", arraySize);
}

void analyze_humidity(double *humidArray, int arraySize, double *timeArray)
{
    initializeAnomaliesFile();
    printf("Humidity Analysis:\n");

    if (arraySize % 24 != 0)
    {
        printf("Error: Array size is not a multiple of 24\n");
        return;
    }

    // Define a threshold for humidity spikes
    double humidityThreshold = 90.0; // Adjust this threshold as needed

    for (int i = 0; i < arraySize / 24; i++)
    {
        double maxHumidity = humidArray[i * 24];
        double minHumidity = humidArray[i * 24];
        double sum = 0;

        // Variable to track anomaly
        // int anomalyIndex = -1;

        for (int j = 0; j < 24; j++)
        {
            double humidityValue = humidArray[(i * 24) + j];
            sum += humidityValue;

            if (humidityValue > maxHumidity)
            {
                maxHumidity = humidityValue;
            }

            if (humidityValue < minHumidity)
            {
                minHumidity = humidityValue;
            }

            // Check for humidity spikes
            if (humidityValue > humidityThreshold)
            {
                printf("Anomaly Detected - Day %d, Hour %d: Humidity Spike at %.2f%%\n", i + 1, j + 1, humidityValue);
                fprintf(anomaliesFile,"Anomaly Detected - Day %d, Hour %d: Humidity Spike at %.2f%%\n", i + 1, j + 1, humidityValue);
                // anomalyIndex = (i * 24) + j;
            }
        }

        double averageHumidity = sum / 24;
        printf("Day %d: Average Humidity: %.2f%%, Peak Humidity: %.2f%%, Lowest Humidity: %.2f%%\n", i + 1, averageHumidity, maxHumidity, minHumidity);
    }
    plotGraph(timeArray, humidArray, "Time", "Humidity", arraySize);
}

void analyze_wind(double *windArray, int arraySize, double *timeArray)
{
    initializeAnomaliesFile();
    printf("Wind Speed Analysis:\n");

    // Define a threshold for wind speed spikes
    double windSpeedThreshold = 40.0; // Adjust this threshold as needed

    for (int i = 0; i < arraySize / 24; i++)
    {
        double maxWindSpeed = windArray[i * 24];
        double sumWindSpeed = 0;

        // Variable to track anomaly
        // int anomalyIndex = -1;

        for (int j = 0; j < 24; j++)
        {
            double windSpeedValue = windArray[(i * 24) + j];
            sumWindSpeed += windSpeedValue;

            // Identify the highest wind speeds
            if (windSpeedValue > maxWindSpeed)
            {
                maxWindSpeed = windSpeedValue;
            }

            // Check for wind speed spikes
            if (windSpeedValue > windSpeedThreshold)
            {
                printf("Anomaly Detected - Day %d, Hour %d: Wind Speed Spike at %.2f m/s\n", i + 1, j + 1, windSpeedValue);
                fprintf(anomaliesFile, "Anomaly Detected - Day %d, Hour %d: Wind Speed Spike at %.2f m/s\n", i + 1, j + 1, windSpeedValue);
                // anomalyIndex = (i * 24) + j;
            }
        }

        // Calculate average wind speed
        double averageWindSpeed = sumWindSpeed / 24;

        // Print the results
        printf("Day %d: Average Wind Speed: %.2f m/s, Maximum Wind Speed: %.2f m/s\n", i + 1, averageWindSpeed, maxWindSpeed);
    }
    plotGraph(timeArray, windArray, "Time", "Wind Speed", arraySize);
}


void analyze_rain(double *precepArray, int arraySize, double *timeArray)
{
    initializeAnomaliesFile();
    printf("Precipitation Probability Analysis:\n");

    // Define a threshold for precipitation spikes
    double precipitationThreshold = 80.0; // Adjust this threshold as needed

    for (int i = 0; i < arraySize / 24; i++)
    {
        double sum = 0;

        // Variable to track anomaly
        // int anomalyIndex = -1;

        for (int j = 0; j < 24; j++)
        {
            double precipitationValue = precepArray[(i * 24) + j];
            sum += precipitationValue;

            // Check for precipitation spikes
            if (precipitationValue > precipitationThreshold)
            {
                printf("Anomaly Detected - Day %d, Hour %d: Precipitation Spike at %.2f%%\n", i + 1, j + 1, precipitationValue);
                fprintf(anomaliesFile, "Anomaly Detected - Day %d, Hour %d: Precipitation Spike at %.2f%%\n", i + 1, j + 1, precipitationValue);

            }
        }

        double average = sum / 24;
        printf("Day %d: Average Rain Chances: %.2f%%\n", i + 1, average);

    }
    plotGraph(timeArray, precepArray, "Time", "Precipitation", arraySize);
}

void analyze_clouds(double *cloudArray, int arraySize, double *timeArray)
{
    initializeAnomaliesFile();
    printf("Cloud Cover Analysis:\n");

    // Define a threshold for cloud cover anomalies
    double cloudCoverThreshold = 90.0; // Adjust this threshold as needed

    for (int i = 0; i < arraySize / 24; i++)
    {
        double sum = 0;

        // Variable to track anomaly
        // int anomalyIndex = -1;

        for (int j = 0; j < 24; j++)
        {
            double cloudValue = cloudArray[(i * 24) + j];
            sum += cloudValue;

            // Check for cloud cover anomalies
            if (cloudValue > cloudCoverThreshold)
            {
                printf("Anomaly Detected - Day %d, Hour %d: High Cloud Cover at %.2f%%\n", i + 1, j + 1, cloudValue);
                fprintf(anomaliesFile, "Anomaly Detected - Day %d, Hour %d: High Cloud Cover at %.2f%%\n", i + 1, j + 1, cloudValue);
                // anomalyIndex = (i * 24) + j;
            }
        }

        double average = sum / 24;

        // Print the cloud cover category based on average percentage
        if (average < 25)
        {
            printf("Day %d: Scattered Clouds\n", i + 1);
        }
        else if (average < 50)
        {
            printf("Day %d: Mostly Cloudy\n", i + 1);
        }
        else if (average < 75)
        {
            printf("Day %d: Broken Clouds\n", i + 1);
        }
        else
        {
            printf("Day %d: Overcast with a chance of rain\n", i + 1);
        }
    }
    plotGraph(timeArray, cloudArray, "Time", "Cloud Coverage", arraySize);
}
