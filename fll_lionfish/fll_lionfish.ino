#include "HUSKYLENS.h"
#include "SoftwareSerial.h"

const int joystickXPin = A0;
const int joystickYPin = A1;

const int motor1Pin = 3;  // Example PWM pin for motor 1
const int motor2Pin = 5;  // Example PWM pin for motor 2
const int motor3Pin = 6; // Example PWM pin for motor 3
const int motor4Pin = 9; // Example PWM pin for motor 4
const int solenoidPin = 7;
const int joystickButtonPin = 2; // Button on joystick
int previousButtonState = LOW;
unsigned long lastDebounceTime = 0; //debouncing for the joystick button
const long debounceDelay = 50; //time to debounce

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(motor3Pin, OUTPUT);
    pinMode(motor4Pin, OUTPUT);
    pinMode(solenoidPin, OUTPUT);
    digitalWrite(solenoidPin, LOW); // Ensure solenoid is off at the start
    pinMode(joystickButtonPin, INPUT_PULLUP); //set button as input
    
    
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
    bool harpoonActive = false; //variable for harpoon state
    
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            if( isLionfish(result) ) {
                Serial.println("Lionfish detected");
                harpoonActive = true; //set harpoon active if true
                
            }
            
            else {
                Serial.println("Not a lionfish!");
            }
        }
    }
    
    // Check Joystick Button
    int currentButtonState = digitalRead(joystickButtonPin);

     if (currentButtonState == LOW){
            harpoonActive = true; //set harpoon active if true, while button is being held down.
        }
    
        if(harpoonActive){
            digitalWrite(solenoidPin, HIGH);
        } else {
            digitalWrite(solenoidPin, LOW);
        }

// Read button state

  // Read Joystick Values
  int joystickX = analogRead(joystickXPin); // 0 - 1023
  int joystickY = analogRead(joystickYPin); // 0 - 1023

  // Map Joystick Values to Motor Speeds (0 to 255)
  int motor1Speed = 0;
  int motor2Speed = 0;
  int motor3Speed = 0;
  int motor4Speed = 0;
  if(joystickY > 337){
        motor1Speed = map(joystickY, 337, 675, 0, 255);
        motor2Speed = map(joystickY, 337, 675, 0, 255);
    }
    
    if(joystickX > 337){
        motor3Speed = map(joystickX, 337, 675, 0, 255);
        motor4Speed = map(joystickX, 337, 675, 0, 255);
    }

  // Set Motor Speeds with PWM
  analogWrite(motor1Pin, motor1Speed);
  analogWrite(motor2Pin, motor2Speed);
  analogWrite(motor3Pin, motor3Speed);
  analogWrite(motor4Pin, motor4Speed);

  // Debugging Serial output (optional)
    Serial.print("Button State: ");
    Serial.print(currentButtonState);
     Serial.print(" X:");
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

  delay(50); // Small delay for stability
}


bool isLionfish(HUSKYLENSResult result){
 if (result.command == COMMAND_RETURN_BLOCK || result.command == COMMAND_RETURN_ARROW){
        if (result.ID==1){
        return true;
        }
        else { return false; }
}
}

void printResult(HUSKYLENSResult result){
    if( isLionfish(result) ) { 
          Serial.println("Lionfish detected");
          }

        else {
          Serial.println("Not a lionfish!");
        }
    }
