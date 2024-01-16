#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
struct APIParams
{
    float longitude;
    float latitude;
    int forecastDays;
    char country[60];
    char paramArray[3][60];
};
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

void input_api_params(struct APIParams *params, char *argv[])
{
    int option;
    int length = 0; // Counter for the number of added parameters

    do
    {
        option = atoi(argv[4 + length]);

        switch (option)
        {
        case 1:
            printf("Temperature will be analyzed\n");
            strncpy(params->paramArray[length], "temperature_2m", 60);
            length++;
            break;
        case 2:
            // Similar changes for other cases

            printf("Humidity will be analyzed\n");
            strncpy(params->paramArray[length], "relative_humidity_2m", 60);
            length++;

            break;
        case 3:
            // Similar changes for other cases

            printf("Wind Speed will be analyzed\n");
            strncpy(params->paramArray[length], "wind_speed_10m", 60);
            length++;

            break;
        case 4:
            // Similar changes for other cases

            printf("Precipitation Probability will be analyzed\n");
            strncpy(params->paramArray[length], "precipitation_probability", 60);
            length++;

            break;
        case 5:
            // Similar changes for other cases

            printf("Cloud Cover will be analyzed\n");
            strncpy(params->paramArray[length], "cloud_cover", 60);
            length++;

            break;
        default:
            printf("%d invalid option for paramter, review documentation and retry.", option);
            break;
        }

    } while (length < 3); // Use the predefined maximum number of parameters
}