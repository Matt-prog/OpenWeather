#include "OpenWeather.h"

#ifdef ESP8266 //code only for ESP8266

/*OpenWeather::OpenWeather(String key_){
  API_key = key_;
}*/

void OPEN_WEATHER::key(String key_){
  API_key = key_;
}

int OPEN_WEATHER::getWeather(Weather *ret,String dest, byte units, char* language){
  String openweatherURL = String(F("http://api.openweathermap.org/data/2.5/weather?"));
  openweatherURL += dest;
  if(units == metric) openweatherURL += String(F("&units=metric"));
  else if(units == imperial) openweatherURL += String(F("&units=imperial"));
  if(language[0] != 0) openweatherURL += String(F("&lang=")) + String(language);
  openweatherURL += String(F("&appid="));
  openweatherURL += API_key;
  
  http.setTimeout(2000);
  //http.setReuse(false);
  http.begin(openweatherURL);
  
  ret->status = http.GET();
  if (ret->status == 200) {
    String payload = http.getString();
    //Serial.println(payload);
    JsonSerialize(payload,ret);
    ret->sunrise += ret->timezone;
    //ret->sunrise %= 86400L;
    ret->sunset += ret->timezone;
    //ret->sunset %= 86400L;
    ret->data_time += ret->timezone;
    ret->units = units;
  }
  http.end();   //Close connection
  return ret->status;
}

Weather OPEN_WEATHER::getWeatherByCityName(String dest, String country_code, byte units, char* language){
  Weather ret;
  //clearStruct(ret);
  dest = String(F("&q="))+dest;
  if(country_code[0] != 0) dest += "," + String(country_code);
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  getWeather(&ret,dest,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByCityName(String dest, byte units, char* language){
  Weather ret;
  //clearStruct(ret);
  dest = String(F("&q="))+dest;
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  getWeather(&ret,dest,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByID(unsigned long ID, byte units, char* language){
  Weather ret;
  //clearStruct(ret);
  getWeather(&ret,String(F("&id="))+String(ID),units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByZIP(unsigned long ZIP, String country_code, byte units, char* language){
  String destination = String(F("&zip="))+String(ZIP);
  if(country_code[0] != 0) destination += "," + String(country_code);
  Weather ret;
  //clearStruct(ret);
  getWeather(&ret,destination,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByZIP(unsigned long ZIP, byte units, char* language){
  String destination = String(F("&zip="))+String(ZIP);
  Weather ret;
  //clearStruct(ret);
  getWeather(&ret,destination,units,language);
  return ret;
}

Weather OPEN_WEATHER::getWeatherByCoords(float latitude, float longitude, byte units, char* language){
  Weather ret;
  //clearStruct(ret);
  getWeather(&ret,String(F("&lat="))+String(latitude) + String(F("&lon=")) + String(longitude),units,language);
  return ret;
}

void OPEN_WEATHER::JsonSerialize(String& json, Weather *I){
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
          if(key == F("lon") && object == 1) I->longitude = value.toFloat();
          else if(key == F("lat") && object == 1) I->latitude = value.toFloat();
          else if(key == F("id") && object == 2) I->weather_id = value.toInt();
          else if(key == F("main") && object == 2) strncpy(I->weather_main, value.c_str(),40);
          else if(key == F("description") && object == 2) strncpy(I->weather_description, value.c_str(),40);
          else if(key == F("icon") && object == 2) strncpy(I->weather_icon, value.c_str(),10);
          else if(key == F("temp") && object == 4) I->temp = value.toFloat();
          else if(key == F("pressure") && object == 4) I->pressure = value.toFloat();
          else if(key == F("humidity") && object == 4) I->humidity = value.toFloat();
          else if(key == F("temp_min") && object == 4) I->temp_min = value.toFloat();
          else if(key == F("temp_max") && object == 4) I->temp_max = value.toFloat();
          else if(key == F("visibility") && object == 0) I->visibility = value.toInt();
          else if(key == F("speed") && object == 5) I->wind_speed = value.toFloat();
          else if(key == F("deg") && object == 5) I->wind_direction = value.toInt();
          else if(key == F("all") && object == 6) I->clouds = value.toInt();
          else if(key == F("1h") && object == 7) I->rain_1h = value.toFloat();
          else if(key == F("3h") && object == 7) I->rain_3h = value.toFloat();
          else if(key == F("1h") && object == 8) I->snow_1h = value.toFloat();
          else if(key == F("3h") && object == 8) I->snow_3h = value.toFloat();
          else if(key == F("country") && object == 9) strncpy(I->country, value.c_str(),6);
          else if(key == F("sunrise") && object == 9) I->sunrise = atol(value.c_str());
          else if(key == F("sunset") && object == 9) I->sunset = atol(value.c_str());
          else if(key == F("timezone") && object == 0) I->timezone = value.toInt();
          else if(key == F("id") && object == 0) I->city_id = value.toInt();
          else if(key == F("name") && object == 0) strncpy(I->city_name, value.c_str(),60);
          else if(key == F("dt") && object == 0) I->data_time = atol(value.c_str());

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
      if(key == F("coord")) object = 1;
      else if(key == F("weather")) object = 2;
      else if(key == F("base")) object = 3;
      else if(key == F("main")) object = 4;
      else if(key == F("wind")) object = 5;
      else if(key == F("clouds")) object = 6;
      else if(key == F("rain")) object = 7;
      else if(key == F("snow")) object = 8;
      else if(key == F("sys")) object = 9;
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


/*void OPEN_WEATHER::clearStruct(Weather &s){
  s.longitude = 0.00;
  s.latitude = 0.00;
  s.weather_id = 0;
  s.weather_main[0] = '/0';
  s.weather_description[0] = '/0';
  s.weather_icon[0] = '/0';
  s.temp = 0.00;
  s.temp_max = 0.00;
  s.temp_min = 0.00;
  s.pressure = 0.00;
  s.humidity = 0.00;
  s.visibility = 0;
  s.wind_speed = 0.00;
  s.wind_direction = 0;
  s.clouds = 0;
  s.rain_1h = 0.00;
  s.rain_3h = 0.00;
  s.snow_1h = 0.00;
  s.snow_3h = 0.00;
  s.sunrise = 0UL;
  s.sunset = 0UL;
  s.country[0] = '/0';
  s.timezone = 0L;
  s.city_name[0] = '/0';
  s.city_id = 0UL;
  s.units = 0;
  s.data_time = 0UL;
  s.status = 0;
}*/


OPEN_WEATHER OpenWeather;

#endif
