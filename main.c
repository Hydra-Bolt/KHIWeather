#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

struct APIParams
{
    float longitude;
    float latitude;
    int forecastDays;
    char *paramArray[3][60];
};
// This just dynamically writes the json and makes memory for it
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    // This function is called by libcurl to write the received data to a buffer
    size_t realsize = size * nmemb;
    char **json_buffer = (char **)userp;

    *json_buffer = realloc(*json_buffer, realsize + 1);
    if (*json_buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    memcpy(*json_buffer, contents, realsize);
    (*json_buffer)[realsize] = 0;

    return realsize;
}

char *create_api_url(struct APIParams *params)
{
    char api_url[] = "https://api.open-meteo.com/v1/forecast";
    char api_params[256];
    z

    // Avoiding buffer overflow if the user is stupid
    snprintf(api_params, sizeof(api_params), "latitude=%.2f&longitude=%.2f&forecast_days=%d&hourly=%s,%s,%s", params->latitude, params->longitude, params->forecastDays, &params->paramArray[0], &params->paramArray[1], params->paramArray[2]);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s?%s", api_url, api_params);
    return strdup(full_url);
}

void process_json(char *json_buffer, FILE *outputFile) //, struct *APIParams)
{
    // printf("%s", json_buffer);
    cJSON *root = cJSON_Parse(json_buffer);
    if (root != NULL)
    {
        cJSON *hourly = cJSON_GetObjectItemCaseSensitive(root, "hourly");
        if (cJSON_IsObject(hourly))
        {
            cJSON *temperatureArray = cJSON_GetObjectItemCaseSensitive(hourly, "temperature_2m"), *timeArray = cJSON_GetObjectItemCaseSensitive(hourly, "time");
            if (cJSON_IsArray(temperatureArray))
            {
                int arraySize = cJSON_GetArraySize(temperatureArray);
                for (int i = 0; i < arraySize; i++)
                {
                    cJSON *temperature = cJSON_GetArrayItem(temperatureArray, i);
                    cJSON *time = cJSON_GetArrayItem(timeArray, i);
                    if (cJSON_IsNumber(temperature))
                    {
                        fprintf(outputFile, "Temperature at %s: %.2f\n", time->valuestring, temperature->valuedouble);
                        printf("Temperature at %s: %.2f\n", time->valuestring, temperature->valuedouble);
                    }
                }
            }
            else
            {
                fprintf(stderr, "Temperature array not found in JSON\n");
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

void remove_parameter(char *status, struct APIParams *params, const char *paramName, int *length)
{
    // Iterate through the parameter array to find and remove the specified parameter
    for (int i = 0; i < *length; i++)
    {
        if (strcmp(*params->paramArray[i], paramName) == 0)
        {
            // Shift elements to fill the gap
            for (int j = i; j < *length - 1; j++)
            {
                strcpy(*params->paramArray[j], *params->paramArray[j + 1]);
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
    char temp[6] = "off", hum[6] = "off", win[6] = "off", pre[6] = "off", clo[6] = "off";
    int option;
    int length = 0; // Counter for the number of added parameters

    do
    {
        // printf("\n\nTell us what to show and what not to show.\n");
        printf("\n\nPress the corresponding key to add it to analysis.\n");
        printf("Press the corresponding key again to remove it:\n");
        printf("1. Temperature %s\n2. Humidity %s\n3. Wind Speed %s\n4. Precipitation Chance %s\n5. Cloud Cover %s\n", temp, hum, win, pre, clo);

        option = _getch() - '0';

        switch (option)
        {
        case 1:
            if (strcmp(temp, "added") != 0)
            {
                printf("\n\nTemperature has been added\n\n");
                strncpy(params->paramArray[length] , "temperature_2m", 60);
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
                strncpy(params->paramArray[length] , "relative_humidity_2m", 60);
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
                strncpy(params->paramArray[length] , "wind_speed_10m", 60);
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
                strncpy(params->paramArray[length] , "precipitation_probability", 60);
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
                strncpy(params->paramArray[length] , "cloud_cover", 60);
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
}

int main(void)
{
    CURL *curl;
    CURLcode res;

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    struct APIParams *params = malloc(sizeof(struct APIParams));

    // Sannan here Implement the input taking from the user:
    // Fill up the input taking part and design the front end
    printf("Welcome to KHI Weather Reporting System\n Where we bring you the latest weather report\n");

    printf("Enter the longitude info of your location: ");
    scanf("%f", &params->longitude);

    printf("Enter the latitude info of your location: ");
    scanf("%f", &params->latitude);

    printf("How many days to forcast?: ");
    scanf("%d", &params->forecastDays);

    printf("Tell us what to show and what not to show");

    input_api_params(params);
    char *full_url = create_api_url(params);
    // printf("%s", full_url);

    curl = curl_easy_init();
    if (curl)
    {
        char *json_buffer = NULL;

        // Set the callback function to handle the received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        // Pass the buffer to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json_buffer);

        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            process_json(json_buffer, outputFile);
        }

        // Cleanup
        curl_free(json_buffer);
        curl_easy_cleanup(curl);
    }
    free(full_url);
    free(params);
    fclose(outputFile);
    curl_global_cleanup();

    return 0;
}
