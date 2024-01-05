#ifndef ANALYSIS_H
#define ANALYSIS_H

void analyze_temperature(double *tempArray, int arraySize, double *timeArray);
void analyze_humidity(double *humidArray, int arraySize, double *timeArray);
void analyze_wind(double *windArray, int arraySize, double *timeArray);
void analyze_rain(double *precepArray, int arraySize, double *timeArray);
void analyze_clouds(double *cloudArray, int arraySize, double *timeArray);

#endif // ANALYSIS_H
