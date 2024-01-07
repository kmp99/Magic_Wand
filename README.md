Project Overview:
The Robust Magic Wand project aims to implement a versatile "Magic Wand" application capable of recognizing and classifying various wizard spells, charms, and curses. The magic wand utilizes an Arduino Nano 33 BLE Sense board with an Inertial Measurement Unit (IMU) for gesture recognition. The project comprises two main components: IMU Capture and Gesture Classification.

Spells, Charms, and Curses:
The magic wand recognizes the following magical actions:

Alohomora Charm: Opens closed doors.
Avada Kedavra: Causes instantaneous death.
Locomotor Charm: Moves the target.
Arresto Momentum Charm: Slows or stops a target's velocity.
Revelio: Reveals hidden objects.
Assumptions and Hints:
Magic wand length: ~30 cm.
Hold wand with Arduino board facing up.
IMU sensor sampling rate: 119 Hz.
Utilize both accelerometer and gyroscope data (6 values per sample).
Extract useful features on Arduino.
Consider useful data augmentations for improved model robustness.
Experiment with different model architectures, considering Arduino limitations.
Model training time on Colab should be under 3 minutes.
Optimize the model's footprint through quantization and efficient layer usage.
Robustness Challenges:
Recognizing spells is akin to handwriting, each person having a unique style.
IMU sensors may not be identical, posing a robustness challenge.
Address robustness issues considering in-distribution and out-of-distribution generalization.
Reproducibility and Robustness Measures:
Share train and test datasets with Olga for validation.
Two datasets: OLGA (collected by Olga) and CLASS (compiled from all participants).
Ensure reproducibility by achieving consistent results.
Evaluate model robustness through average accuracy on provided datasets.
Efficiency Measures:
Fix seeds for reproducibility.
Code compatibility for both Colab (model training) and Arduino (model inference).
Provide information on the amount of data used, model size, FLOPs, and inference time.
What I am doing?
In this Project, I will show you how to:

Use machine learning to recognize wand gestures.
Build an Arduino-powered magic wand.
Record your own training data.
Train and deploy an ML model to recognize custom spell gestures.
Project Hardware Requirements:
Arduino Nano 33 BLE Sense board.
USB Cable.
Magic wand should be ~30 cm long.
Project Software Requirements:
Download the right version for your OS at Arduino Software.
Run the Arduino IDE.
Install the Board Package: Boards -> Boards Manager and search for "mbed" to install the "Arduino mbed-enabled Boards" package.
Testing the Setup:
To ensure your Arduino is connected correctly, go to File -> Example -> Basics -> Blink and try uploading a sketch that blinks an LED.

IMU Capture (Arduino Code):
Description:
The IMU Capture component captures acceleration and gyroscope data when significant motion is detected. It uses an Inertial Measurement Unit (IMU) to collect sensor data and prints the data in CSV format to the Serial Monitor.

Key Parameters:
Acceleration Threshold: 2.5 G's.
Number of Samples to Capture: 119.
Gesture Classification (Arduino Code):
The Gesture Classification component reads captured data, normalizes it, and feeds it into a TensorFlow Lite model for classification. The model predicts the gesture performed and outputs the result to the Serial Monitor. TensorFlow Lite for Microcontrollers is used, with model details and operations resolver included in the code.

Model Architecture:
The TensorFlow Lite model for gesture classification is embedded in the Arduino code. It recognizes five gestures: "alohomora," "avadakedavra," "locomotor," "revelio," and "arrestomomentum." The model includes input normalization and an output layer with softmax activation for multi-class classification.

Data Augmentation:
Data augmentation is applied to the original sensor data to increase the diversity of the training dataset, improving the model's ability to recognize gestures under various conditions.

Hyperparameter Tuning:
The hyperparameters of the TensorFlow Lite model are predefined and embedded in the Arduino code. Fine-tuning may require adjusting values and retraining the model.

Regularization and Dropout:
Regularization and dropout are implemented in the model architecture to prevent overfitting:

kernel_regularizer=tf.keras.regularizers.l2(0.01): L2 regularization on weights.
tf.keras.layers.Dropout(0.5): Dropout applied to input.
Project Limitations and Future Work:
Assumes a predefined set of gestures, limiting adaptability.
Real-world testing and validation needed for diverse environments.
Model optimization for Arduino memory constraints can be explored.
Continuous improvement through additional training and retraining is recommended.
Conclusion:
The Magic Wand Gesture Recognition project demonstrates the integration of Arduino, IMU sensor, and TensorFlow Lite for Microcontrollers to recognize gestures. The provided code serves as a foundation for further exploration, improvements, and adaptations to specific use cases.
