#include "SevSeg.h"
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
//#include "./data.h"

const int MAX_POTENTIOMETER_VAL = 1023;
const unsigned long buttonCooldownDuration = 1000;  // 1 second cooldown

SevSeg sevseg; 
IRsendRaw mySender;

//onHold -> not setting timer, not onHold -> setting timer
bool isOnHold = true;
bool isButtonOnCoolDown = false;
//boolean to indicate if timer is currently "running"
bool isTimerRunning = false;
unsigned long buttonCooldownStartTime = 0;

//timer for ir activation
unsigned long timerStartTime = 0;
unsigned long desiredTimerDuration;

int displayNumber = 0;
bool halfHourDisplay = false;


#define RAW_DATA_LEN 48
uint16_t rawData[RAW_DATA_LEN]={
    1301, 362, 1279, 383, 447, 1215, 447, 1215, 469, 1194, 469, 1194, 447, 1194, 469, 1194,
    1301, 362, 469, 1194, 469, 1194, 469, 7957, 1301, 362, 1279, 383, 447, 1215, 469, 1194,
    469, 1194, 447, 1215, 447, 1215, 447, 1194, 1301, 383, 469, 1194, 469, 1194, 469 // Add the remaining codes
  };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {6, 5, 2, 12, 4, 7, 8};
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

  
    if(isTimerRunning){
      sevseg.setChars("-");
    }else{
      sevseg.blank();
    }
    
    sevseg.refreshDisplay();    

    //CLEAR POINT
    digitalWrite(9, LOW);
    //clear buzzer
    digitalWrite(11, LOW);

    if (buttonState == LOW && !isButtonOnCoolDown) {
        isButtonOnCoolDown = true;
        isOnHold = false;
        buttonCooldownStartTime = millis();
        digitalWrite(11, HIGH);
        delay(150);

        //TODO: REMOVE
        //testing to turn of the lights
        Serial.println("Sending ir");
        mySender.send(rawData, RAW_DATA_LEN, 38);
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
        Serial.println("Button is pressed");
        buttonCooldownStartTime = millis(); 

        //make buzzer sound
        digitalWrite(11, HIGH);
        


        //IR LOGIC
        if( halfHourDisplay || displayNumber != 0){
          Serial.println("Enters TIMER SET");
          //set a timer for the desired amout of time
          if(halfHourDisplay){
            desiredTimerDuration = (displayNumber + 0.5) * 60 * 60  * 1000;
          }else{

            desiredTimerDuration = (displayNumber + 0.0) * 60 * 60  * 1000;
          }
          Serial.print("SE HA ESTABLECIDO UN TIMER CON DURACION: ");
          Serial.println(desiredTimerDuration);
          timerStartTime = millis();
          isTimerRunning = true;
        }

        isOnHold = true;
        delay(150);
    }
    
  }

  if (isButtonOnCoolDown && (millis() - buttonCooldownStartTime >= buttonCooldownDuration)) {
    isButtonOnCoolDown = false;  // Reset the button cooldown flag
  }

  // Check if the timer duration has passed
  if (timerStartTime != 0 && (millis() - timerStartTime >= desiredTimerDuration)) {
    timerStartTime = 0;  
    // IR ACTIVATION HERE
    Serial.println("TIMER ACTIVATED");
    //RESET EVERY OTHER THING
    isOnHold = true;
    isTimerRunning = false;
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
