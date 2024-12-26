// Define pins
const int joystickXPin = A0;
const int joystickYPin = A1;

const int motor1Pin = 8;  // Example pin for motor 1
const int motor2Pin = 9;  // Example pin for motor 2
const int motor3Pin = 10; // Example pin for motor 3
const int motor4Pin = 11; // Example pin for motor 4
const int solenoidPin = 7;

bool harpoonTrigger = false; // Placeholder harpoon condition - modify this later

void setup() {
  Serial.begin(9600); // For debugging (optional)
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, LOW); // Ensure solenoid is off at the start
}

void loop() {
    
  // Read Joystick Values
  int joystickX = analogRead(joystickXPin); // 0 - 1023
  int joystickY = analogRead(joystickYPin); // 0 - 1023

  // Map Joystick Values to Motor Speeds (0 to 255)
    int motor1Speed = 0;
    int motor2Speed = 0;
    int motor3Speed = 0;
    int motor4Speed = 0;
  if(joystickY > 512){
   motor1Speed = map(joystickY, 512, 1023, 0, 255);
     motor2Speed = map(joystickY, 512, 1023, 0, 255);
  }
  if(joystickX > 512){
      motor3Speed = map(joystickX, 512, 1023, 0, 255);
      motor4Speed = map(joystickX, 512, 1023, 0, 255);
  }



  // Set Motor Speeds with PWM
   analogWrite(motor1Pin, motor1Speed);
  analogWrite(motor2Pin, motor2Speed);
   analogWrite(motor3Pin, motor3Speed);
  analogWrite(motor4Pin, motor4Speed);

  // Check harpoon condition and Trigger Solenoid
  if (harpoonTrigger) {
    digitalWrite(solenoidPin, HIGH); // Activate the solenoid
  } else {
    digitalWrite(solenoidPin, LOW);  // Ensure the solenoid is off.
  }


  // Debugging Serial output (optional)
  Serial.print("X:");
  Serial.print(joystickX);
  Serial.print(",Y:");
  Serial.print(joystickY);
      Serial.print(",M1:");
  Serial.print(motor1Speed);
    Serial.print(",M2:");
  Serial.print(motor2Speed);
      Serial.print(",M3:");
    Serial.print(motor3Speed);
      Serial.print(",M4:");
    Serial.print(motor4Speed);
  Serial.println();

  delay(10); // Small delay for stability
}