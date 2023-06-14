#include <IRremote.h>

IRsend irsend;

void sendRawData() {
  // Define the raw code for your ceiling fan
  unsigned int fanRawCode[] = {
    1301, 362, 1279, 383, 447, 1215, 447, 1215, 469, 1194, 469, 1194, 447, 1194, 469, 1194,
    1301, 362, 469, 1194, 469, 1194, 469, 7957, 1301, 362, 1279, 383, 447, 1215, 469, 1194,
    469, 1194, 447, 1215, 447, 1215, 447, 1194, 1301, 383, 469, 1194, 469, 1194, 469 // Add the remaining codes
  };

  // Calculate the size of the raw code array
  unsigned int fanRawCodeSize = sizeof(fanRawCode) / sizeof(fanRawCode[0]);

  // Send the raw code
  irsend.sendRaw(fanRawCode, fanRawCodeSize, 38); // Replace '38' with the appropriate carrier frequency of your fan IR protocol
}

void setup() {
  Serial.begin(9600);
  // Set up your IR LED pin as an output

  // Other setup code
  // ...
  IrSender.begin(3, ENABLE_LED_FEEDBACK);
}

void loop() {
  // Other code in the loop
  // ...

  // Call the function to send the raw code
  sendRawData();

  // Other code in the loop
  // ...
  delay(3000);
}
