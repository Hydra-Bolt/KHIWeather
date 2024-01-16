#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "apiparams.h"
// #include <termios.h>
// #include <unistd.h>

// struct APIParams
// {
//     float longitude;
//     float latitude;
//     int forecastDays;
//     char country[60];
//     char paramArray[3][60];
// };

// struct APIParams *params;
// This just dynamically writes the json and makes memory for it; issue maybe hidden
struct MemoryStruct
{
    char *memory;
    size_t size;
};

// functions for taking input in special way
// void setBufferedInput(int enable)
// {
//     static struct termios oldt, newt;

//     if (!enable)
//     {
//         tcgetattr(STDIN_FILENO, &oldt);
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO);
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     }
//     else
//     {
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     }
// }

// function for dynamic malloc
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
// removing a pramater when selected
// void remove_parameter(char *status, struct APIParams *params, const char *paramName, int *length)
// {
//     // Iterate through the parameter array to find and remove the specified parameter
//     for (int i = 0; i < *length; i++)
//     {
//         if (strcmp(params->paramArray[i], paramName) == 0)
//         {
//             // Shift elements to fill the gap
//             for (int j = i; j < *length - 1; j++)
//             {
//                 strcpy(params->paramArray[j], params->paramArray[j + 1]);
//             }

//             // Update status and reduce the length
//             strcpy(status, "off");
//             (*length)--;
//             printf("%s has been removed\n", paramName);
//             return;
//         }
//     }

//     printf("%s not found in the parameter list\n", paramName);
// }

// void input_api_params(struct APIParams *params)
// {
//     setBufferedInput(0);
//     char temp[6] = "off", hum[6] = "off", win[6] = "off", pre[6] = "off", clo[6] = "off";
//     int option;
//     int length = 0; // Counter for the number of added parameters

//     do
//     {
//         // printf("\n\nTell us what to show and what not to show.\n");
//         printf("\n\nPress the corresponding key to add it to analysis.\n");
//         printf("Press the corresponding key again to remove it:\n");
//         printf("1. Temperature %s\n2. Humidity %s\n3. Wind Speed %s\n4. Precipitation Chance %s\n5. Cloud Cover %s\n", temp, hum, win, pre, clo);

//         option = getchar() - '0';

//         switch (option)
//         {
//         case 1:
//             if (strcmp(temp, "added") != 0)
//             {
//                 printf("\n\nTemperature has been added\n\n");
//                 strncpy(params->paramArray[length], "temperature_2m", 60);
//                 strcpy(temp, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(temp, params, "temperature_2m", &length);
//             }
//             break;
//         case 2:
//             // Similar changes for other cases
//             if (strcmp(hum, "added") != 0)
//             {
//                 printf("\n\nHumidity has been added\n");
//                 strncpy(params->paramArray[length], "relative_humidity_2m", 60);
//                 strcpy(hum, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(hum, params, "relative_humidity_2m", &length);
//             }
//             break;
//         case 3:
//             // Similar changes for other cases
//             if (strcmp(win, "added") != 0)
//             {
//                 printf("\n\nWind Speed has been added\n\n");
//                 strncpy(params->paramArray[length], "wind_speed_10m", 60);
//                 strcpy(win, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(win, params, "wind_speed_10m", &length);
//             }
//             break;
//         case 4:
//             // Similar changes for other cases
//             if (strcmp(pre, "added") != 0)
//             {
//                 printf("\n\nPrecipitation Probability has been added\n\n");
//                 strncpy(params->paramArray[length], "precipitation_probability", 60);
//                 strcpy(pre, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(pre, params, "precipitation_probability", &length);
//             }
//             break;
//         case 5:
//             // Similar changes for other cases
//             if (strcmp(clo, "added") != 0)
//             {
//                 printf("\n\nCloud Cover has been added\n\n");
//                 strncpy(params->paramArray[length], "cloud_cover", 60);
//                 strcpy(clo, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(clo, params, "cloud_cover", &length);
//             }
//             break;
//         default:
//             printf("Invalid, Try again.\n");
//         }

//     } while (length < 3); // Use the predefined maximum number of parameters
//     setBufferedInput(1);
// }

char *create_api_url(struct APIParams *params)
{
    char api_url[] = "https://api.open-meteo.com/v1/forecast";
    char api_params[256];
    // Avoiding buffer overflow if the user is stupid
    snprintf(api_params, sizeof(api_params), "latitude=%.2f&longitude=%.2f&forecast_days=%d&hourly=%s,%s,%s", params->latitude, params->longitude, params->forecastDays, params->paramArray[0], params->paramArray[1], params->paramArray[2]);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s?%s", api_url, api_params);
    return strdup(full_url);
}

void analyze_temperature(double *tempArray, int arraySize)
{
    printf("Temperature Analysis:\n");

    if (arraySize % 24 != 0)
    {
        printf("Error: Array size is not a multiple of 24\n");
        return;
    }

    double maxTemp = tempArray[0], minTemp = tempArray[0];
    for (int i = 0; i < arraySize / 24; i++)
    {
        double sum = 0;
        for (int j = 0; j < 24; j++)
        {
            double tempValue = tempArray[(i * 24) + j];
            sum += tempValue;
            if (tempValue > maxTemp)
            {
                maxTemp = tempValue;
            }
            if (tempValue < maxTemp)
            {
                minTemp = tempValue;
            }
        }
        double average = sum / 24;
        printf("Day %d: The Average Temperature %.2f\n", i + 1, average);
    }
    printf("Peak Temprature %.2f, Lowest Temprature %.2f\n", maxTemp, minTemp);
}

void analyze_humidity(double *humidArray, int arraySize)
{
    printf("Humidity Analysis:\n");

    if (arraySize % 24 != 0)
    {
        printf("Error: Array size is not a multiple of 24\n");
        return;
    }

    double maxHumidity = humidArray[0], minHumidity = humidArray[0];
    double sum = 0;

    // Additional variables for trends and correlation
    int increasingTrendCount = 0;
    int decreasingTrendCount = 0;
    int comfortableCount = 0;
    int correlatedCount = 0;

    for (int i = 0; i < arraySize; i++)
    {
        sum += humidArray[i];

        if (humidArray[i] > maxHumidity)
        {
            maxHumidity = humidArray[i];
        }

        if (humidArray[i] < minHumidity)
        {
            minHumidity = humidArray[i];
        }

        // Check for trends
        if (i > 0)
        {
            if (humidArray[i] > humidArray[i - 1])
            {
                increasingTrendCount++;
            }
            else if (humidArray[i] < humidArray[i - 1])
            {
                decreasingTrendCount++;
            }
        }

        // Check for comfort levels (assuming a range of 30% to 70% is comfortable)
        if (humidArray[i] >= 30 && humidArray[i] <= 70)
        {
            comfortableCount++;
        }
    }

    double averageHumidity = sum / arraySize;

    printf("Average Humidity: %.2f%%\n", averageHumidity);
    printf("Peak Humidity: %.2f%%, Lowest Humidity: %.2f%%\n", maxHumidity, minHumidity);

    // Print trends
    printf("Relative Humidity Trends:\n");
    printf("Increasing Trends: %d\n", increasingTrendCount);
    printf("Decreasing Trends: %d\n", decreasingTrendCount);

    // Print comfort levels
    printf("Comfort Levels:\n");
    printf("Comfortable Hours: %d\n", comfortableCount);
}
void analyze_wind(double *windArray, int arraySize)
{
    printf("Wind Speed Analysis:\n");

    double maxWindSpeed = windArray[0];
    int maxWindSpeedIndex = 0;

    for (int i = 0; i < arraySize; i++)
    {
        // Identify the highest wind speeds
        if (windArray[i] > maxWindSpeed)
        {
            maxWindSpeed = windArray[i];
            maxWindSpeedIndex = i;
        }
    }

    // Print the results
    printf("Peak Wind Speeds:\n");
    printf("Maximum Wind Speed: %.2f m/s at on %d hour of the %d day\n", maxWindSpeed, maxWindSpeedIndex % 24, maxWindSpeedIndex / 24 + 1);
}

void analyze_rain(double *precepArray, int arraySize)
{
    printf("Precipitation Probability Analysis:\n");
    for (int i = 0; i < arraySize/ 24; i++)
    {
        double sum = 0;
        for (int j = 0; j < 24; j++)
        {
            // printf("%.2f: %d Day %d Hour Rain Chances\n", precepArray[i], i % 24, i / 24 + 1);
            double tempValue = precepArray[(i * 24) + j];
            sum += tempValue;
        }
        double average = sum / 24;
        printf("%.2f: %d Day Rain Chances\n", precepArray[i], i);
    }
    printf("\n");
}

void analyze_clouds(double *cloudArray, int arraySize)
{
    printf("Cloud Cover Analysis:\n");
    for (int i = 0; i < arraySize / 24; i++)
    {
        double sum = 0;
        for (int j = 0; j < 24; j++)
        {
            double tempValue = cloudArray[(i * 24) + j];
            sum += tempValue;
        }
        double average = sum / 24;
        if (average < 25)
        {
            printf("Scatterd Clouds for the %d Day\n", i+1);
        }
        else if (average < 50)
        {
            printf("Mostly Clouds for the %d Day\n", i+1);
        }
        else if (average < 75)
        {
            printf("Broken Clouds for the %d Day\n", i+1);
        }
        else if (average <= 100)
        {
            printf("Overcast with a chance of rain %d Day\n", i +1);
        }
    }
    printf("\n");
}

void process_json(char *chunk, FILE *rawData, FILE *procFile, struct APIParams *params)
{
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
            for (int i = 0; i < 3; i++)
            {
                printf("\nNOW OUTPUTING %s\n", params->paramArray[i]);
                cJSON *subArray = cJSON_GetObjectItemCaseSensitive(hourly, params->paramArray[i]);
                if (cJSON_IsArray(subArray))
                {
                    int arraySize = cJSON_GetArraySize(subArray);
                    double outArray[arraySize];
                    for (int j = 0; j < arraySize; j++)
                    {
                        cJSON *param = cJSON_GetArrayItem(subArray, j);
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
                        analyze_temperature(outArray, arraySize);
                    }
                    else if (strcmp(params->paramArray[i], "relative_humidity_2m") == 0)
                    {
                        analyze_humidity(outArray, arraySize);
                    }
                    else if (strcmp(params->paramArray[i], "wind_speed_10m") == 0)
                    {
                        analyze_wind(outArray, arraySize);
                    }
                    else if (strcmp(params->paramArray[i], "precipitation_probability") == 0)
                    {
                        analyze_rain(outArray, arraySize);
                    }
                    else if (strcmp(params->paramArray[i], "cloud_cover") == 0)
                    {
                        analyze_clouds(outArray, arraySize);
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
}

int main(void)
{
    CURL *curl;
    CURLcode res;

    FILE *procData = fopen("processData.txt", "w");
    if (procData == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    FILE *rawData = fopen("rawData.json", "a");
    if (rawData == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    struct APIParams *params = malloc(sizeof(struct APIParams));

    if (params == NULL)
    {
        printf("Error when allocating memory!");
    }
    for (int i = 0; i < 3; i++)
    {
        params->paramArray[i][0] = '\0'; // Initialize each string to an empty string
    }
    // Sannan here Implement the input taking from the user:
    // Fill up the input taking part and design the front end
    printf("Welcome to KHI Weather Reporting System\n Where we bring you the latest weather report around the world\n");
    printf("For now we bring the weather report from the follwoing countries only select your country\n");
    printf("1. Karachi\n2. London\n3. Moscow\n4. Tel Aviv\n5. Beijing\n6. Dehli\n7. Sydney\n8. Washington D.C\n");
    setBufferedInput(0);
    int option;
    option = getchar() - '0';
    option--;
    setBufferedInput(1);
    switch (option)
    {
    case 0:
        params->latitude = 24.8607;
        params->longitude = 67.0011;
        printf("You selected Karachi.\n");
        break;
    case 1:
        params->latitude = 51.509865;
        params->longitude = -0.118092; // London
        printf("You selected London.\n");
        break;
    case 2:
        params->latitude = 55.7558;
        params->longitude = 37.6176; // Moscow
        printf("You selected Moscow.\n");
        break;
    case 3:
        params->latitude = 32.0853;
        params->longitude = 34.7818; // Ankara
        printf("You selected Ankara.\n");
        break;
    case 4:
        params->latitude = 39.9042;
        params->longitude = 116.4074; // Beijing
        printf("You selected Beijing.\n");
        break;
    case 5:
        params->latitude = 28.6139;
        params->longitude = 77.2090; // Delhi
        printf("You selected Delhi.\n");
        break;
    case 6:
        params->latitude = -33.8688;
        params->longitude = 151.2093; // Sydney
        printf("You selected Sydney.\n");
        break;
    case 7:
        params->latitude = 38.8951;
        params->longitude = -77.0364; // Washington D.C.
        printf("You selected Washington D.C..\n");
        break;
    default:
        printf("Invalid option. Please enter a number between 0 and 8.\n");
    };
    // setBufferedInput(1);
    printf("How many days to forcast?: ");
    scanf("%d", &params->forecastDays);

    printf("Tell us what to show and what not to show");

    input_api_params(params);
    char *full_url = create_api_url(params);
    printf("%s\n", full_url);
    struct MemoryStruct chunk;

    chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;                   /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    // curl_handle = curl_easy_init();

    curl = curl_easy_init();
    if (curl)
    {
        // char *chunk = NULL;

        // Set the callback function to handle the received data
        /* specify URL to get */
        // curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_URL, full_url);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            // fprintf(rawData, "%s", json_buffer);
            process_json(chunk.memory, rawData, procData, params);
        }

        // Cleanup
        curl_free(chunk.memory);
        curl_easy_cleanup(curl);
    }
    // free(full_url);
    free(params);
    fclose(procData);
    fclose(rawData);
    curl_global_cleanup();

    return 0;
}
