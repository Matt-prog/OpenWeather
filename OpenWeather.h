#ifdef ESP8266 //code only for ESP8266

#ifndef OpenWeather_h
#define OpenWeather_h

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif


typedef struct{
  float longitude = 0.00;
  float latitude = 0.00;
  int weather_id = 0;                 //Weather condition ID
  char weather_main[40] = "";         //Weather main text (Rain, Clear,...)
  char weather_description[40] = "";  //Weather description
  char weather_icon[10] = "";         //Weather icon (to get weather icon use URL: "http://openweathermap.org/img/wn/" + weather_icon + "@2x.png")
  float temp = 0.00;                  //Actual temperature (Units: Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit)
  float temp_max = 0.00;              //Actual maximum temperature (Units: Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit)
  float temp_min = 0.00;              //Actual minimum temperature (Units: Default: Kelvin, Metric: Celsius, Imperial: Fahrenheit)
  float pressure = 0.00;              //Actual pressure in hPa
  float humidity = 0.00;              //Actual humidity in %
  int visibility = 0;                 //Visibility in meters
  float wind_speed = 0.00;            //Actual wind speed (Units: Default: meter/sec, Metric: meter/sec, Imperial: miles/hour)
  short wind_direction = 0;           //Wind direction in degrees
  byte clouds = 0;                    //Cloudiness in %
  float rain_1h = 0.00;               //Rain volume for the last 1 hour in mm
  float rain_3h = 0.00;               //Rain volume for the last 3 hours in mm
  float snow_1h = 0.00;               //Snow volume for the last 1 hour in mm
  float snow_3h = 0.00;               //Snow volume for the last 3 hours in mm
  unsigned long sunrise = 0;          //Sunrise in seconds UNIX (including timezone)
  unsigned long sunset = 0;           //Sunset in seconds UNIX (including timezone)
  char country[6] = "";               //Country code (GB, USA,...)
  long timezone = 0;                  //Timezone offset in seconds
  char city_name[60] = "";            //City name (London, ...)
  unsigned long city_id = 0;          //City ID
  byte units = 0;                     //Units of response
  unsigned long data_time = 0;        //Data receiving time in seconds UNIX (including timezone)
  int status = 0;                     //Status of received message (> 0 HTTP codes, -1 to -11 are HTTP client errors defined in ESP8266HTTPClient.h,
                                      // 404 is CITY_NOT_FOUND
                                      // 401 invalid API key
} Weather;

//Units enums
enum{
  default_, //Kelvin, meters per seconds
  metric,   //Celsius, meters per seconds
  imperial  //Fahrenheit, miles per seconds
};
//Change will appear only in values: temp, temp_max, temp_min, wind_speed

class OPEN_WEATHER{
  public:
  //OpenWeather(String key_);
  void key(String key_);
  byte getWeather(Weather *ret,String dest, byte units = default_, char* language = "");
  Weather getWeatherByCityName(String dest, String country_code = "", byte units = default_, char* language = "");
  Weather getWeatherByCityName(String dest, byte units = default_, char* language = "");
  Weather getWeatherByID(unsigned long ID, byte units = default_, char* language = "");
  Weather getWeatherByZIP(unsigned long ZIP, String country_code = "", byte units = default_, char* language = "");
  Weather getWeatherByZIP(unsigned long ZIP, byte units = default_, char* language = "");
  Weather getWeatherByCoords(float latitude, float longitude, byte units = default_, char* language = "");

  private:
  void JsonSerialize(String json, Weather *I);
  String API_key;
};

extern OPEN_WEATHER OpenWeather;


/*! CPP guard */
#ifdef __cplusplus
}
#endif

#endif

#endif
