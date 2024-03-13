#include <Arduino.h>
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

