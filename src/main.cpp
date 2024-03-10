#include <Arduino.h>
#include "Statistic.h"

statistic::Statistic<float, uint32_t, true> myStats;

const int NUM_MEASUREMENTS = 10;
//  defines pins numbers
const int trigPin = 9;
const int echoPin = 10;

int readSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  // print the distance on the serial monitor
  Serial.print("sensed:");
  Serial.println(distance);
  return distance;
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myStats.clear();
}

void loop() {

  myStats.clear(); //explicitly start clean

  // Perform 5 measurements
  Serial.println("Performing 5 measurements...");
  for (int i = 0; i < NUM_MEASUREMENTS; ++i) {
      int distance = readSensor();
      if (distance > 0) {
        myStats.add(distance);
      }
      delay(1000); // Adjust delay as needed
  }

  // Find the most common measurement
  Serial.print("        Count: ");
  Serial.println(myStats.count());
  Serial.print("          Min: ");
  Serial.println(myStats.minimum(), 4);
  Serial.print("          Max: ");
  Serial.println(myStats.maximum(), 4);
  Serial.print("      Average: ");
  Serial.println(myStats.average(), 4);
  Serial.print("     variance: ");
  Serial.println(myStats.variance(), 4);
  Serial.print("    pop stdev: ");
  Serial.println(myStats.pop_stdev(), 4);
  Serial.print(" unbias stdev: ");
  Serial.println(myStats.unbiased_stdev(), 4);
  Serial.print("       Middle: ");
  Serial.println(myStats.middle(), 4);
  Serial.print("    pop stdev: ");
  Serial.println(myStats.pop_stdev(), 4);
  Serial.print(" range/stddev: ");
  Serial.println(myStats.range() / myStats.pop_stdev(), 4);
  Serial.println("=====================================");
  myStats.clear();
  // Wait before starting the next set of measurements
  delay(5000); // Adjust delay as needed
}

