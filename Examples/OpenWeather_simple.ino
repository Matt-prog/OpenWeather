/* This example shows you, how to get weather data from openweathermap.org and prints it
 *  to the Serial monitor.
 */
 
#include <ESP8266WiFi.h>
#include <OpenWeather.h>

#define SSID "YOUR-SSID"      //Insert your WiFi SSID
#define PASS  "YOUR-PASSWORD" //Insert your WiFi password

//To get API key visit this URL: https://openweathermap.org/appid
#define OW_API_KEY "YOUR-API-KEY" //Insert your API key

char city_name[] = "CITY"; //Name of city where you want to know the weather (including special characters: ľ,š,č,ť,ž,ý,á,í,é,ô...)
char country[] = "COUNTRY-CODE";  //2-letter country code (ISO3166)("usa","gb",...) (OPTIONAL)
char language[] = "LANGUAGE"; //Language of response(only weather_description will be translated) (OPTIONAL)
/*Supported languages:
 * Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el, English - en,
 * Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl, Croatian - hr, Hungarian - hu,
 * Italian - it, Japanese - ja, Korean - kr, Latvian - la, Lithuanian - lt, Macedonian - mk,
 * Dutch - nl, Polish - pl, Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
 * Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi, Chinese Simplified - zh_cn,
 * Chinese Traditional - zh_tw
 */
byte units = metric;    //Units of response

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  Serial.println();
  Serial.println(F("Weather data:"));
  OpenWeather.key(OW_API_KEY); //Inserting API key that allows us to use openweathermap.org API
  Weather w = OpenWeather.getWeatherByCityName(city_name,country,units,language); //Getting weather data from openweathermap.org
  //Execution time around 100ms
  //You can also use those commands to get weather:
  //Weather w = getWeatherByID(ID,units,language);
  //Weather w = getWeatherByZIP(ZIP_code,country,units,language);
  //Weather w = getWeatherByCoords(latitude,longitude,units,language);

  //status is http code of response
  if(w.status == 200) printWeather(&w);                           //Data received, prints all data to Serial monitor
  else if(w.status == 401) Serial.println(F("BAD API KEY!"));     //Wrong API key
  else if(w.status == 404) Serial.println(F("CITY NOT FOUND!"));  //City was not found
  else if(w.status == 429) Serial.println(F("REACHED 60 CALLS PER MINUTE LIMIT!")); //You have reached limit of your Free account (60 calls per minute)
}

void loop() {
  //Nothing to do here.

}


//Prints all data from weather structure
void printWeather(Weather *w){
  Serial.print(F("Longitude: "));
  Serial.print(w->longitude);
  Serial.println(F("\xC2\xB0")); //degrees = °
  
  Serial.print(F("Latitude: "));
  Serial.print(w->latitude);
  Serial.println(F("\xC2\xB0")); //degrees = °
  
  Serial.print(F("Weather ID: ")); Serial.println(w->weather_id);
  
  Serial.print(F("Weather main: ")); Serial.println(w->weather_main);
  
  Serial.print(F("Weather description: ")); Serial.println(w->weather_description);
  
  Serial.print(F("Weather icon: ")); Serial.println(w->weather_icon);
  
  Serial.print(F("Temperature: "));
  Serial.print(w->temp);
  Serial.print(F("\xC2\xB0")); //degrees = °
  if(w->units == default_) Serial.println('K');
  else if(w->units == metric) Serial.println('C');
  else if(w->units == imperial) Serial.println('F');
  
  Serial.print("Temperature max: ");
  Serial.print(w->temp_max);
  Serial.print(F("\xC2\xB0")); //degrees = °
  if(w->units == default_) Serial.println('K');
  else if(w->units == metric) Serial.println('C');
  else if(w->units == imperial) Serial.println('F');
  
  Serial.print(F("Temperature min: "));
  Serial.print(w->temp_min);
  Serial.print(F("\xC2\xB0")); //degrees = °
  if(w->units == default_) Serial.println('K');
  else if(w->units == metric) Serial.println('C');
  else if(w->units == imperial) Serial.println('F');
  
  Serial.print(F("Pressure: "));
  Serial.print(w->pressure);
  Serial.println(F("hPa"));
  
  Serial.print(F("Humidity: "));
  Serial.print(w->humidity);
  Serial.println('%');
  
  Serial.print(F("Visibility: "));
  Serial.print(w->visibility);
  Serial.println("m");
  
  Serial.print(F("Wind speed: "));
  Serial.print(w->wind_speed);
  if(w->units == metric || w->units == default_) Serial.println(F("m/s"));
  else if(w->units == imperial) Serial.println(F("mi/s"));
  
  Serial.print(F("Wind direction: "));
  Serial.print(windDirection(w->wind_direction));
  Serial.print('(');
  Serial.print(w->wind_direction);
  Serial.println(F("\xC2\xB0)")); //degrees = °
  
  Serial.print(F("Clouds: "));
  Serial.print(w->clouds);
  Serial.println('%');
  
  Serial.print(F("Rain 1h: "));
  Serial.print(w->rain_1h);
  Serial.println("mm");
  
  Serial.print(F("Rain 3h: "));
  Serial.print(w->rain_3h);
  Serial.println("mm");
  
  Serial.print(F("Snow 1h: "));
  Serial.print(w->snow_1h);
  Serial.println("mm");
  
  Serial.print(F("Snow 3h: "));
  Serial.print(w->snow_3h);
  Serial.println("mm");
  
  Serial.print(F("Sunrise: "));
  Serial.print((w->sunrise/3600)%24); //hours
  Serial.print(':');
  Serial.print((w->sunrise/60)%60);   //minutes
  Serial.print(':');
  Serial.println(w->sunrise%60);      //seconds
  
  Serial.print(F("Sunset: "));
  Serial.print((w->sunset/3600)%24); //hours
  Serial.print(':');
  Serial.print((w->sunset/60)%60);   //minutes
  Serial.print(':');
  Serial.println(w->sunset%60);      //seconds
  
  Serial.print(F("Country: ")); Serial.println(w->country);
  
  Serial.print(F("Timezone: GMT"));
  if(w->timezone > 0) Serial.print('+');
  Serial.println(w->timezone/3600);
  
  Serial.print(F("City name: ")); Serial.println(w->city_name);
  
  Serial.print("City ID: "); Serial.println(w->city_id);

  Serial.print(F("Data receiving time: "));
  Serial.print((w->data_time/3600)%24); //hours
  Serial.print(':');
  Serial.print((w->data_time/60)%60);   //minutes
  Serial.print(':');
  Serial.println(w->data_time%60);      //seconds
  
  Serial.print(F("Status: ")); Serial.println(w->status);
}


//Converts wind direction in degrees to cardinal direction
String windDirection(int direction){
  direction %= 360;
  if(direction >= 349 && direction < 11) return F("N");
  else if(direction < 34) return F("NNE");
  else if(direction < 56) return F("NE");
  else if(direction < 79) return F("ENE");
  else if(direction < 101) return F("E");
  else if(direction < 124) return F("ESE");
  else if(direction < 146) return F("SE");
  else if(direction < 169) return F("SSE");
  else if(direction < 191) return F("S");
  else if(direction < 214) return F("SSW");
  else if(direction < 236) return F("SW");
  else if(direction < 259) return F("WSW");
  else if(direction < 281) return F("W");
  else if(direction < 304) return F("WNW");
  else if(direction < 326) return F("NW");
  return F("NNW");
}
