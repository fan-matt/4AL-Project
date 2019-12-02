#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>


const byte ADDRESS[6] = "94523";      //Any address that has to match with the reciever
const int CHANNEL = 95;
const long BAUD = 115200;

const int MPU_ADDR = 0x68;            // I2C address of the MPU-6050

const double X_OFFSET = -160;  
const double Y_OFFSET = -340;
const double SCALE = 9.8 / 16384.0;


double xAccel;
double yAccel;
double xPos = 0;
double yPos = 0;
double loopDuration = 0;

double data[2];


RF24 radio(7,8); // Corresponds to the pins that connect to the wifi chip


void setup()
{
  Serial.begin(BAUD);
  radio.begin();
  radio.setChannel(CHANNEL);
  radio.openWritingPipe(ADDRESS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);

  radio.stopListening();

  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the accelerometer)
  Wire.endTransmission(true);
}


void loop() 
{  
  long startTime = millis();
  
  //Transmitter Code
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,8,true);  // request a total of 14 registers
  xAccel = ((Wire.read() << 8 | Wire.read()) - X_OFFSET) * SCALE;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  yAccel = ((Wire.read() << 8 | Wire.read()) - Y_OFFSET) * SCALE;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  

  // Integrate
  xPos += xAccel * loopDuration * loopDuration;
  yPos += yAccel * loopDuration * loopDuration;

  xPos *= -1;   // Just to get the sign right

  data[0] = xAccel;
  data[1] = yAccel;

  Serial.print(data[0]);
  Serial.print(" , ");   
  Serial.println(data[1]);

  radio.write(&data, sizeof(data));    

  delayMicroseconds(10);

  loopDuration = (millis() - startTime) / 1000;   // Convert to seconds
}
