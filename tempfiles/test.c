#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <ncurses.h>

// Function to handle the curl response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *data = (char *)userp;

    strcat(data, (char *)contents);

    return realsize;
}

// Function to fetch current weather using Open-Meteo API
void get_current_weather(char *latitude, char *longitude) {
    CURL *curl;
    CURLcode res;

    // API endpoint for current weather
    char api_url[256];
    snprintf(api_url, sizeof(api_url), "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&current=temperature_2m,weather_code,relative_humidity_2m,wind_speed_10m,precipitation", latitude, longitude);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the API URL
        curl_easy_setopt(curl, CURLOPT_URL, api_url);

        // Create a buffer to store the response
        char response_buffer[4096] = "";
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            // Parse the JSON response using cJSON
            cJSON *root = cJSON_Parse(response_buffer);
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
            if (weather_code_value == 0) {
                printw("Clear sky");
            } else if (weather_code_value >= 1 && weather_code_value <= 3) {
                printw("Partly Cloudy");
            } else if (weather_code_value >= 45 && weather_code_value <= 48) {
                printw("Fog");
            } else if (weather_code_value >= 51 && weather_code_value <= 55) {
                printw("Drizzle");
            } else if (weather_code_value >= 61 && weather_code_value <= 65) {
                printw("Rain");
            } else if (weather_code_value >= 71 && weather_code_value <= 75) {
                printw("Snowfall");
            } else if (weather_code_value == 77) {
                printw("Snow grains");
            } else if ((weather_code_value >= 80 && weather_code_value <= 82) || (weather_code_value >= 85 && weather_code_value <= 86)) {
                printw("Rain Showers");
            } else if (weather_code_value == 95 || weather_code_value == 96 || weather_code_value == 99) {
                printw("Thunderstorm");
            } else {
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

int main() {
    // Replace with the desired latitude and longitude
    char latitude[] = "24.00";
    char longitude[] = "66.70";

    get_current_weather(latitude, longitude);

    return 0;
}
