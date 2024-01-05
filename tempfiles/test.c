#include "apiparams.h"
#include <stdio.h>
int main() {
    struct APIParams params;

    // Initialize APIParams struct members if needed

    input_api_params(&params);

    // Now, you can use the params struct as needed

    printf("\nAPI Parameters:\n");
    printf("Longitude: %f\n", params.longitude);
    printf("Latitude: %f\n", params.latitude);
    printf("Forecast Days: %d\n", params.forecastDays);
    printf("Country: %s\n", params.country);

    printf("\nSelected Parameters for Analysis:\n");
    for (int i = 0; i < 3; ++i) {
        printf("%d. %s\n", i + 1, params.paramArray[i]);
    }

    return 0;
}
