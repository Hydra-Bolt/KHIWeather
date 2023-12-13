#include <stdio.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>


struct APIParams {
    float longitude;
    float latitude;
    int forecastDays;
    char* time[30];
    char* paramArray[3][20];

}; // Added semicolon to complete struct definition


size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // This function is called by libcurl to write the received data to a buffer
    size_t realsize = size * nmemb;
    char **json_buffer = (char **)userp;

    *json_buffer = realloc(*json_buffer, realsize + 1);
    if (*json_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    memcpy(*json_buffer, contents, realsize);
    (*json_buffer)[realsize] = 0;

    return realsize;
}

char* create_api_url(struct APIParams* params)
{
    char api_url[] = "https://api.open-meteo.com/v1/forecast";
    char api_params[256];
    char* apiReq[60];
    // Avoiding buffer overflow if the user is stupid
    snprintf(api_params, sizeof(api_params), "latitude=%.2f&longitude=%.2f&forecast_days=%d&%s=%s", params->latitude, params->longitude, params->forecastDays, params->time, apiReq);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    char full_url[512];
    snprintf(full_url, sizeof(full_url), "%s?%s", api_url, api_params);
    return strdup(full_url);
}

void process_json(char *json_buffer, FILE *outputFile)
{
    printf("%s", json_buffer);
    cJSON *root = cJSON_Parse(json_buffer);
    if (root != NULL) {
        cJSON *hourly = cJSON_GetObjectItemCaseSensitive(root, "hourly");
        if (cJSON_IsObject(hourly)) {
            cJSON *temperatureArray = cJSON_GetObjectItemCaseSensitive(hourly, "temperature_2m"), *timeArray = cJSON_GetObjectItemCaseSensitive(hourly, "time");
            if (cJSON_IsArray(temperatureArray)) {
                int arraySize = cJSON_GetArraySize(temperatureArray);
                for (int i = 0; i < arraySize; i++) {
                    cJSON *temperature = cJSON_GetArrayItem(temperatureArray, i);
                    cJSON *time = cJSON_GetArrayItem(timeArray, i);
                    if (cJSON_IsNumber(temperature)) {
                        fprintf(outputFile, "Temperature at %s: %.2f\n", time->valuestring, temperature->valuedouble);
                        printf("Temperature at %s: %.2f\n", time->valuestring, temperature->valuedouble);
                    }
                }
            } else {
                fprintf(stderr, "Temperature array not found in JSON\n");
            }
        } else {
            fprintf(stderr, "Hourly data not found in JSON\n");
        }

        cJSON_Delete(root);
    } else {
        fprintf(stderr, "Failed to parse JSON\n");
    }
}


int main(void) {
    CURL *curl;
    CURLcode res;

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    char* full_url = create_api_url();
    printf("%s", full_url);

    curl = curl_easy_init();
    if (curl) {
        char *json_buffer = NULL;

        // Set the callback function to handle the received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        // Pass the buffer to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json_buffer);

        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            process_json(json_buffer, outputFile);
        }

        // Cleanup
        curl_free(json_buffer);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
