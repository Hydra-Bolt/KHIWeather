#ifndef PROCESSDATA_H
#define PROCESSDATA_H

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processdata.h"
#include "emailanomalies.h"
#include "apiparams.h"

void process_json(char *chunk, FILE *rawData, FILE *procFile, struct APIParams *params);

#endif /*PROCESSDATA_H*/