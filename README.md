# KHIWeather
A small scale weather analysis and Realtime alert system.

# Running Instructions

- run `requirments.sh`, let it install all the dependencies.
- run `compile_and_run.sh` and the program will run

# Custom Report generation

This document provides information on how to use the environment variables specified in the provided `.env` file for interacting with the Weather API  such that to generate customized reports.

## City Codes

The following city codes are used to specify the location for weather information:

- 1: Karachi
- 2: London
- 3: Moscow
- 4: Ankara
- 5: Beijing
- 6: Delhi
- 7: Sydney
- 8: Washington D.C.

### Example
```bash
CITY=8
```

## Weather Type

The `WEATHER_CAST` variable is used to determine the type of weather information you want:

- 1: Current Weather
- 2: Forecasting

### Example
```bash
WEATHER_CAST=2
```

### Forecasting Parameters (if `WEATHER_CAST=2`)

The following parameters are only valid when forecasting (`WEATHER_CAST=2`):

### Number of Days (1 to 16)

Specify the number of days for the weather forecast (1 to 16):

```bash
NUM_OF_DAYS=16
```

### Measurement Parameters (1 to 5)

Specify the measurement parameters for the weather forecast (1 to 5):

- 1: Temperature
- 2: Humidity
- 3: Wind Speed
- 4: Precipitation
- 5: Cloud Cover

```bash
PARAM_1=1
PARAM_2=2
PARAM_3=3
```

## Email Address

Provide the email address where you want to receive the weather information:

```bash
EMAIL=mubashir200017@gmail.com
```

Make sure to set these environment variables in your `.env` file before running the Weather API script. Adjust the values according to your preferences and requirements.
