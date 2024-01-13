#ifndef APIPARAMS_H
#define APIPARAMS_H

struct APIParams
{
    float longitude;
    float latitude;
    int forecastDays;
    char country[60];
    char paramArray[3][60];
};
char *create_api_url(struct APIParams *params);
void input_api_params(struct APIParams *params, char* argv[]);

#endif // APIPARAMS_H
