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
const int light = 4;
const int pump = 13;

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
  Serial.println("this is the value of pumpstate: ");
  Serial.println(pumpstate);
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
  return String();
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
  //Route for images
  server.on("/Logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Logo.png", "image/png");
  });
  
   // Routes to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  

  //Routes for javaScript files
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "text/js");
  });
  
    
  //Routes for web pages
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/control.html", String(), false, processor);
   });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(username, password))
      return request->requestAuthentication();
     request->redirect("/control");
//    request->send(SPIFFS, "/control.html", String(), false, processor);
  });
  // server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.html", String(), false, processor);
  // });
  
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

  //routes for generator
  server.on("/onGen", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(gen,HIGH);
    request->send(200, "text/plain", GenState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offGen", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(gen,LOW);
    request->send(200, "text/plain", GenState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  //routes for Lighting
  server.on("/onLight", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(light,HIGH);
    request->send(200, "text/plain", LightState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offLight", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(light,LOW);
    request->send(200, "text/plain", LightState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  //routes for water pump
  server.on("/onPump", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(pump,HIGH);
    request->send(200, "text/plain", PumpState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.on("/offPump", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(pump,LOW);
    request->send(200, "text/plain", PumpState().c_str());
    // request->send(SPIFFS, "/control.html", String(), false, processor);
  });

  server.begin();
}
void loop(){

}
