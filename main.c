#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "apiparams.h"
#include "analysis.h"

struct MemoryStruct
{
    char *memory;
    size_t size;
};


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
                // cJSON *subTimeArray = cJSON_GetObjectItemCaseSensitive(hourly, "time");
                if (cJSON_IsArray(subArray))
                {
                    int arraySize = cJSON_GetArraySize(subArray);
                    double outArray[arraySize];
                    double timeArray[arraySize];
                    for (int j = 0; j < arraySize; j++)
                    {
                        cJSON *param = cJSON_GetArrayItem(subArray, j);
                        // cJSON *time = cJSON_GetArrayItem(subTimeArray, j)
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
                        analyze_temperature(outArray, arraySize, timeArray);
                    }
                    else if (strcmp(params->paramArray[i], "relative_humidity_2m") == 0)
                    {
                        analyze_humidity(outArray, arraySize, timeArray);
                    }
                    else if (strcmp(params->paramArray[i], "wind_speed_10m") == 0)
                    {
                        analyze_wind(outArray, arraySize, timeArray);
                    }
                    else if (strcmp(params->paramArray[i], "precipitation_probability") == 0)
                    {
                        analyze_rain(outArray, arraySize, timeArray);
                    }
                    else if (strcmp(params->paramArray[i], "cloud_cover") == 0)
                    {
                        analyze_clouds(outArray, arraySize, timeArray);
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
        params->longitude = 34.7818; // Tel Aviv
        printf("You selected Tel Aviv.\n");
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

    curl = curl_easy_init();
    if (curl)
    {
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
