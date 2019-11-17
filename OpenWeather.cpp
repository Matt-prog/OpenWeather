#include "OpenWeather.h"

#ifdef ESP8266 //code only for ESP8266

/*OpenWeather::OpenWeather(String key_){
  API_key = key_;
}*/

void OPEN_WEATHER::key(String key_){
  API_key = key_;
}

byte OPEN_WEATHER::getWeather(Weather *ret,String dest, byte units, char* language){
  HTTPClient http;
  
  String openweatherURL = "http://api.openweathermap.org/data/2.5/weather?";
  openweatherURL += dest;
  if(units == metric) openweatherURL += "&units=metric";
  else if(units == imperial) openweatherURL += "&units=imperial";
  if(language[0] != 0) openweatherURL += "&lang=" + String(language);
  openweatherURL += "&appid=";
  openweatherURL += API_key;
  
  http.begin(openweatherURL);
  
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    //Serial.println(payload);
    JsonSerialize(payload,ret);
    ret->sunrise += ret->timezone;
    //ret->sunrise %= 86400L;
    ret->sunset += ret->timezone;
    //ret->sunset %= 86400L;
    ret->data_time += ret->timezone;
    ret->status = httpCode;
    ret->units = units;
  }
  http.end();   //Close connection
  return ret->status;
}

Weather OPEN_WEATHER::getWeatherByCityName(String dest, String country_code, byte units, char* language){
  Weather ret;
  dest = "&q="+dest;
  if(country_code[0] != 0) dest += "," + String(country_code);
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  getWeather(&ret,dest,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByCityName(String dest, byte units, char* language){
  Weather ret;
  dest = "&q="+dest;
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  getWeather(&ret,dest,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByID(unsigned long ID, byte units, char* language){
  Weather ret;
  getWeather(&ret,"&id="+String(ID),units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByZIP(unsigned long ZIP, String country_code, byte units, char* language){
  String destination = "&zip="+String(ZIP);
  if(country_code[0] != 0) destination += "," + String(country_code);
  Weather ret;
  getWeather(&ret,destination,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByZIP(unsigned long ZIP, byte units, char* language){
  String destination = "&zip="+String(ZIP);
  Weather ret;
  getWeather(&ret,destination,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByCoords(float latitude, float longitude, byte units, char* language){
  Weather ret;
  getWeather(&ret,"&lat="+String(latitude) + "&lon=" + String(longitude),units,language);
  return ret;
}

void OPEN_WEATHER::JsonSerialize(String json, Weather *I){
  int json_length = json.length();
  byte object = 0;
  bool key_started = false;
  bool key_end = false;
  bool value_started = false;
  bool com_en = false;
  String key = "";
  String value = "";
  for(int i = 0; i < json_length; i++){
    if(json.charAt(i) == '"' || (key_started && key_end && !value_started && json.charAt(i) == ':' && json.charAt(i+1) >= '0' && json.charAt(i+1) <= '9') || (key_started && key_end && value_started && !com_en && (json.charAt(i) == ',' || json.charAt(i) == '}'))){
      if(value_started){ //end of value found
        key_started = false;
        key_end = false;
        value_started = false;
          if(key == "lon" && object == 1) I->longitude = value.toFloat();
          else if(key == "lat" && object == 1) I->latitude = value.toFloat();
          else if(key == "id" && object == 2) I->weather_id = value.toInt();
          else if(key == "main" && object == 2) strcpy(I->weather_main, value.c_str());
          else if(key == "description" && object == 2) strcpy(I->weather_description, value.c_str());
          else if(key == "icon" && object == 2) strcpy(I->weather_icon, value.c_str());
          else if(key == "temp" && object == 4) I->temp = value.toFloat();
          else if(key == "pressure" && object == 4) I->pressure = value.toFloat();
          else if(key == "humidity" && object == 4) I->humidity = value.toFloat();
          else if(key == "temp_min" && object == 4) I->temp_min = value.toFloat();
          else if(key == "temp_max" && object == 4) I->temp_max = value.toFloat();
          else if(key == "visibility" && object == 0) I->visibility = value.toInt();
          else if(key == "speed" && object == 5) I->wind_speed = value.toFloat();
          else if(key == "deg" && object == 5) I->wind_direction = value.toInt();
          else if(key == "all" && object == 6) I->clouds = value.toInt();
          else if(key == "1h" && object == 7) I->rain_1h = value.toFloat();
          else if(key == "3h" && object == 7) I->rain_3h = value.toFloat();
          else if(key == "1h" && object == 8) I->snow_1h = value.toFloat();
          else if(key == "3h" && object == 8) I->snow_3h = value.toFloat();
          else if(key == "country" && object == 9) strcpy(I->country, value.c_str());
          else if(key == "sunrise" && object == 9) I->sunrise = atol(value.c_str());
          else if(key == "sunset" && object == 9) I->sunset = atol(value.c_str());
          else if(key == "timezone" && object == 0) I->timezone = value.toInt();
          else if(key == "id" && object == 0) I->city_id = value.toInt();
          else if(key == "name" && object == 0) strcpy(I->city_name, value.c_str());
          else if(key == "dt" && object == 0) I->data_time = atol(value.c_str());

        if(json.charAt(i) == '}'){
          if(object == 0){
            break; //end of json
          }
          object = 0; //end of object
        }
      }
      else if(key_end){
        value_started = true;
        value = "";
        com_en = json.charAt(i) == '"';
      }
      else if(key_started) key_end = true;
      else{
        key_started = true;
        key = "";
      }
    }
    else if(json.charAt(i) == '{' && key_end && !value_started){ //new object found
      if(key == "coord") object = 1;
      else if(key == "weather") object = 2;
      else if(key == "base") object = 3;
      else if(key == "main") object = 4;
      else if(key == "wind") object = 5;
      else if(key == "clouds") object = 6;
      else if(key == "rain") object = 7;
      else if(key == "snow") object = 8;
      else if(key == "sys") object = 9;
      else object = 10;
      key_end = false;
      key_started = false;
    }
    else if(json.charAt(i) == '}'){
      if(object == 0){
        break; //end of json
      }
      object = 0; //end of object
    }
    else if(key_started && !key_end){
      key += json.charAt(i);
    }
    else if(key_started && key_end && value_started){
      value += json.charAt(i);
    }
  }
}

OPEN_WEATHER OpenWeather;

#endif
