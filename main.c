#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <hpdf.h>
#include "apiparams.h"
#include "analysis.h"

struct MemoryStruct
{
    char *memory;
    size_t size;
};

void process_json(char *chunk, FILE *rawData, FILE *procFile, struct APIParams *params);

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

void forcast_weather(struct APIParams *params)
{
    CURL *curl;
    CURLcode res;
    // Truncating the file before adding reports
    FILE *report = fopen("report.txt", "w");
    fclose(report);
    FILE *procData = fopen("processData.txt", "w");
    if (procData == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    FILE *rawData = fopen("rawData.json", "a");
    if (rawData == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    // printf("How many days to forcast?: ");
    // scanf("%d", &params->forecastDays);
    
    // printf("Tell us what to show and what not to show");
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
    fclose(procData);
    fclose(rawData);
    curl_global_cleanup();
}

void get_current_weather(float latitude, float longitude)
{
    CURL *curl;
    CURLcode res;

    // API endpoint for current weather
    char api_url[256];
    snprintf(api_url, sizeof(api_url), "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,weather_code,relative_humidity_2m,wind_speed_10m,precipitation", latitude, longitude);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    struct MemoryStruct chunk;

    chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;                   /* no data at this point */
    if (curl)
    {
        // Set the API URL
        curl_easy_setopt(curl, CURLOPT_URL, api_url);

        // Create a buffer to store the response
        // char response_buffer[4096] = "";/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
        {
            // Parse the JSON response using cJSON
            cJSON *root = cJSON_Parse(chunk.memory);
            cJSON *current = cJSON_GetObjectItemCaseSensitive(root, "current");
            cJSON *temperature = cJSON_GetObjectItemCaseSensitive(current, "temperature_2m");
            cJSON *weather_code = cJSON_GetObjectItemCaseSensitive(current, "weather_code");
            cJSON *humidity = cJSON_GetObjectItemCaseSensitive(current, "relative_humidity_2m");
            cJSON *wind_speed = cJSON_GetObjectItemCaseSensitive(current, "wind_speed_10m");
            cJSON *precipitation = cJSON_GetObjectItemCaseSensitive(current, "precipitation");

            // Initialize ncurses
            initscr();
            start_color();
            init_pair(1, COLOR_CYAN, COLOR_BLACK);
            init_pair(2, COLOR_YELLOW, COLOR_BLACK);
            init_pair(3, COLOR_GREEN, COLOR_BLACK);
            init_pair(4, COLOR_RED, COLOR_BLACK);

            // Display weather details with colors
            printw("Press Enter to exit\n");
            printw("Current Weather Details\n");
            printw("------------------------\n");
            printw("Temperature: %.2f°C\n", cJSON_GetNumberValue(temperature));
            printw("Relative Humidity: %.2f%%\n", cJSON_GetNumberValue(humidity));
            printw("Wind Speed: %.2f km/h\n", cJSON_GetNumberValue(wind_speed));
            printw("Precipitation: %.2f mm\n", cJSON_GetNumberValue(precipitation));

            // Display weather condition with color based on weather code
            int weather_code_value = cJSON_GetNumberValue(weather_code);
            attron(COLOR_PAIR(1));
            printw("Weather Condition: ");
            if (weather_code_value == 0)
            {
                printw("Clear sky");
            }
            else if (weather_code_value >= 1 && weather_code_value <= 3)
            {
                printw("Partly Cloudy");
            }
            else if (weather_code_value >= 45 && weather_code_value <= 48)
            {
                printw("Fog");
            }
            else if (weather_code_value >= 51 && weather_code_value <= 55)
            {
                printw("Drizzle");
            }
            else if (weather_code_value >= 61 && weather_code_value <= 65)
            {
                printw("Rain");
            }
            else if (weather_code_value >= 71 && weather_code_value <= 75)
            {
                printw("Snowfall");
            }
            else if (weather_code_value == 77)
            {
                printw("Snow grains");
            }
            else if ((weather_code_value >= 80 && weather_code_value <= 82) || (weather_code_value >= 85 && weather_code_value <= 86))
            {
                printw("Rain Showers");
            }
            else if (weather_code_value == 95 || weather_code_value == 96 || weather_code_value == 99)
            {
                printw("Thunderstorm");
            }
            else
            {
                printw("Unknown");
            }
            attroff(COLOR_PAIR(1));

            // Refresh the screen and wait for user input
            refresh();
            getch();

            // Cleanup cJSON and ncurses
            cJSON_Delete(root);
            endwin();
        }

        // Cleanup libcurl
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}

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
            fprintf(anomaly, "Anomlies detected from %s to %s:\n\n", first->valuestring, final->valuestring);
            fclose(anomaly);
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

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("City set to Karachi\n");
        printf("Forecasting temperature, precipitation, and humidity up to 7 days\n");
        
        struct APIParams *params = malloc(sizeof(struct APIParams));

        params->longitude = 67.0001;
        params->latitude = 24.86;
        params->forecastDays = 7;

        strcpy(params->paramArray[0], "temperature_2m");
        strcpy(params->paramArray[1], "precipitation_probability");
        strcpy(params->paramArray[2], "relative_humidity_2m");

        forcast_weather(params);
        free(params);
        return 0;
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
    int option;
    option = atoi(argv[1]);
    option--;
    // setBufferedInput();
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
        return 0;
    };
    int weather;
    weather = atoi(argv[2]);

    switch (weather)
    {
    case 1:
        get_current_weather(params->latitude, params->longitude);
        break;

    case 2:

        params->forecastDays = atoi(argv[3]);
        input_api_params(params, argv);
        forcast_weather(params);
    default:
        printf("Invalid selection of weather cast");
        break;
    }

    // free(full_url);
    free(params);

    return 0;
}
