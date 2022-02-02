#include <Arduino.h>
// library Networking
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
// library jsonn
#include <ArduinoJson.h>


// global variable data sensor yang diterima pada json docs
// dari serial arduino
long soil   ;
long ldr    ;
int hcsr    ;
float humid ;
float temp  ;

// masukkan ssid dan password wifi anda dibawah ini
const char* ssid = "WIFI_ANDA";
const char* pswd = "PASSWORD_WIFI_ANDA";
// membuatserver pada port 80
AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  
  // menggunakan SPIFFS sebagai tempat index.html yang berada pada
  // folder data didalam projek
  if(!SPIFFS.begin()){ return; }
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  // menampilkan ip address yang di dapatkan oleh esp
  Serial.println(WiFi.localIP());

  // http get 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/humid", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(humid).c_str()); // mengirim data humid menjadi String
  });
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temp).c_str()); // megirim data temp menjadi String
  });
  server.on("/hcsr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(hcsr).c_str()); // mengirim data hcsr menjadi Stringg
  });
  server.on("/soil", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(soil).c_str()); // mengirim data soil menjadi String
  });
  server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(ldr).c_str()); // mengirim data ldr menjadi String
  });

  server.begin(); // memulai server tersebut
}

void loop(){
  // pada loop hanya memeriksa apakah ada data yang diterima di serial
  // jika tersedia maka akan di encode menggunakan json dan memasukkan value sensor pada
  // variable yang sudah disiapkan diatas.
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
