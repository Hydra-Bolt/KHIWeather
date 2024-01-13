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
#include "emailanomalies.h"
#include "processdata.h"
#include "weathercast.h"


int main(int argc, char *argv[])
{
    if (argc < 7)
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

        forecast_weather(params, argv);
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
        forecast_weather(params, argv);
        break;
    default:
        printf("Invalid selection of weather cast");
        break;
    }

    // free(full_url);
    free(params);

    return 0;
}
