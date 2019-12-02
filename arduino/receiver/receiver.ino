/*
* Arduino Wireless Communication Reciever Accelerometer Data
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte ADDRESS[6] = "94523";      //Any address that has to match with the reciever
const int CHANNEL = 95;
const long BAUD = 115200;

double data[2];


void setup() {
  Serial.begin(BAUD);
  radio.begin();
  radio.setChannel(CHANNEL);
  radio.openReadingPipe(0, ADDRESS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.startListening();
  Serial.println("starting");
}

 
void loop() {
  if(radio.available(0)) 
  {
    radio.read(&data , sizeof(data));
    Serial.print(data[0]);
    Serial.print(","); 
    Serial.print(data[1]);
    Serial.println();
  }
}
