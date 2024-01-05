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


void setBufferedInput(int enable)
{
    static struct termios oldt, newt;

    if (!enable)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    else
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void remove_parameter(char *status, struct APIParams *params, const char *paramName, int *length)
{
    // Iterate through the parameter array to find and remove the specified parameter
    for (int i = 0; i < *length; i++)
    {
        if (strcmp(params->paramArray[i], paramName) == 0)
        {
            // Shift elements to fill the gap
            for (int j = i; j < *length - 1; j++)
            {
                strcpy(params->paramArray[j], params->paramArray[j + 1]);
            }

            // Update status and reduce the length
            strcpy(status, "off");
            (*length)--;
            printf("%s has been removed\n", paramName);
            return;
        }
    }

    printf("%s not found in the parameter list\n", paramName);
}

void input_api_params(struct APIParams *params)
{
    setBufferedInput(0);
    char temp[6] = "off", hum[6] = "off", win[6] = "off", pre[6] = "off", clo[6] = "off";
    int option;
    int length = 0; // Counter for the number of added parameters

    do
    {
        // printf("\n\nTell us what to show and what not to show.\n");
        printf("\n\nPress the corresponding key to add it to analysis.\n");
        printf("Press the corresponding key again to remove it:\n");
        printf("1. Temperature %s\n2. Humidity %s\n3. Wind Speed %s\n4. Precipitation Chance %s\n5. Cloud Cover %s\n", temp, hum, win, pre, clo);

        option = getchar() - '0';

        switch (option)
        {
        case 1:
            if (strcmp(temp, "added") != 0)
            {
                printf("\n\nTemperature has been added\n\n");
                strncpy(params->paramArray[length], "temperature_2m", 60);
                strcpy(temp, "added");
                length++;
            }
            else
            {
                remove_parameter(temp, params, "temperature_2m", &length);
            }
            break;
        case 2:
            // Similar changes for other cases
            if (strcmp(hum, "added") != 0)
            {
                printf("\n\nHumidity has been added\n");
                strncpy(params->paramArray[length], "relative_humidity_2m", 60);
                strcpy(hum, "added");
                length++;
            }
            else
            {
                remove_parameter(hum, params, "relative_humidity_2m", &length);
            }
            break;
        case 3:
            // Similar changes for other cases
            if (strcmp(win, "added") != 0)
            {
                printf("\n\nWind Speed has been added\n\n");
                strncpy(params->paramArray[length], "wind_speed_10m", 60);
                strcpy(win, "added");
                length++;
            }
            else
            {
                remove_parameter(win, params, "wind_speed_10m", &length);
            }
            break;
        case 4:
            // Similar changes for other cases
            if (strcmp(pre, "added") != 0)
            {
                printf("\n\nPrecipitation Probability has been added\n\n");
                strncpy(params->paramArray[length], "precipitation_probability", 60);
                strcpy(pre, "added");
                length++;
            }
            else
            {
                remove_parameter(pre, params, "precipitation_probability", &length);
            }
            break;
        case 5:
            // Similar changes for other cases
            if (strcmp(clo, "added") != 0)
            {
                printf("\n\nCloud Cover has been added\n\n");
                strncpy(params->paramArray[length], "cloud_cover", 60);
                strcpy(clo, "added");
                length++;
            }
            else
            {
                remove_parameter(clo, params, "cloud_cover", &length);
            }
            break;
        default:
            printf("Invalid, Try again.\n");
        }

    } while (length < 3); // Use the predefined maximum number of parameters
    setBufferedInput(1);
}