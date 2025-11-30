#include <AccelStepper.h>
#include <math.h>

#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN  3
#define MOTOR_Y_STEP_PIN 4
#define MOTOR_Y_DIR_PIN  5
#define MOTOR_Z_STEP_PIN 6
#define MOTOR_Z_DIR_PIN  7
#define LIMIT_X_PIN 22
#define LIMIT_Y_PIN 24
#define LIMIT_Z_PIN 26
#define LOW_SHANK_LENGTH  120.0 
#define HIGH_SHANK_LENGTH 120.0 
#define END_EFFECTOR_OFFSET 54.0
#define HOME_SPEED 200.0     
#define HOME_BACKOFF 50      
#define STEPS_PER_REV 200.0   
#define MICROSTEPS 16.0       
#define GEAR_RATIO 4.5        

float STEPS_PER_RAD = (GEAR_RATIO * STEPS_PER_REV * MICROSTEPS) / (2.0 * PI);

AccelStepper stepperX(AccelStepper::DRIVER, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN);

String inputString = ""; 
bool stringComplete = false;

float theta1, theta2, theta3;

void setup() {
  Serial.begin(115200);
  pinMode(LIMIT_X_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Z_PIN, INPUT_PULLUP);
  stepperX.setMaxSpeed(1000.0); stepperX.setAcceleration(500.0);
  stepperY.setMaxSpeed(1000.0); stepperY.setAcceleration(500.0);
  stepperZ.setMaxSpeed(1000.0); stepperZ.setAcceleration(500.0);
  Serial.println(">>> BAT DAU QUA TRINH VE HOME (HOMING)...");
  homeAxis(&stepperZ, LIMIT_Z_PIN, -1);
  homeAxis(&stepperY, LIMIT_Y_PIN, -1);
  homeAxis(&stepperX, LIMIT_X_PIN, -1);
  Serial.println(">>> VE HOME HOAN TAT! ROBOT SAN SANG.");
  Serial.println("Nhap toa do: X,Y,Z (VD: 100,100,50)");
}

void homeAxis(AccelStepper* stepper, int limitPin, int direction) {
  while (digitalRead(limitPin) == HIGH) { 
    stepper->setSpeed(direction * HOME_SPEED);
    stepper->runSpeed();
  }
  stepper->stop(); 
  stepper->setCurrentPosition(0); 
  stepper->runToNewPosition(-direction * HOME_BACKOFF); 
  stepper->setCurrentPosition(0); 
}
void loop() {
  if (stringComplete) {
    parseCoordinates(inputString);
    inputString = "";         
    stringComplete = false;
  }
  stepperX.run();
  stepperY.run();
  stepperZ.run();
}
bool calculateIK(float x, float y, float z) {
  float rrot_ee = hypot(x, y);           
  float rrot = rrot_ee - END_EFFECTOR_OFFSET; 
  float rside = hypot(rrot, z);         
  if (rside > (LOW_SHANK_LENGTH + HIGH_SHANK_LENGTH)) {
    Serial.println("LOI: Toa do ngoai tam voi!");
    return false;
  }
  float rside_2 = sq(rside);
  float low_2 = sq(LOW_SHANK_LENGTH);
  float high_2 = sq(HIGH_SHANK_LENGTH);
  theta1 = asin(x / rrot_ee); 
  float cos_val = (low_2 + high_2 - rside_2) / (2 * LOW_SHANK_LENGTH * HIGH_SHANK_LENGTH);
  if(cos_val > 1.0) cos_val = 1.0;
  if(cos_val < -1.0) cos_val = -1.0;
  theta3 = PI - acos(cos_val);
  float ang_a = acos((low_2 - high_2 + rside_2) / (2 * LOW_SHANK_LENGTH * rside));
  float ang_b = (z > 0) ? acos(z / rside) : asin(rrot / rside);
  if (z > 0) theta2 = ang_b - ang_a;
  else       theta2 = PI - ang_b - ang_a;
  theta3 = theta3 + theta2; 
  return true;
}

void runToCoords(float x, float y, float z) {
  if (calculateIK(x, y, z)) {
    long s1 = theta1 * STEPS_PER_RAD;
    long s2 = theta2 * STEPS_PER_RAD;
    long s3 = theta3 * STEPS_PER_RAD;
    Serial.print("MOVING -> Steps: ");
    Serial.print(s1); Serial.print(", ");
    Serial.print(s2); Serial.print(", ");
    Serial.println(s3);
    stepperX.moveTo(s1);
    stepperY.moveTo(s2);
    stepperZ.moveTo(s3);
  }
}
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') stringComplete = true;
  }
}

void parseCoordinates(String cmd) {
  int c1 = cmd.indexOf(',');
  int c2 = cmd.indexOf(',', c1 + 1);
  
  if (c1 > 0 && c2 > 0) {
    float x = cmd.substring(0, c1).toFloat();
    float y = cmd.substring(c1 + 1, c2).toFloat();
    float z = cmd.substring(c2 + 1).toFloat();
    runToCoords(x, y, z);
  }
}