#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct APIParams
{
    float longitude;
    float latitude;
    int forecastDays;
    char country[60];
    char paramArray[3][60];
};
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
            printf("Temperature will be analyzed\n\n");
            strncpy(params->paramArray[length], "temperature_2m", 60);
            length++;
            break;
        case 2:
            // Similar changes for other cases

            printf("Humidity will be analyzed\n\n");
            strncpy(params->paramArray[length], "relative_humidity_2m", 60);
            length++;

            break;
        case 3:
            // Similar changes for other cases

            printf("Wind Speed will be analyzed\n\n");
            strncpy(params->paramArray[length], "wind_speed_10m", 60);
            length++;

            break;
        case 4:
            // Similar changes for other cases

            printf("Precipitation Probability will be analyzed\n\n");
            strncpy(params->paramArray[length], "precipitation_probability", 60);
            length++;

            break;
        case 5:
            // Similar changes for other cases

            printf("Cloud Cover will be analyzed\n\n");
            strncpy(params->paramArray[length], "cloud_cover", 60);
            length++;

            break;
        default:
            printf("Invalid, Try again.\n");
        }

    } while (length < 3); // Use the predefined maximum number of parameters
}