#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apiparams.h"
#include "weathercast.h"

void printCityInfo(int option, struct APIParams *params) {
    switch (option) {
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
            params->latitude = 39.9334;
            params->longitude = 32.8597 // Ankara
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
            fprintf(stderr, "Invalid option. Please enter a number between 0 and 7.\n");
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        printf("Invalid number of arguments supplied, S")
        printf("City set to Karachi\n");
        printf("Forecasting temperature, precipitation, and humidity up to 7 days\n");

        struct APIParams *params = malloc(sizeof(struct APIParams));

        if (params == NULL) {
            fprintf(stderr, "Error when allocating memory!\n");
            exit(EXIT_FAILURE);
        }

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

    if (params == NULL) {
        fprintf(stderr, "Error when allocating memory!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        params->paramArray[i][0] = '\0'; // Initialize each string to an empty string
    }

    int option = atoi(argv[1]);
    option--;

    if (option < 0 || option > 7) {
        fprintf(stderr, "Invalid option. Please enter a number between 0 and 7.\n");
        free(params);
        exit(EXIT_FAILURE);
    }

    printCityInfo(option, params);

    int weather = atoi(argv[2]);

    switch (weather) {
        case 1:
            get_current_weather(params->latitude, params->longitude);
            break;
        case 2:
            params->forecastDays = atoi(argv[3]);
            input_api_params(params, argv);
            forecast_weather(params, argv);
            break;
        default:
            fprintf(stderr, "Invalid selection of weather cast\n");
            break;
    }

    free(params);

    return 0;
}
