/*
  Magic Wand - IMU Gesture Classifier

  This Arduino sketch turns your Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board
  into a Magic Wand. It uses the on-board IMU to read acceleration and gyroscope data,
  and a TensorFlow Lite (Micro) model to classify the movement as a known gesture.

  Note: Direct use of C/C++ pointers, namespaces, and dynamic memory is generally
  discouraged in Arduino examples, and in the future, the TensorFlowLite library
  might change to make the sketch simpler.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 BLE Sense board.

  Created by Krishan Mohan Patel

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

// Include the generated TensorFlow Lite model
#include "C:\Users\lenovo\Desktop\Krishna\Courses\Semester_I_Oct-Feb_2023-2024\Embedded_Machine Learning_Information and Computer Engineering\Magic_Wand\Arduino_Code\Data_Classification\Magic_Wond\model.h"

const float accelerationThreshold = 2.5; // Threshold of significance in G's
const int numSamples = 119;

int samplesRead = numSamples;

// Pull in all the TFLM ops, you can remove this line and
// only pull in the TFLM ops you need, if you wouFld like to reduce
// the compiled size of the sketch.
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM; the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

// Array to map gesture index to a name
const char* GESTURES[] = {
  "alohomora",
  "avadakedavra",
  "locomotor",
  "revelio",
  "arrestomomentum"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Print out the sample rates of the IMUs
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();

  // Get the TFL representation of the model byte array
  tflModel = tflite::GetModel(model);
  if (tflModel->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();

  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}

void loop() {
  unsigned int i;
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

  // Check if all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // Check if new acceleration AND gyroscope data is available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // Read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // Normalize the IMU data between 0 to 1 and store in the model's
     // input tensor
      tflInputTensor->data.f[samplesRead * 6 + 0] = (aX + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 1] = (aY + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 2] = (aZ + 4.0) / 8.0;
      tflInputTensor->data.f[samplesRead * 6 + 3] = (gX + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 4] = (gY + 2000.0) / 4000.0;
      tflInputTensor->data.f[samplesRead * 6 + 5] = (gZ + 2000.0) / 4000.0;

      samplesRead++;

      if (samplesRead == numSamples) {
        // Run inferencing
        TfLiteStatus invokeStatus = tflInterpreter->Invoke();
        if (invokeStatus != kTfLiteOk) {
          Serial.println("Invoke failed!");
          while (1);
          return;
        }

        // Loop through the output tensor values from the model
        for (i = 0; i < NUM_GESTURES; i++) {
          Serial.print(GESTURES[i]);
          Serial.print(": ");
          Serial.println(tflOutputTensor->data.f[i], 6);
        }
        Serial.println();
      }
    }
  }
}
