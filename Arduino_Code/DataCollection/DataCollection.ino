/*
  IMU Capture

  This example uses the on-board IMU to read acceleration and gyroscope
  data when significant motion is detected and prints it to the Serial Monitor.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Krishan Mohan Patel

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

// Threshold of significant motion in G's
const float accelerationThreshold = 2.5;
// Number of samples to capture
const int numSamples = 119;

int samplesRead = numSamples;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Print the CSV header
  Serial.println("aX,aY,aZ,gX,gY,gZ");
}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // Wait for significant motion
  while (samplesRead == numSamples) {
    if (IMU.accelerationAvailable()) {
      // Read the acceleration data
      IMU.readAcceleration(aX, aY, aZ);

      // Sum up the absolutes
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      // Check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // Reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  }

  // Check if all required samples have been read since
  // the last time significant motion was detected
  while (samplesRead < numSamples) {
    // Check if both new acceleration and gyroscope data are available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // Read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      samplesRead++;

      // Print the data in CSV format
      Serial.print(aX, 3);
      Serial.print(',');
      Serial.print(aY, 3);
      Serial.print(',');
      Serial.print(aZ, 3);
      Serial.print(',');
      Serial.print(gX, 3);
      Serial.print(',');
      Serial.print(gY, 3);
      Serial.print(',');
      Serial.print(gZ, 3);
      Serial.println();

      if (samplesRead == numSamples) {
        // Add an empty line if it's the last sample
        Serial.println();
      }
    }
  }
}
