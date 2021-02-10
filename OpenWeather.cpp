#include "OpenWeather.h"

#if defined(ESP8266) || defined(ESP32)  //code only for ESP8266 or ESP32

OpenWeather::OpenWeather(){
}

OpenWeather::OpenWeather(String key_){
  API_key = key_;
}

OpenWeather::~OpenWeather(){
  if(weatherInfo && weatherInfoCount >= 1) delete[] weatherInfo;
}

void OpenWeather::key(String key_){
  API_key = key_;
}

int16_t OpenWeather::getWeather(String dest, Units units_, String language_){
  String openweatherURL = String(F("http://api.openweathermap.org/data/2.5/weather?"));
  openweatherURL += dest;
  if(units_ == metric) openweatherURL += String(F("&units=metric"));
  else if(units_ == imperial) openweatherURL += String(F("&units=imperial"));
  if(language_[0] != 0) openweatherURL += String(F("&lang=")) + language_;
  openweatherURL += String(F("&appid="));
  openweatherURL += API_key;
  
  (OpenWeather::http).setTimeout(2000);
  //http.setReuse(false);
  (OpenWeather::http).begin((OpenWeather::httpClient), openweatherURL);
  
  status = (OpenWeather::http).GET();
  if (status == 200) {
    String payload = (OpenWeather::http).getString();
    //Serial.println(payload);
    JsonSerialize(payload);
    if(status == 200){
      sunrise += timezone;
      //ret->sunrise %= 86400L;
      sunset += timezone;
      //ret->sunset %= 86400L;
      data_time += timezone;
    }
    units = units_;
  }
  (OpenWeather::http).end();   //Close connection
  return status;
}

bool OpenWeather::getWeatherByCityName(String dest, String state_code, String country_code, Units units, String language){
  //clear();
  dest = String(F("&q="))+dest;
  if(state_code[0] != 0) dest += "," + String(state_code);
  if(country_code[0] != 0) dest += "," + String(country_code);
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  return (getWeather(dest,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByCityName(String dest, String country_code, Units units, String language){
  //clear();
  dest = String(F("&q="))+dest;
  if(country_code[0] != 0) dest += "," + String(country_code);
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  return (getWeather(dest,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByCityName(String dest, Units units, String language){
  //clear();
  dest = String(F("&q="))+dest;
  dest.replace(" ", "+"); //HTML encoding does not allow spaces
  return (getWeather(dest,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByID(unsigned long ID, Units units, String language){
  //clear();
  return (getWeather(String(F("&id="))+String(ID),units,language) == OW_OK);
}

bool OpenWeather::getWeatherByZIP(String ZIP, String country_code, Units units, String language){
  String destination = String(F("&zip="))+String(ZIP);
  destination.replace(" ", "+");
  if(country_code[0] != 0) destination += "," + String(country_code);
  //clear();
  return (getWeather(destination,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByZIP(String ZIP, String state_code, String country_code, Units units, String language){
  String destination = String(F("&zip="))+String(ZIP);
  destination.replace(" ", "+");
  if(state_code[0] != 0) destination += "," + String(state_code);
  if(country_code[0] != 0) destination += "," + String(country_code);
  //clear();
  return (getWeather(destination,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByZIP(String ZIP, Units units, String language){
  String destination = String(F("&zip="))+String(ZIP);
  destination.replace(" ", "+");
  //clear();
  return (getWeather(destination,units,language) == OW_OK);
}

bool OpenWeather::getWeatherByCoords(float latitude, float longitude, Units units, String language){
  //clear();
  return (getWeather(String(F("&lat="))+String(latitude) + String(F("&lon=")) + String(longitude),units,language) == OW_OK);
}

void OpenWeather::JsonSerialize(String& json){
  int json_length = json.length();
  byte object = 0;
  bool key_started = false;
  bool key_end = false;
  bool value_started = false;
  bool array_started = false;
  bool com_en = false;
  String key = "";
  String value = "";
  int8_t arr_index = -1;
  byte arr_size = 0;
  WeatherInfo *tmp_w = new WeatherInfo[maxWeatherInfoCount]; //temporary weather array
  if(!tmp_w){
    status = HTTPC_ERROR_TOO_LESS_RAM;
    return; // cannot allocate array
  }
  for(int i = 0; i < maxWeatherInfoCount; i++){
    tmp_w[i].id = 0;
    tmp_w[i].main[0] = 0;
    tmp_w[i].description[0] = 0;
    tmp_w[i].icon[0] = 0;
  }
  for(int i = 0; i < json_length; i++){
    //Serial.print(json.charAt(i));
    if(json.charAt(i) == '"' || (key_started && key_end && !value_started && json.charAt(i) == ':' && ((json.charAt(i+1) >= '0' && json.charAt(i+1) <= '9') || json.charAt(i+1) == '+' || json.charAt(i+1) == '-')) || (key_started && key_end && value_started && !com_en && (json.charAt(i) == ',' || json.charAt(i) == '}'))){
      if(value_started){ //end of value found
        key_started = false;
        key_end = false;
        value_started = false;
        //Serial.print(key); Serial.print(": "); Serial.println(value);
        if(key == F("lon") && object == 1) longitude = value.toFloat();
        else if(key == F("lat") && object == 1) latitude = value.toFloat();
        else if(key == F("temp") && object == 4) temp = value.toFloat();
        else if(key == F("pressure") && object == 4) pressure = value.toFloat();
        else if(key == F("humidity") && object == 4) humidity = value.toFloat();
        else if(key == F("temp_min") && object == 4) temp_min = value.toFloat();
        else if(key == F("temp_max") && object == 4) temp_max = value.toFloat();
        else if(key == F("feels_like") && object == 4) feels_like = value.toFloat();
        else if(key == F("visibility") && object == 0) visibility = value.toInt();
        else if(key == F("speed") && object == 5) wind_speed = value.toFloat();
        else if(key == F("deg") && object == 5) wind_direction = value.toInt();
        else if(key == F("all") && object == 6) clouds = value.toInt();
        else if(key == F("1h") && object == 7) rain_1h = value.toFloat();
        else if(key == F("3h") && object == 7) rain_3h = value.toFloat();
        else if(key == F("1h") && object == 8) snow_1h = value.toFloat();
        else if(key == F("3h") && object == 8) snow_3h = value.toFloat();
        else if(key == F("country") && object == 9) strncpy(country, value.c_str(),5);
        else if(key == F("sunrise") && object == 9) sunrise = atol(value.c_str());
        else if(key == F("sunset") && object == 9) sunset = atol(value.c_str());
        else if(key == F("timezone") && object == 0) timezone = value.toInt();
        else if(key == F("id") && object == 0) city_id = value.toInt();
        else if(key == F("name") && object == 0) strncpy(city_name, value.c_str(),59);
        else if(key == F("dt") && object == 0) data_time = atol(value.c_str());
        else if(arr_index < maxWeatherInfoCount && object == 2){ //filling array
          if(arr_index < 0){
            arr_index = 0; //for non array
            arr_size = 1;
          }
          if(key == F("id")) tmp_w[arr_index].id = value.toInt();
          else if(key == F("main")) strncpy(tmp_w[arr_index].main, value.c_str(),39);
          else if(key == F("description")) strncpy(tmp_w[arr_index].description, value.c_str(),39);
          else if(key == F("icon")) strncpy(tmp_w[arr_index].icon, value.c_str(),9);
          tmp_w[arr_index].main[39] = 0;
          tmp_w[arr_index].description[39] = 0;
          tmp_w[arr_index].icon[9] = 0;

          /*Serial.println(tmp_w[arr_index].id);
          Serial.println(tmp_w[arr_index].main);
          Serial.println(tmp_w[arr_index].description);
          Serial.println(tmp_w[arr_index].icon);*/
        }

        /*Serial.println();
        Serial.print("Key: ");
        Serial.println(key);
        Serial.print("Value: ");
        Serial.println(value);*/
        if(json.charAt(i) == '}'){
          if(array_started){ //end of one item in array
            if(object == 2) arr_size = arr_index + 1;
            //Serial.println("C");
          }
          else{
            if(object == 0){
              break; //end of json
              //Serial.println("E");
            }
            object = 0; //end of object
            arr_index = -1;
            //Serial.println("D");
          }
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
    else if(json.charAt(i) == '[' && key_started && key_end && !value_started){ //start of an array
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
      /*Serial.println();
      Serial.print("Object: ");
      Serial.println(key);*/
      key_end = false;
      key_started = false;
      array_started = true;
      arr_index = -1;
      //Serial.println("A");
    }
    else if(json.charAt(i) == ']' && !value_started && (!key_started || (key_started && key_end)) && object > 0 && array_started){ //end of an array
      if(object == 2) { //array is available only on object 2
        arr_size = arr_index + 1;
      }
      key_end = false;
      key_started = false;
      array_started = false;
      //Serial.println("B");
    }
    else if(json.charAt(i) == '{' && array_started){// item of array started
      arr_index++;
      /*Serial.println();
      Serial.print("arr_index: ");
      Serial.println(arr_index);*/
    }
    else if(json.charAt(i) == '{' && key_end && !value_started && !array_started){ //new object found
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
      /*Serial.println();
      Serial.print("Object: ");
      Serial.println(key);*/
      key_end = false;
      key_started = false;
    }
    else if(json.charAt(i) == '}'){
      if(array_started){ //end of one item in array
        if(object == 2) arr_size = arr_index + 1;
        //Serial.println("C");
      }
      else{
        if(object == 0){
          break; //end of json
          //Serial.println("E");
        }
        object = 0; //end of object
        arr_index = -1;
        //Serial.println("D");
      }
    }
    else if(key_started && !key_end){
      key += json.charAt(i);
    }
    else if(key_started && key_end && value_started){
      value += json.charAt(i);
    }
    
  }

  if(arr_size > 0){
    if(weatherInfo && weatherInfoCount >= 1) delete[] weatherInfo;
    weatherInfoCount = arr_size;
    if(weatherInfoCount > maxWeatherInfoCount) weatherInfoCount = maxWeatherInfoCount;
    weatherInfo = new WeatherInfo[weatherInfoCount];
    if(!weatherInfo){
      status = HTTPC_ERROR_TOO_LESS_RAM;
      delete[] tmp_w; //delete temporary array
      return; //allocation error
    }
    for(int i = 0; i < weatherInfoCount; i++){ //copy data from temporary array
      weatherInfo[i] = tmp_w[i];
      /*weatherInfo[i].id = tmp_w[i].id;
      strncpy(weatherInfo[i].main, tmp_w[i].main,39);
      strncpy(weatherInfo[i].description, tmp_w[i].description,39);
      strncpy(weatherInfo[i].icon, tmp_w[i].icon,9);
      weatherInfo[i].main[39] = 0;
      weatherInfo[i].description[39] = 0;
      weatherInfo[i].icon[9] = 0;*/
    }
  }
  else{
    weatherInfo = NULL;
    weatherInfoCount = 0;
  }
  delete[] tmp_w; //delete temporary array
}

void OpenWeather::clear(){
  longitude = 0.00;
  latitude = 0.00;
  weatherInfo = NULL;
  weatherInfoCount = 0;
  temp = 0.00;
  temp_max = 0.00;
  temp_min = 0.00;
  feels_like = 0.00;
  pressure = 0.00;
  humidity = 0.00;
  visibility = 0;
  wind_speed = 0.00;
  wind_direction = 0;
  clouds = 0;
  rain_1h = 0.00;
  rain_3h = 0.00;
  snow_1h = 0.00;
  snow_3h = 0.00;
  sunrise = 0UL;
  sunset = 0UL;
  country[0] = '\0';
  timezone = 0L;
  city_name[0] = '\0';
  city_id = 0UL;
  units = metric;
  data_time = 0UL;
  status = OW_NONE;
}

HTTPClient OpenWeather::http;
WiFiClient OpenWeather::httpClient;

#endif