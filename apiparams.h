#ifndef APIPARAMS_H
#define APIPARAMS_H

struct APIParams
{
    float longitude;
    float latitude;
    int forecastDays;
    char *paramArray[3][60];
};

void input_api_params(struct APIParams *params);
void remove_parameter(char *status, struct APIParams *params, const char *paramName, int *length);

#endif  // APIPARAMS_H
