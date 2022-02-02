#include <Arduino.h>

// include library yang dibutuhkan untuk encode json yang akan
// dikirimkan melaluui software serial
#include <ArduinoJson.h>

// include library yang dibutuhkan untuk sensor
#include <Ultrasonic.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// pin software serial pada arduino yang disambungkan pada esp
#include <SoftwareSerial.h>
SoftwareSerial espSerial(2,3);

// define pin pin uang dibutuhkan pada arduino
#define pinTRIG   9
#define pinECHO   10
#define pinSOIL   0
#define pinDHT    11
#define pinLDR    2
const int relay[] = {4,5,6,7}; // pin pin yang dibutuhkan untuk keempat relay
const unsigned long interval = 1000; // interval waktu atau pengganti `delay()` pada loop

DHT dht(pinDHT, DHT11);
Ultrasonic hcsr(pinTRIG, pinECHO);

// global varibale untuk data bacaaan sensor
long  val_ldr;   
long  val_soil;   
int   val_hcsr;   
float val_dhthumid;   
float val_dhttemp;   


void setup() {
  // setup serial
  espSerial.begin(115200);
  // mengubah mode pin relay menjadi output
  for (int i = 0; i < 4; i++){
    pinMode(relay[i], OUTPUT);
  }
  dht.begin();
}

void loop() {
  // menggantikan `delay()` menggunakan millis agar tidak blocking code
  const unsigned long mills = millis();
  unsigned long now = 0;
  if ((unsigned long)(mills - now) > interval){
    // semua perkondisian sensor pada relay
    val_dhthumid = dht.readHumidity(false);
    if ( val_dhthumid >= 80.0){ digitalWrite(relay[0], LOW);
    } else if ( val_dhthumid <= 80.0){
    } else if ( val_dhthumid <= 70.0){ digitalWrite(relay[0], HIGH); }

    val_dhttemp = dht.readTemperature(false, false);
    if ( val_dhttemp <= 28.0){ digitalWrite(relay[1], LOW);
    } else if ( val_dhttemp >= 28.0){
    } else if ( val_dhttemp >= 32.0){ digitalWrite(relay[1], HIGH); }

    val_hcsr = hcsr.read(CM);
    if ( val_hcsr >= 150){ digitalWrite(relay[2], HIGH);
    } else if ( val_hcsr >= 100){
    } else if ( val_hcsr >= 75){
    } else if ( val_hcsr >= 50){
    } else if ( val_hcsr >= 20){ digitalWrite(relay[2], LOW); }

    val_soil = map(analogRead(pinSOIL),0 ,1024, 0, 100);
    if (val_soil >= 50){ digitalWrite(relay[3], HIGH);
    } else if ( val_soil <= 50){
    } else if ( val_soil <= 30){ digitalWrite(relay[3], LOW); }
    val_ldr = map(analogRead(pinLDR), 0, 1024, 0, 100);

    // membuat json dinamic dokumen yang isinya semua value sensor
    // yang sudah di inisialisasikan menjadi json tersebut dan dikirimkan
    // pada software serial dan akan di terima oleh ESP8266
    DynamicJsonDocument docs(1024);
    docs["humid"] = val_dhthumid;
    docs["temp"] = val_dhttemp;
    docs["hcsr"] = val_hcsr;
    docs["soil"] = val_soil;
    docs["ldr"] = val_ldr;

    serializeJson(docs, espSerial);
  }
}
