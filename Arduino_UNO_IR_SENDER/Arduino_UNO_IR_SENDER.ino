#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>

#define RAW_DATA_LEN 24
uint16_t rawData[RAW_DATA_LEN]={

1246, 442, 1246, 438, 402, 1286, 402, 1286, 
378, 1310, 398, 1286, 1246, 442, 378, 1310, 
402, 1286, 402, 1282, 402, 1286, 402, 1000};

IRsendRaw mySender;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Seding on");
  mySender.send(rawData, RAW_DATA_LEN,40);
  delay(5000);
  Serial.println("Seding off");
  mySender.send(rawData, RAW_DATA_LEN,40);
  delay(5000);
}
