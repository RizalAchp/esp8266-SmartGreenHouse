#include <Arduino.h>
// library Networking
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
// library jsonn
#include <ArduinoJson.h>


//TODO! menerima data dari arduino dan  memproses data tersebut sehingga dapat digunakan
//      untuk ditampilkan pada WEB!
long soil   ;
long ldr    ;
int hcsr    ;
float humid ;
float temp  ;

const char* ssid = "Bismillah";
const char* pswd = "MuhammadSAW";

AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  
  if(!SPIFFS.begin()){ return; }
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/humid", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(humid).c_str());
  });
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temp).c_str());
  });
  server.on("/hcsr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(hcsr).c_str());
  });
  server.on("/soil", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(soil).c_str());
  });
  server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(ldr).c_str());
  });

  server.begin();
}

void loop(){
  if (Serial.available()){
    DynamicJsonDocument docs(1024);
    DeserializationError error = deserializeJson(docs, Serial);
    if (error) {
      return;
    }
    humid = docs["humid"];
    temp  = docs["temp"];
    hcsr  = docs["hcsr"];
    soil  = docs["soil"];
    ldr   = docs["ldr"];

  }
}