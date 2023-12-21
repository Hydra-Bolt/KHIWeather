// #include <stdio.h>
// #include <string.h>
// #include <conio.h>1

// // #define MAX_PARAMS 5 // Maximum number of parameters

// struct APIParams
// {
//     char* paramArray[3];
// };

// void remove_parameter(char* status, struct APIParams* params, const char* paramName, int* length)
// {
//     // Iterate through the parameter array to find and remove the specified parameter
//     for (int i = 0; i < *length; i++)
//     {
//         if (strcmp(params->paramArray[i], paramName) == 0)
//         {
//             // Shift elements to fill the gap
//             for (int j = i; j < *length - 1; j++)
//             {
//                 params->paramArray[j] = params->paramArray[j + 1];
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

// void input_api_params(struct APIParams* params)
// {
//     char temp[6] = "off", hum[6] = "off", win[6] = "off", pre[6] = "off", clo[6] = "off";
//     int option;
//     int length = 0; // Counter for the number of added parameters

//     do
//     {
//         printf("Tell us what to show and what not to show.\n");
//         printf("Press the corresponding key to add it to analysis.\n");
//         printf("Press the corresponding key again to remove it:\n");
//         printf("1. Temperature %s\n2. Humidity %s\n3. Wind Speed %s\n4. Precipitation Chance %s\n5. Cloud Cover %s\n", temp, hum, win, pre, clo);

//         option = _getch() - '0'; // Use getchar instead of _getch

//         switch (option)
//         {
//         case 1:
//             if (strcmp(temp, "added") != 0)
//             {
//                 printf("Temperature has been added\n");
//                 params->paramArray[length] = "temperature_2m";
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
//                 printf("Humidity has been added\n");
//                 params->paramArray[length] = "humidity";
//                 strcpy(hum, "added");
//                 length++;
//             }
//             else
//             {
//                 remove_parameter(hum, params, "humidity", &length);
//             }
//             break;
//         case 3:
//             // Similar changes for other cases
//             if (strcmp(win, "added") != 0)
//             {
//                 printf("Wind Speed has been added\n");
//                 params->paramArray[length] = "wind_speed_10m";
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
//                 printf("Precipitation Probability has been added\n");
//                 params->paramArray[length] = "precipitation_probability";
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
//                 printf("Cloud Cover has been added\n");
//                 params->paramArray[length] = "cloud_cover";
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
// }

// int main()
// {
//     struct APIParams params;

//     // Call your function to input API parameters
//     input_api_params(&params);
//     for (int i = 0 ; i<3; i++)
//     {
//         printf("%s\n", (params).paramArray[i]);
//     }
//     // Other code...

//     return 0;
// }

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// Function to enable or disable terminal input buffering
void setBufferedInput(int enable) {
    static struct termios oldt, newt;

    if (!enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

int main() {
    int c;

    // Disable buffered input
    setBufferedInput(0);

    printf("Press any key (ESC to exit):\n");

    // Loop until the user presses the ESC key
    while (1) {
        c = getchar() - '0
        
        ';
        // Check for the ESC key
        if (c == 27) {
            break;
        }

        // Print the pressed key
        printf("You pressed: %d\n", c);
    }

    // Enable buffered input before exiting
    setBufferedInput(1);

    return 0;
}
