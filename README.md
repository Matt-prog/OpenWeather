# OpenWeather
Simple and fast ESP8266 and ESP32 (arduino) library which allows you to get current weather data by city name, ZIP code, coordinates or city ID using openweathermap API.

## Install
Clone or download and unzip the repository to Arduino libraries directory. It can be found in `~/Arduino/libraries` where Arduino directory is directory, where Arduino IDE was installed.

## Usage
To use this library, you have to obtain API key from https://openweathermap.org/.
It can be done by following those steps:
1. Sign in or create an account at https://home.openweathermap.org/users/sign_in.
2. Now you see page with some tabs (New Products, Services, API keys, ...). Click to "API keys" tab.
3. There you see yours API keys. Copy the API key you want to use and add it to your code. Call `OpenWeather::key("this_is_your_api_key")` method to set an API key, that will be used by OpenWeather class instance.
4. Feel free to this library :smiley:

## Note
This verison of library allows you to get only a current weather data (even you have paid for others APIs).

## Learn more
Learn more about the current weather API at: https://openweathermap.org/current
