#include <Arduino.h>

////////////////////////
// AWS IoT Integration
#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_NICLA_VISION) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_GIGA) || defined(ARDUINO_OPTA)
  #include <WiFi.h>
#elif defined(ARDUINO_PORTENTA_C33)
  #include <WiFiC3.h>
#elif defined(ARDUINO_UNOR4_WIFI)
  #include <WiFiS3.h>
#endif
#include <WiFiClientSecure.h>
#include "arduino_secrets.h"

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiClientSecure wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "a1taodwra0tmmc-ats.iot.us-east-1.amazonaws.com";
int        port     = 1883;
const char topic[]  = "home/water-softener";

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;
//
////////////////////////

#include "mode_calculator.h"

#define MAX_MEASUREMENTS 10 // Define
int samples[MAX_MEASUREMENTS]; // Create an empty array

//  defines pins numbers
const int trigPin = 9;
const int echoPin = 10;


// Function to reset the array values to zero
void resetSamples() {
    for (int i = 0; i < MAX_MEASUREMENTS; i++) {
        samples[i] = 0; // Reset each element to zero
    }
}

//
int readSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  // print the distance on the serial monitor
  Serial.print("sampled:");
  Serial.println(distance);
  return distance;
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Reset array values by reinitializing the array
  resetSamples();

  ////////////////////////
  // AWS IoT Integration
  // attempt to connect to WiFi network:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(SECRET_SSID);
  while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  wifiClient.setCertificate(AWS_CERT_CRT);
  wifiClient.setPrivateKey(AWS_CERT_PRIVATE);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  //
  ////////////////////////
}

void loop() {

  int sampleIndex = 0; // Variable to keep track of the current sample in the array
  resetSamples();

  // Perform MAX_MEASUREMENTS measurements
  Serial.print("Performing up to ");
  Serial.print(MAX_MEASUREMENTS);
  Serial.println(" measurements...");
  while (sampleIndex < MAX_MEASUREMENTS) {
    int distance = readSensor();
    if (distance > 0) {
        samples[sampleIndex] = distance;
        sampleIndex++; // Increment sampleIndex only if a valid distance is read
    }
    delay(1000); // Adjust delay as needed
  }

  // Calculate the mode
  int mode = calculateMode(samples, MAX_MEASUREMENTS);
  Serial.print("Mode: ");
  Serial.println(mode);
  Serial.println("=====================================");
  // Wait before starting the next set of measurements
  delay(5000); // Adjust delay as needed
}

