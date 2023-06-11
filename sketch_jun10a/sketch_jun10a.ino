#include "SevSeg.h"

const int MAX_POTENTIOMETER_VAL = 1023;

SevSeg sevseg; 


bool isOnHold = true;
bool isButtonOnCoolDown = false;


int displayNumber = 0;
bool halfHourDisplay = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(100);

  // punto decimal
  pinMode(9, OUTPUT); 
  //BUTTON
  pinMode(10,INPUT_PULLUP);
  //BUZZER
  pinMode(11,OUTPUT);
  digitalWrite(11, LOW);



}

void loop() {

  byte buttonState = digitalRead(10);

  if (isOnHold){

    sevseg.blank();
    sevseg.refreshDisplay();    

    //CLEAR POINT
    digitalWrite(9, LOW);
    //clear buzzer
    digitalWrite(11, LOW);

    if (buttonState == LOW && !isButtonOnCoolDown) {
        isButtonOnCoolDown = true;
        isOnHold = false;
        digitalWrite(11, HIGH);
        delay(200);
    }
    
  }else{
    
    //stop buzzer
    digitalWrite(11, LOW);
    readPotentiometer();
    //Display

    sevseg.setNumber(displayNumber);

    if(halfHourDisplay) digitalWrite(9, HIGH);
    else digitalWrite(9, LOW);

    sevseg.refreshDisplay();    

    byte buttonState = digitalRead(10);
    
    if (buttonState == LOW && !isButtonOnCoolDown) {
        isButtonOnCoolDown = true;
        //Serial.println("Button is pressed");
        digitalWrite(11, HIGH);
        isOnHold = true;
        delay(200);
    }
    
  }

  

  delay(10);  // delay in between reads for stability
}


void readPotentiometer(){

  // read the input on analog pin 0:
    int sensorValue = analogRead(A0);

    //to get a value from 0.0 - 9.5
    float normalized_value = ((float) sensorValue / (float) MAX_POTENTIOMETER_VAL) * 9.5; 

    //get only the part before decimals
    displayNumber = trunc(normalized_value);
    //get the decimal part and get if is greater that 0.5, to check it as a + half hour
    halfHourDisplay = normalized_value - int(normalized_value) >= 0.5;

    //Serial.println(String(normalized_value) + " Display: " + displayNumber);
}
