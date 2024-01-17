#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processdata.h"
#include "emailanomalies.h"
#include "apiparams.h"


struct MemoryStruct {
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

void forecast_weather(struct APIParams *params, char *argv[])
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
    char *full_url = create_api_url(params);
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

    FILE *file = fopen("emailanomalies.txt", "r");

    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size == 0)
    {
        printf("No anomalies Detected!\n");
    }
    else
    {
        printf("Anomalies Detected!\nSending Report to %s\n", argv[7]);
        char *contents = malloc(size + 1);

        if (contents == NULL)
        {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }

        fread(contents, 1, size, file);
        contents[size] = '\0';

        sendEmailNotification("Anomaly Report", contents, argv[7], "./output.pdf");

        free(contents);
    }

    fclose(file);
}

// void get_current_weather(float latitude, float longitude)
// {
//     CURL *curl;
//     CURLcode res;

//     // API endpoint for current weather
//     char api_url[256];
//     snprintf(api_url, sizeof(api_url), "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,weather_code,relative_humidity_2m,wind_speed_10m,precipitation", latitude, longitude);

//     curl_global_init(CURL_GLOBAL_DEFAULT);
//     curl = curl_easy_init();

//     struct MemoryStruct chunk;

//     chunk.memory = (char *)malloc(1); /* will be grown as needed by the realloc above */
//     chunk.size = 0;                   /* no data at this point */
//     if (curl)
//     {
//         // Set the API URL
//         curl_easy_setopt(curl, CURLOPT_URL, api_url);

//         // Create a buffer to store the response
//         // char response_buffer[4096] = "";/
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

//         // Perform the HTTP request
//         res = curl_easy_perform(curl);

//         // Check for errors
//         if (res != CURLE_OK)
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//         else
//         {
//             // Parse the JSON response using cJSON
//             cJSON *root = cJSON_Parse(chunk.memory);
//             cJSON *current = cJSON_GetObjectItemCaseSensitive(root, "current");
//             cJSON *temperature = cJSON_GetObjectItemCaseSensitive(current, "temperature_2m");
//             cJSON *weather_code = cJSON_GetObjectItemCaseSensitive(current, "weather_code");
//             cJSON *humidity = cJSON_GetObjectItemCaseSensitive(current, "relative_humidity_2m");
//             cJSON *wind_speed = cJSON_GetObjectItemCaseSensitive(current, "wind_speed_10m");
//             cJSON *precipitation = cJSON_GetObjectItemCaseSensitive(current, "precipitation");

//             // Initialize ncurses
//             initscr();
//             start_color();
//             init_pair(1, COLOR_CYAN, COLOR_BLACK);
//             init_pair(2, COLOR_YELLOW, COLOR_BLACK);
//             init_pair(3, COLOR_GREEN, COLOR_BLACK);
//             init_pair(4, COLOR_RED, COLOR_BLACK);

//             // Display weather details with colors
//             printw("Press Enter to exit\n");
//             printw("Current Weather Details\n");
//             printw("------------------------\n");
//             printw("Temperature: %.2f°C\n", cJSON_GetNumberValue(temperature));
//             printw("Relative Humidity: %.2f%%\n", cJSON_GetNumberValue(humidity));
//             printw("Wind Speed: %.2f km/h\n", cJSON_GetNumberValue(wind_speed));
//             printw("Precipitation: %.2f mm\n", cJSON_GetNumberValue(precipitation));

//             // Display weather condition with color based on weather code
//             int weather_code_value = cJSON_GetNumberValue(weather_code);
//             attron(COLOR_PAIR(1));
//             printw("Weather Condition: ");
//             if (weather_code_value == 0)
//             {
//                 printw("Clear sky");
//             }
//             else if (weather_code_value >= 1 && weather_code_value <= 3)
//             {
//                 printw("Partly Cloudy");
//             }
//             else if (weather_code_value >= 45 && weather_code_value <= 48)
//             {
//                 printw("Fog");
//             }
//             else if (weather_code_value >= 51 && weather_code_value <= 55)
//             {
//                 printw("Drizzle");
//             }
//             else if (weather_code_value >= 61 && weather_code_value <= 65)
//             {
//                 printw("Rain");
//             }
//             else if (weather_code_value >= 71 && weather_code_value <= 75)
//             {
//                 printw("Snowfall");
//             }
//             else if (weather_code_value == 77)
//             {
//                 printw("Snow grains");
//             }
//             else if ((weather_code_value >= 80 && weather_code_value <= 82) || (weather_code_value >= 85 && weather_code_value <= 86))
//             {
//                 printw("Rain Showers");
//             }
//             else if (weather_code_value == 95 || weather_code_value == 96 || weather_code_value == 99)
//             {
//                 printw("Thunderstorm");
//             }
//             else
//             {
//                 printw("Unknown");
//             }
//             attroff(COLOR_PAIR(1));

//             // Refresh the screen and wait for user input
//             refresh();
//             getch();

//             // Cleanup cJSON and ncurses
//             cJSON_Delete(root);
//             endwin();
//         }

//         // Cleanup libcurl
//         curl_easy_cleanup(curl);
//         curl_global_cleanup();
//     }
// }
