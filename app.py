import matplotlib.pyplot as plt
import numpy as np

# Provided weather data
weather_data = {
    "latitude": 2.125,
    "longitude": 12.125,
    "generationtime_ms": 0.054955482482910156,
    "utc_offset_seconds": 0,
    "timezone": "GMT",
    "timezone_abbreviation": "GMT",
    "elevation": 605.0,
    "hourly_units": {
        "time": "iso8601",
        "temperature_2m": "°C",
        "relative_humidity_2m": "%",
        "wind_speed_10m": "km/h"
    },
    "hourly": {
        "time": [...],
        "temperature_2m": [...],
        "relative_humidity_2m": [...],
        "wind_speed_10m": [...]
    }
}

# Extract data
time = weather_data["hourly"]["time"]
temperature = weather_data["hourly"]["temperature_2m"]
humidity = weather_data["hourly"]["relative_humidity_2m"]
wind_speed = weather_data["hourly"]["wind_speed_10m"]

# Daily Averages: Calculate the average temperature for each day
daily_average_temperature = [np.mean(temperature[i:i+24]) for i in range(0, len(temperature), 24)]

# Temperature Range: Identify the highest and lowest temperatures during the forecast period
max_temperature = max(temperature)
min_temperature = min(temperature)

# Trend Analysis: Check for temperature trends over the forecast days
temperature_trend = np.polyfit(range(len(temperature)), temperature, 1)

# Relative Humidity Trends: Look for patterns or trends in relative humidity
humidity_trend = np.polyfit(range(len(humidity)), humidity, 1)

# ... Perform other analysis tasks ...

# Plotting
plt.figure(figsize=(12, 8))

# Daily Averages
plt.subplot(2, 2, 1)
plt.plot(time[::24], daily_average_temperature, marker='o')
plt.title('Daily Average Temperature')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')

# Temperature Range
plt.subplot(2, 2, 2)
plt.plot(time, temperature, label='Temperature')
plt.axhline(y=max_temperature, color='r', linestyle='--', label='Max Temperature')
plt.axhline(y=min_temperature, color='b', linestyle='--', label='Min Temperature')
plt.title('Temperature Range')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')
plt.legend()

# Trend Analysis
plt.subplot(2, 2, 3)
plt.plot(time, temperature, label='Temperature')
plt.plot(time, np.polyval(temperature_trend, range(len(temperature))), linestyle='--', color='r', label='Temperature Trend')
plt.title('Temperature Trend Analysis')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')
plt.legend()

# Relative Humidity Trends
plt.subplot(2, 2, 4)
plt.plot(time, humidity, label='Relative Humidity')
plt.plot(time, np.polyval(humidity_trend, range(len(humidity))), linestyle='--', color='r', label='Humidity Trend')
plt.title('Relative Humidity Trends')
plt.xlabel('Time')
plt.ylabel('Relative Humidity (%)')
plt.legend()

plt.tight_layout()
plt.show()
import matplotlib.pyplot as plt
import numpy as np

# Provided weather data
weather_data = {
    "latitude": 2.125,
    "longitude": 12.125,
    "generationtime_ms": 0.054955482482910156,
    "utc_offset_seconds": 0,
    "timezone": "GMT",
    "timezone_abbreviation": "GMT",
    "elevation": 605.0,
    "hourly_units": {
        "time": "iso8601",
        "temperature_2m": "°C",
        "relative_humidity_2m": "%",
        "wind_speed_10m": "km/h"
    },
    "hourly": {
        "time": [...],
        "temperature_2m": [...],
        "relative_humidity_2m": [...],
        "wind_speed_10m": [...]
    }
}

# Extract data
time = weather_data["hourly"]["time"]
temperature = weather_data["hourly"]["temperature_2m"]
humidity = weather_data["hourly"]["relative_humidity_2m"]
wind_speed = weather_data["hourly"]["wind_speed_10m"]

# Daily Averages: Calculate the average temperature for each day
daily_average_temperature = [np.mean(temperature[i:i+24]) for i in range(0, len(temperature), 24)]

# Temperature Range: Identify the highest and lowest temperatures during the forecast period
max_temperature = max(temperature)
min_temperature = min(temperature)

# Trend Analysis: Check for temperature trends over the forecast days
temperature_trend = np.polyfit(range(len(temperature)), temperature, 1)

# Relative Humidity Trends: Look for patterns or trends in relative humidity
humidity_trend = np.polyfit(range(len(humidity)), humidity, 1)

# ... Perform other analysis tasks ...

# Plotting
plt.figure(figsize=(12, 8))

# Daily Averages
plt.subplot(2, 2, 1)
plt.plot(time[::24], daily_average_temperature, marker='o')
plt.title('Daily Average Temperature')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')

# Temperature Range
plt.subplot(2, 2, 2)
plt.plot(time, temperature, label='Temperature')
plt.axhline(y=max_temperature, color='r', linestyle='--', label='Max Temperature')
plt.axhline(y=min_temperature, color='b', linestyle='--', label='Min Temperature')
plt.title('Temperature Range')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')
plt.legend()

# Trend Analysis
plt.subplot(2, 2, 3)
plt.plot(time, temperature, label='Temperature')
plt.plot(time, np.polyval(temperature_trend, range(len(temperature))), linestyle='--', color='r', label='Temperature Trend')
plt.title('Temperature Trend Analysis')
plt.xlabel('Time')
plt.ylabel('Temperature (°C)')
plt.legend()

# Relative Humidity Trends
plt.subplot(2, 2, 4)
plt.plot(time, humidity, label='Relative Humidity')
plt.plot(time, np.polyval(humidity_trend, range(len(humidity))), linestyle='--', color='r', label='Humidity Trend')
plt.title('Relative Humidity Trends')
plt.xlabel('Time')
plt.ylabel('Relative Humidity (%)')
plt.legend()

plt.tight_layout()
plt.show()
