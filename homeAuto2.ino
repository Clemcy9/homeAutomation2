#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

//network credentials
const char* apn = "MelcatAuto";
const char* pswd = "123456789";

//web credentials
const char* username = "admin";
const char* password = "admin";

//web server port
AsyncWebServer server(80);

//web variables
const int gen = 2;
const int light = 13;
const int pump = 14;

//web placeholders
String genstate, lightstate,pumpstate;

//function to get control states
String GenState(){
  if(digitalRead(gen)){
     genstate = "ON";
  }
  else{
    genstate = "OFF";
  }
  return genstate;
}
String LightState(){
  if(digitalRead(light)){
      lightstate = "ON";
  }
  else{
    lightstate = "OFF";
  }
  return lightstate;
}
String PumpState(){
  if(digitalRead(pump)){
    pumpstate = "ON";
  }
  else{
    pumpstate = "OFF";
  }
  return pumpstate;    
}

String processor (const String& var){
  Serial.println(var);
  if(var == "Power"){
    return GenState();
   }
  else if(var == "Light"){
    return LightState();
  }
  else if(var == "Pump"){
    return PumpState();
  }
}

void setup(){
  Serial.begin(115200);
  Serial.println("To God be the Glory");
  //initializing Wifi access point
  WiFi.softAP(apn,pswd);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Ap ip address is:");
  Serial.println(IP);
         
  //initializing pinmodes for power =12, light=13 and pump=14
  for(int i = 12; i<=14; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(2, OUTPUT);

  //initializing SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("could not start SPIFFS");
  }

  //Routes for Placeholder Updating (power,light,pump)
//  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", GenState().c_str());
//  });
//
//  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", LightState().c_str());
//  });
//
//  server.on("/pump", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send_P(200, "text/plain", PumpState().c_str());
//  });

  //Route for images
  server.on("/Logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Logo.png", "image/png");
  });
  
   // Routes to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/bootstrap-grid.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap-grid.min.css", "text/css");
  });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });
  server.on("/bootstrap4-toggle.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap4-toggle.min.css", "text/css");
  });

  //Routes for javaScript files
  server.on("/jquery3.2.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/jquery3.2.js", "text/js");
  });
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.js", "text/js");
  });
  server.on("/bootstrap4-toggle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap4-toggle.min.js", "text/js");
  });
    
  //Routes for web pages
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(username, password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });
  server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  //routes for generator
  server.on("/onGen", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(gen,HIGH);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offGen", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(gen,LOW);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  //routes for Lighting
  server.on("/onLight", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(light,HIGH);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offLight", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(light,LOW);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  //routes for water pump
  server.on("/onPump", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(pump,HIGH);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offPump", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(pump,LOW);
    request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.begin();
}
void loop(){

}
