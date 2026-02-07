#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <IRremote.h>

// Voice module setup
VR myVR(2,3); // RX, TX pins for voice module

// IR remote setup
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Motor pins
int motorLeftForward = 4;
int motorLeftBackward = 5;
int motorRightForward = 6;
int motorRightBackward = 7;

// Sensors & buttons
int irSensor = 8;       // IR obstacle sensor
int buttonPin = 9;      // Push-button control

void setup() {
  Serial.begin(9600);

  // Voice module
  myVR.begin(9600);
  Serial.println("Voice Control Ready");

  // IR remote
  irrecv.enableIRIn();

  // Motor pins
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  // Sensors & buttons
  pinMode(irSensor, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int obstacle = digitalRead(irSensor);
  int buttonState = digitalRead(buttonPin);

  // --- Push-button mode ---
  if (buttonState == LOW) {
    if (obstacle == HIGH) {
      moveForward();
    } else {
      stopMotors();
    }
  }

  // --- Voice control mode ---
  uint8_t buf[64];
  if(myVR.recognize(buf, 50) > 0) {
    switch(buf[1]) {
      case 0: if(obstacle == HIGH) moveForward(); else stopMotors(); break; // "Forward"
      case 1: moveBackward(); break;   // "Backward"
      case 2: turnLeft(); break;       // "Left"
      case 3: turnRight(); break;      // "Right"
      case 4: stopMotors(); break;     // "Stop"
    }
  }

  // --- Remote control mode ---
  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 0xFF629D: if(obstacle == HIGH) moveForward(); else stopMotors(); break; // Up button
      case 0xFFA857: moveBackward(); break;  // Down button
      case 0xFF22DD: turnLeft(); break;      // Left button
      case 0xFFC23D: turnRight(); break;     // Right button
      case 0xFF02FD: stopMotors(); break;    // OK button
    }
    irrecv.resume();
  }
}

// --- Motor control functions ---
void moveForward() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightBackward, LOW);
}

void moveBackward() {
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightBackward, HIGH);
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorRightForward, LOW);
}

void turnLeft() {
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
}

void turnRight() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorRightBackward, HIGH);
}

void stopMotors() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightBackward, LOW);
}
