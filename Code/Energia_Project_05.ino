// Conveyor and Sorting System with Emergency Stop Toggle and Servo Sorters
// GROUP NO: 5(B)
// REG NO: 2023-MC-48
//         2023-MC-49
//         2023-MC-58


#include <LiquidCrystal.h>
#include <Servo.h>

// Stepper Motor Control
#define DIR_PIN PD_0
#define STEP_PIN PD_1

// Servo Pins
#define SERVO_A_PIN PD_2
#define SERVO_B_PIN PD_3

// IR Sensors
#define IR_SENSOR_A PE_0
#define IR_SENSOR_B PE_1
#define IR_SENSOR_C PE_2
#define IR_OVERLOAD PE_3  // Overload detection sensor

// Emergency Stop Button
#define EMERGENCY_BUTTON PC_4

// LCD Pins
#define RS PB_0
#define EN PB_1
#define D4 PB_2
#define D5 PB_3
#define D6 PB_4
#define D7 PB_5

// Onboard LEDs
#define RED_LED PF_1    // Red = Emergency Stop
#define BLUE_LED PF_2   // Blue = Overload

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// Servo objects
Servo servoA;
Servo servoB;

// Count variables
volatile int countA = 0;
volatile int countB = 0;
volatile int countC = 0;

unsigned long lastMotorStep = 0;
unsigned long motorStepInterval = 100; // motor step interval

bool emergencyStop = false;
bool overloadStop = false;

// Previous states for IR
bool lastA = HIGH;
bool lastB = HIGH;
bool lastC = HIGH;
bool lastOverload = HIGH;

// Overload tracking
int objectsOnConveyor = 0;
unsigned long lastOverloadTrigger = 0;
const unsigned long debounceTime = 300;

void setup() {
  // Stepper Motor pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, HIGH); // set motor direction

  // IR Sensor pins
  pinMode(IR_SENSOR_A, INPUT_PULLUP);
  pinMode(IR_SENSOR_B, INPUT_PULLUP);
  pinMode(IR_SENSOR_C, INPUT_PULLUP);
  pinMode(IR_OVERLOAD, INPUT_PULLUP);

  // Emergency button pin
  pinMode(EMERGENCY_BUTTON, INPUT); // External pull-down

  // LCD setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conveyor Ready");
  delay(1000);

  // Servo setup
  servoA.attach(SERVO_A_PIN);
  servoB.attach(SERVO_B_PIN);
  servoA.write(90);  // Initial position
  servoB.write(0);

  // LED setup
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void loop() {
  checkEmergencyButton();
  checkOverload();

  if (!emergencyStop && !overloadStop) {
    moveConveyorNonBlocking();
    checkSensors();
    digitalWrite(RED_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  } else {
    stopConveyor();
    if (emergencyStop) {
      digitalWrite(RED_LED, HIGH);
    } else {
      digitalWrite(RED_LED, LOW);
    }

    if (overloadStop) {
      digitalWrite(BLUE_LED, HIGH);
    } else {
      digitalWrite(BLUE_LED, LOW);
    }
  }

  updateLCD();
}

void checkEmergencyButton() {
  static bool lastButtonState = LOW;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  bool reading = digitalRead(EMERGENCY_BUTTON);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == HIGH) {
      emergencyStop = !emergencyStop;
      while (digitalRead(EMERGENCY_BUTTON) == HIGH); // wait for release
      delay(20);
    }
  }

  lastButtonState = reading;
}

// Refined Overload Logic: Only one object allowed at a time
void checkOverload() {
  static bool lastOverloadState = HIGH;
  static unsigned long lastOverloadHandled = 0;
  const unsigned long overloadCooldown = 1500; // Ignore repeated triggers for 1.5 sec

  bool currentOverload = digitalRead(IR_OVERLOAD);

  if (lastOverloadState == HIGH && currentOverload == LOW &&
      (millis() - lastOverloadHandled) > overloadCooldown) {

    if (objectsOnConveyor == 0) {
      objectsOnConveyor = 1;  // Accept the first object
    } else {
      overloadStop = true;   // Already an object on belt → overload
    }

    lastOverloadHandled = millis();  // Start cooldown timer
  }

  lastOverloadState = currentOverload;
}


void stopConveyor() {
  digitalWrite(STEP_PIN, LOW);
}

void moveConveyorNonBlocking() {
  if (micros() - lastMotorStep >= motorStepInterval) {
    lastMotorStep = micros();
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP_PIN, LOW);
  }
}

void checkSensors() {
  static unsigned long lastSensorCheck = 0;
  static unsigned long lastTriggerA = 0;
  static unsigned long lastTriggerB = 0;
  static unsigned long lastTriggerC = 0;
  const unsigned long retriggerDelay = 500;

  if (millis() - lastSensorCheck >= 10) {
    lastSensorCheck = millis();

    bool currentA = digitalRead(IR_SENSOR_A);
    bool currentB = digitalRead(IR_SENSOR_B);
    bool currentC = digitalRead(IR_SENSOR_C);

    if (lastA == HIGH && currentA == LOW && millis() - lastTriggerA >= retriggerDelay) {
      countA++;
      lastTriggerA = millis();
      activateServo(servoA);
      objectsOnConveyor = max(0, objectsOnConveyor - 1);
    }

    if (lastB == HIGH && currentB == LOW && millis() - lastTriggerB >= retriggerDelay) {
      countB++;
      lastTriggerB = millis();
      activateServo1(servoB);
      objectsOnConveyor = max(0, objectsOnConveyor - 1);
    }

    if (lastC == HIGH && currentC == LOW && millis() - lastTriggerC >= retriggerDelay) {
      countC++;
      lastTriggerC = millis();
      objectsOnConveyor = max(0, objectsOnConveyor - 1);
    }

    lastA = currentA;
    lastB = currentB;
    lastC = currentC;
  }
}

void activateServo(Servo &servo) {
  servo.write(0);     // Push position (reversed)
  delay(300);         
  servo.write(90);    // Return position
}

void activateServo1(Servo &servo) {
  servo.write(90);     // Push position (reversed)
  delay(300);         
  servo.write(0);    // Return position
}

void updateLCD() {
  static unsigned long lastLCDUpdate = 0;
  if (millis() - lastLCDUpdate >= 500) {
    lastLCDUpdate = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" A:");
    lcd.print(countA);
    lcd.print(" B:");
    lcd.print(countB);
    lcd.print(" C:");
    lcd.print(countC);

    lcd.setCursor(0, 1);
    if (emergencyStop) {
      lcd.print(" EMERGENCY STOP");
    } else if (overloadStop) {
      lcd.print("  OVERLOADED");
    } else {
      lcd.print("CONVEYOR WORKING ");
    }
  }
}
