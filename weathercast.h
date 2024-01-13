#ifndef WEATHERCAST_H
#define WEATHERCAST_H

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processdata.h"
#include "emailanomalies.h"
#include "apiparams.h"

// Structure to hold memory for HTTP response
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Function to handle writing HTTP response to memory
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

// Function to forecast weather
void forecast_weather(struct APIParams *params, char *argv[]);

// Function to get current weather
void get_current_weather(float latitude, float longitude);

#endif // WEATHERCAST_H
