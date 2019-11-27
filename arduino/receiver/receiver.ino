/*
* Arduino Wireless Communication Reciever Accelerometer Data
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

long data[2];

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

int i = 0;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(70);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();
  Serial.println("starting");
}
 
void loop() {
//  if (radio.available(0)) {
//    radio.read(&data, sizeof(data));
//    Serial.print(data[0]);
//    Serial.print(',');
//    Serial.print(data[1]);
//    Serial.println();
//  }
//  else 
//  {
//    Serial.println("Hello!");
//  }

  Serial.println(i);
  i++;
}
