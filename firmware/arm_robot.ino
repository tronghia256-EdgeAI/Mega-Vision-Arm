#include <AccelStepper.h>

#define MOTOR1_STEP_PIN 2
#define MOTOR1_DIR_PIN  3
#define MOTOR2_STEP_PIN 4
#define MOTOR2_DIR_PIN  5
#define MOTOR3_STEP_PIN 6
#define MOTOR3_DIR_PIN  7

AccelStepper stepper1(AccelStepper::DRIVER, MOTOR1_STEP_PIN, MOTOR1_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, MOTOR3_STEP_PIN, MOTOR3_DIR_PIN);

String inputString = ""; 
bool stringComplete = false;

void setup() {
  Serial.begin(115200);
  Serial.println("He thong san sang!");
  Serial.println("Nhap lenh dang: ID_DongCo,Vi_Tri_Buoc (VD: 1,2000)");
  stepper1.setMaxSpeed(2000.0);
  stepper1.setAcceleration(1000.0); 
  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(1000.0);
  stepper3.setMaxSpeed(2000.0);
  stepper3.setAcceleration(1000.0);
}

void loop() {
  if (stringComplete) {
    parseCommand(inputString);
    inputString = "";         
    stringComplete = false;
  }
  stepper1.run();
  stepper2.run();
  stepper3.run();
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') { 
      stringComplete = true;
    }
  }
}
void parseCommand(String command) {
  int commaIndex = command.indexOf(',');
  
  if (commaIndex > 0) {
    int motorID = command.substring(0, commaIndex).toInt();
    long targetPos = command.substring(commaIndex + 1).toInt();

    Serial.print("Motor: "); Serial.print(motorID);
    Serial.print(" -> Den vi tri: "); Serial.println(targetPos);

    switch (motorID) {
      case 1:
        stepper1.moveTo(targetPos); 
        break;
      case 2:
        stepper2.moveTo(targetPos);
        break;
      case 3:
        stepper3.moveTo(targetPos);
        break;
      default:
        Serial.println("Sai ID Motor! Chi nhap 1, 2 hoac 3.");
    }
  }
}