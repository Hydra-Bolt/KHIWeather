#ifndef ANALYSIS_H
#define ANALYSIS_H

void analyze_temperature(double *tempArray, int arraySize, double *timeArray, HPDF_Doc pdf);
void analyze_humidity(double *humidArray, int arraySize, double *timeArray, HPDF_Doc pdf);
void analyze_wind(double *windArray, int arraySize, double *timeArray, HPDF_Doc pdf);
void analyze_rain(double *precepArray, int arraySize, double *timeArray, HPDF_Doc pdf);
void analyze_clouds(double *cloudArray, int arraySize, double *timeArray, HPDF_Doc pdf);

#endif // ANALYSIS_H
