#if defined(ESP8266) || defined(ESP32)  //code only for ESP8266 or ESP32

#ifndef OpenWeather_h
#define OpenWeather_h

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif


/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif


/** None status */
#define OW_NONE 0
/** Open weather OK status */
#define OW_OK 200
/** Bad API key status */
#define OW_BAD_API_KEY 401
/** City not found status */
#define OW_CITY_NOT_FOUND 404
/** This status means, that you have reached limit of your Free account (60 calls per minute) */
#define OW_LIMIT_REACHED 429
/** OpenWeather server error */
#define OW_SERVER_ERROR 500

/** Units type enums */
typedef enum{
  /** Kelvin, meters per seconds */
  metricK,
  /** Celsius, meters per seconds */
  metric,
  /** Fahrenheit, miles per seconds */
  imperial
} Units;
//Change will appear only in values: temp, temp_max, temp_min, feels_like, wind_speed


/** Weather info structure */
typedef struct{
  /** Weather condition ID */
  int id = 0;
  /** Weather main text (Rain, Clear,...) */
  char main[40] = "";
  /** Weather description */
  char description[40] = "";
  /** Weather icon (to get weather icon use URL: "http://openweathermap.org/img/wn/" + weather_icon + "@2x.png") */
  char icon[10] = "";
} WeatherInfo;


class OpenWeather{
  public:

  //Constructors
  /**
   * @brief Constructor
   */
  OpenWeather();
  /**
   * @brief Constructor
   * @param key_ User open weather API key
   * @note Check this if you want to get API key: https://openweathermap.org/appid
   */
  OpenWeather(String key_);
  
  //Destructor
  /**
   * @brief Destructor
   */
  ~OpenWeather();
  
  // Copy assignment operator
  /**
   * @brief Copy assignment operator
   */
  OpenWeather &operator=(const OpenWeather &w){
    if(weatherInfo && weatherInfoCount >= 1) delete[] weatherInfo ;
    memcpy(this, &w, sizeof(OpenWeather));
    weatherInfo = new WeatherInfo[weatherInfoCount];
    //copy dynamic array
    if(weatherInfo){
      for(int i = 0; i < weatherInfoCount; i++){
        weatherInfo[i] = w.weatherInfo[i];
      }
    }
    else status = -8;//HTTPC_ERROR_TOO_LESS_RAM;
    return *this;
  }

  /**
   * @brief Set new API key
   * @param key_ User open weather API key
   * @note Check this if you want to get API key: https://openweathermap.org/appid
   */
  void key(String key_);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param dest Name of the city you want to get actual weather for
   * @param state_code 2-characters country code (use ISO 3166 state codes)
   * @param country_code 2-characters country code (use ISO 3166 country codes)
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByCityName(String dest, String state_code, String country_code, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param dest Name of the city you want to get actual weather for
   * @param country_code 2-characters country code (use ISO 3166 country codes)
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByCityName(String dest, String country_code, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param dest Name of the city you want to get actual weather for
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByCityName(String dest, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param ID ID of the city you want to get actual weather for (list of IDs can be downloaded here: http://bulk.openweathermap.org/sample/)
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByID(unsigned long ID, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param ZIP ZIP code of the city (or city block) you want to get actual weather for
   * @param country_code 2-characters country code (use ISO 3166 country codes)
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByZIP(String ZIP, String country_code, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param ZIP ZIP code of the city (or city block) you want to get actual weather for
   * @param state_code 2-characters country code (use ISO 3166 state codes)
   * @param country_code 2-characters country code (use ISO 3166 country codes)
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByZIP(String ZIP, String state_code, String country_code, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/.
   * @param ZIP ZIP code of the city (or city block) you want to get actual weather for
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByZIP(String ZIP, Units units, String language);

  /**
   * @brief Gets actual weather from https://openweathermap.org/ by coordinates.
   * @param latitude Geographical latitude coordinate of the city (or city block) you want to get actual weather for
   * @param longitude Geographical longitude coordinate
   * @param units Units of data, you want to get (use Units enum)
   * @param language Language of text data (check this, to learn about language parameter format: https://openweathermap.org/current#multi)
   * @return Returns true when http code of http request was OK(200)
   */
  bool getWeatherByCoords(float latitude, float longitude, Units units, String language);

  /**
   * @brief Clears all weather data.
   */
  void clear();

  /**
   * @brief Sets maximum weatherInfo array size.
   * @param maxWeatherInfoCount Value between 1-10;
   */
  void setWeatherArraySize(int size){maxWeatherInfoCount = constrain(size,1,10);}

  /**
   * @brief Gets maximum weatherInfo array size.
   * @return Returns maximum weatherInfo array size.
   */
  int getWeatherArraySize(){return maxWeatherInfoCount;}

//Weather data
  /** Geographical longitude coordinate of the city, where we get weather data. */
  float longitude = 0.00;
  /** Geographical latitude coordinate of the city, where we get weather data. */
  float latitude = 0.00;
  /** Actual weather informations (array) */
  WeatherInfo *weatherInfo = NULL;
  /** Size of array "weather" */
  int weatherInfoCount = 0;
  /** Actual temperature (Units: MetricK: Kelvin, Metric: Celsius, Imperial: Fahrenheit) */
  float temp = 0.00;
  /** Actual maximum temperature (Units: MetricK: Kelvin, Metric: Celsius, Imperial: Fahrenheit) */
  float temp_max = 0.00;
  /** Actual minimum temperature (Units: MetricK: Kelvin, Metric: Celsius, Imperial: Fahrenheit) */
  float temp_min = 0.00;
  /** Actual feels like temperature */
  float feels_like = 0.00;
  /** Actual pressure in hPa */
  float pressure = 0.00;
  /** Actual humidity in % */
  float humidity = 0.00;
  /** Visibility in meters */
  int visibility = 0;
  /** Actual wind speed (Units: MetricK: meter/sec, Metric: meter/sec, Imperial: miles/hour) */
  float wind_speed = 0.00;
  /** Wind direction in degrees */
  short wind_direction = 0;
  /** Cloudiness in % */
  byte clouds = 0;
  /** Rain volume for the last 1 hour in mm */
  float rain_1h = 0.00;
  /** Rain volume for the last 3 hours in mm */
  float rain_3h = 0.00;
  /** Snow volume for the last 1 hour in mm */
  float snow_1h = 0.00;
  /** Snow volume for the last 3 hours in mm */
  float snow_3h = 0.00;
  /** Sunrise in seconds UNIX (including timezone) */
  unsigned long sunrise = 0;
  /** Sunset in seconds UNIX (including timezone) */
  unsigned long sunset = 0;
  /** Country code (GB, USA,...) */
  char country[6] = {0};
  /** Timezone offset in seconds */
  long timezone = 0;
  /** City name (London, ...) */
  char city_name[60] = {0};
  /** City ID */
  unsigned long city_id = 0;
  /** Units of response */
  Units units = metric;
  /** Data receiving time in seconds UNIX (including timezone) */
  unsigned long data_time = 0;
  /** Status of received message (> 0 HTTP codes, -1 to -11 are HTTP client errors defined in ESP8266HTTPClient.h) */
  int16_t status = 0;


  private:
  int16_t getWeather(String dest, Units units_, String language_);
  void JsonSerialize(String& json);
  String API_key;
  static HTTPClient http;
  static WiFiClient httpClient;
  int maxWeatherInfoCount = 5; //default is 5
};


/*! CPP guard */
#ifdef __cplusplus
}
#endif

#endif

#endif