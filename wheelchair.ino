/**
 * =============================================================================
 * Smart Solar-Powered Wheelchair — Embedded Control System
 * =============================================================================
 * Author      : Ogbodo Uchenna Maxwell Adrian (@MaxCybOps)
 * Institution : Federal University of Technology, Owerri (FUTO)
 * Supervisor  : Engr. Dr. V. O. Aniugo
 * Version     : 2.0.0
 * Platform    : Arduino Nano (ATmega328P)
 * =============================================================================
 * Description:
 *   Multi-modal control system for a solar-powered assistive wheelchair.
 *   Supports three independent control modes — Push-Button, Voice, and IR
 *   Remote — with continuous IR-based obstacle avoidance active across all
 *   modes to ensure user safety at all times.
 *
 * Control Modes:
 *   1. Push-Button  — Manual forward/stop via digital button
 *   2. Voice        — Hands-free via Voice Recognition V3 module
 *   3. IR Remote    — Navigation via standard IR remote controller
 *
 * Safety:
 *   IR obstacle sensor blocks forward motion in ALL modes when an
 *   obstacle is detected within range.
 * =============================================================================
 */

#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <IRremote.h>

// ---------------------------------------------------------------------------
// Pin Definitions
// ---------------------------------------------------------------------------
// Motor driver pins (H-Bridge)
const int MOTOR_LEFT_FORWARD   = 4;
const int MOTOR_LEFT_BACKWARD  = 5;
const int MOTOR_RIGHT_FORWARD  = 6;
const int MOTOR_RIGHT_BACKWARD = 7;

// Input pins
const int IR_SENSOR_PIN  = 8;   // IR obstacle sensor — HIGH = clear, LOW = obstacle
const int BUTTON_PIN     = 9;   // Push-button — LOW when pressed (INPUT_PULLUP)
const int IR_RECV_PIN    = 11;  // IR remote receiver

// ---------------------------------------------------------------------------
// IR Remote Button Codes
// ---------------------------------------------------------------------------
const unsigned long IR_FORWARD  = 0xFF629D;
const unsigned long IR_BACKWARD = 0xFFA857;
const unsigned long IR_LEFT     = 0xFF22DD;
const unsigned long IR_RIGHT    = 0xFFC23D;
const unsigned long IR_STOP     = 0xFF02FD;

// ---------------------------------------------------------------------------
// Voice Command Indices (mapped in Voice Recognition V3 module)
// ---------------------------------------------------------------------------
const uint8_t VOICE_FORWARD  = 0;
const uint8_t VOICE_BACKWARD = 1;
const uint8_t VOICE_LEFT     = 2;
const uint8_t VOICE_RIGHT    = 3;
const uint8_t VOICE_STOP     = 4;

// ---------------------------------------------------------------------------
// Module Instances
// ---------------------------------------------------------------------------
VR      myVR(2, 3);   // Voice Recognition V3: RX=2, TX=3
IRrecv  irRecv(IR_RECV_PIN);
decode_results irResults;

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println(F("=============================================="));
  Serial.println(F("  Smart Solar-Powered Wheelchair v2.0.0"));
  Serial.println(F("  Initializing control systems..."));
  Serial.println(F("=============================================="));

  // Motor output pins
  pinMode(MOTOR_LEFT_FORWARD,   OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD,  OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD,  OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Sensor and button input pins
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUTTON_PIN,    INPUT_PULLUP);

  // Initialize voice module
  myVR.begin(9600);
  Serial.println(F("[OK] Voice Recognition Module ready."));

  // Initialize IR remote receiver
  irRecv.enableIRIn();
  Serial.println(F("[OK] IR Remote Receiver ready."));

  // Ensure motors are off at startup
  stopMotors();
  Serial.println(F("[OK] Motors initialized — STOPPED."));
  Serial.println(F("[READY] Awaiting control input..."));
}

// ---------------------------------------------------------------------------
// Main Loop
// ---------------------------------------------------------------------------
void loop() {
  bool pathClear  = (digitalRead(IR_SENSOR_PIN) == HIGH);
  bool btnPressed = (digitalRead(BUTTON_PIN)    == LOW);

  // --- Mode 1: Push-Button Control ---
  if (btnPressed) {
    if (pathClear) {
      moveForward();
    } else {
      stopMotors();
      Serial.println(F("[SAFETY] Obstacle detected — forward motion blocked."));
    }
  }

  // --- Mode 2: Voice Control ---
  uint8_t voiceBuf[64];
  if (myVR.recognize(voiceBuf, 50) > 0) {
    handleVoiceCommand(voiceBuf[1], pathClear);
  }

  // --- Mode 3: IR Remote Control ---
  if (irRecv.decode(&irResults)) {
    handleIRCommand(irResults.value, pathClear);
    irRecv.resume();
  }
}

// ---------------------------------------------------------------------------
// Voice Command Handler
// ---------------------------------------------------------------------------
void handleVoiceCommand(uint8_t command, bool pathClear) {
  switch (command) {
    case VOICE_FORWARD:
      if (pathClear) { moveForward(); Serial.println(F("[VOICE] Forward")); }
      else           { stopMotors(); Serial.println(F("[VOICE] Forward blocked — obstacle.")); }
      break;
    case VOICE_BACKWARD:
      moveBackward();
      Serial.println(F("[VOICE] Backward"));
      break;
    case VOICE_LEFT:
      turnLeft();
      Serial.println(F("[VOICE] Left"));
      break;
    case VOICE_RIGHT:
      turnRight();
      Serial.println(F("[VOICE] Right"));
      break;
    case VOICE_STOP:
      stopMotors();
      Serial.println(F("[VOICE] Stop"));
      break;
    default:
      Serial.println(F("[VOICE] Unrecognized command."));
      break;
  }
}

// ---------------------------------------------------------------------------
// IR Remote Command Handler
// ---------------------------------------------------------------------------
void handleIRCommand(unsigned long code, bool pathClear) {
  switch (code) {
    case IR_FORWARD:
      if (pathClear) { moveForward(); Serial.println(F("[IR] Forward")); }
      else           { stopMotors(); Serial.println(F("[IR] Forward blocked — obstacle.")); }
      break;
    case IR_BACKWARD:
      moveBackward();
      Serial.println(F("[IR] Backward"));
      break;
    case IR_LEFT:
      turnLeft();
      Serial.println(F("[IR] Left"));
      break;
    case IR_RIGHT:
      turnRight();
      Serial.println(F("[IR] Right"));
      break;
    case IR_STOP:
      stopMotors();
      Serial.println(F("[IR] Stop"));
      break;
    default:
      // Ignore unrecognized IR codes silently
      break;
  }
}

// ---------------------------------------------------------------------------
// Motor Control Functions
// ---------------------------------------------------------------------------

/**
 * moveForward — Drives both motors in the forward direction.
 * Note: Always check pathClear before calling this function.
 */
void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD,   HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD,  HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD,  LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

/**
 * moveBackward — Drives both motors in the reverse direction.
 * Obstacle avoidance does not apply to backward movement.
 */
void moveBackward() {
  digitalWrite(MOTOR_LEFT_BACKWARD,  HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  digitalWrite(MOTOR_LEFT_FORWARD,   LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD,  LOW);
}

/**
 * turnLeft — Reverses left motor, drives right motor forward.
 * Produces an in-place left pivot turn.
 */
void turnLeft() {
  digitalWrite(MOTOR_LEFT_BACKWARD,  HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD,  HIGH);
  digitalWrite(MOTOR_LEFT_FORWARD,   LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

/**
 * turnRight — Drives left motor forward, reverses right motor.
 * Produces an in-place right pivot turn.
 */
void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD,   HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD,  LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD,  LOW);
}

/**
 * stopMotors — Cuts power to all motor pins immediately.
 * Called on obstacle detection and STOP commands.
 */
void stopMotors() {
  digitalWrite(MOTOR_LEFT_FORWARD,   LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD,  LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD,  LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
