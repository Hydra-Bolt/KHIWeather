#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <hpdf.h>
#include <string.h>
#include "apiparams.h"
#include "analysis.h"

void process_json(char *chunk, FILE *rawData, FILE *procFile, struct APIParams *params)
{
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        fprintf(stderr, "Error: Cannot create PDF document\n");
        return;
    }
    // printf("%s", chunk);
    cJSON *root = cJSON_Parse(chunk);
    if (root != NULL)
    {
        char *formattedJson = cJSON_Print(root);

        // Print or save the formatted JSON
        fprintf(rawData, "%s\n", formattedJson);
        cJSON *hourly = cJSON_GetObjectItemCaseSensitive(root, "hourly");
        if (cJSON_IsObject(hourly))
        {
            cJSON *subTimeArray = cJSON_GetObjectItemCaseSensitive(hourly, "time");
            int arraySize = cJSON_GetArraySize(subTimeArray);
            cJSON *final = cJSON_GetArrayItem(subTimeArray, arraySize - 1);
            cJSON *first = cJSON_GetArrayItem(subTimeArray, 0);
            FILE *anomaly = fopen("anomalies.log", "a");
            FILE *temp = fopen("emailanomalies.txt", "w");
            fprintf(anomaly, "\nAnomlies detected from %s to %s:\n\n", first->valuestring, final->valuestring);
            fclose(anomaly);
            fclose(temp);
            for (int i = 0; i < 3; i++)
            {
                // printf("\nNOW OUTPUTING %s\n", params->paramArray[i]);
                cJSON *subArray = cJSON_GetObjectItemCaseSensitive(hourly, params->paramArray[i]);
                if (cJSON_IsArray(subArray))
                {
                    double outArray[arraySize];
                    double timeArray[arraySize];
                    for (int j = 0; j < arraySize; j++)
                    {
                        cJSON *param = cJSON_GetArrayItem(subArray, j);
                        timeArray[j] = j;
                        if (cJSON_IsNumber(param))
                        {
                            fprintf(procFile, "%.2f\n", param->valuedouble);
                            outArray[j] = param->valuedouble;
                        }
                        else if (cJSON_IsString(param))
                        {
                            fprintf(procFile, "%s\n", param->valuestring);
                        }
                    }
                    if (strcmp(params->paramArray[i], "temperature_2m") == 0)
                    {
                        analyze_temperature(outArray, arraySize, timeArray, pdf);
                    }
                    else if (strcmp(params->paramArray[i], "relative_humidity_2m") == 0)
                    {
                        analyze_humidity(outArray, arraySize, timeArray, pdf);
                    }
                    else if (strcmp(params->paramArray[i], "wind_speed_10m") == 0)
                    {
                        analyze_wind(outArray, arraySize, timeArray, pdf);
                    }
                    else if (strcmp(params->paramArray[i], "precipitation_probability") == 0)
                    {
                        analyze_rain(outArray, arraySize, timeArray, pdf);
                    }
                    else if (strcmp(params->paramArray[i], "cloud_cover") == 0)
                    {
                        analyze_clouds(outArray, arraySize, timeArray, pdf);
                    }
                    else
                    {
                        fprintf(stderr, "Unknown parameter: %s\n", params->paramArray[i]);
                    }
                }
                else
                {
                    fprintf(stderr, "Temperature array not found in JSON\n");
                }
            }
        }
        else
        {
            fprintf(stderr, "Hourly data not found in JSON\n");
        }

        cJSON_Delete(root);
    }
    else
    {
        fprintf(stderr, "Failed to parse JSON\n");
    }
    HPDF_SaveToFile(pdf, "output.pdf");

    // Clean up
    HPDF_Free(pdf);

    // retu12rn EXIT_SUCCESS;
}