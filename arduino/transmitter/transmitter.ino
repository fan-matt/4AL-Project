/*
* Arduino Wireless Communication Reciever Accelerometer Data
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

const int red_pin = 5; // Defines which pin corresponds to red
const int green_pin = 6; //Defines which pin corresponds to green

double x_in = 0;
  double y_in = 0;

  int acc_index = 0;

  double x_acc[37];
  double y_acc[37];

  bool turnLightGreen = false;

 
  double calc_x = 0.0;
  double calc_y = 0.0;

  double xvi = 0.0;
  double yvi = 0.0;

double simpsonsRule(double y[],double dt);

int redVal=0;
int greenVal=0;

//The following is the TRANSMITTER data
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

RF24 radio(7,8); // Corresponds to the pins that connect to the wifi chip
const byte address[6] = "94523"; // Our address

const int MPU_addr=0x68;  // I2C address of the accelerometer
long elapsedTime;

float AcX, AcY, AcZ;

// This is our calibration from lab 5
float Xoffset=-500;  
float Yoffset=250;
float Zoffset=-500;

// DO NOT TOUCH THIS SECTION
float Xscale=16384;
float Yscale=16384;
float Zscale=18384;

float sound_speed= 343000; //measured in mm/s
float g=980; //acceleration due to gravity measured in cm/s^2


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
radio.begin();
radio.setChannel(95); // Our channel
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MAX);
radio.setDataRate(RF24_2MBPS);

radio.stopListening();

Wire.begin(); //Configures the accelerometer
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the accelerometer)
  Wire.endTransmission(true);

  //Specifies that the LEDs are output
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);

}


void loop() {
  
// Basic code for the RGB
if (turnLightGreen) {
  digitalWrite(red_pin, LOW);
  
  digitalWrite(green_pin, HIGH);}
else{
  digitalWrite(green_pin, LOW);
  digitalWrite(red_pin, HIGH);}

//Transmitter Code
 Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,8,true);  // request a total of 14 registers
  AcX=float((Wire.read()<<8|Wire.read())-Xoffset)*g/Xscale;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=float((Wire.read()<<8|Wire.read())-Yoffset)*g/Yscale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  //AcZ=float((Wire.read()<<8|Wire.read())-Zoffset)*g/Zscale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  elapsedTime = millis();

  if (abs(AcX) < 25)
    AcX = 0;
  if (abs(AcY) < 25)
    AcY = 0;
  
  long dataArray[5] = {elapsedTime, AcX, AcY,calc_x,calc_y} ; //I took out 'duration' here because it was part of the ultrasonic sensor

  for(int i = 0; i < (sizeof(dataArray) / sizeof(dataArray[0])-1); i++)
  {
    Serial.print(dataArray[i]);
    Serial.print(",");  
   }
   Serial.print(dataArray[4]);
   Serial.println();
   
  radio.write(&dataArray, sizeof(dataArray));  



  x_acc[acc_index] = AcX;
  y_acc[acc_index] = AcY;


   if (acc_index==36){


    double x_vel[7];
    double y_vel[7];

    x_vel[0] = xvi;
      y_vel[0] = yvi;
      
    int a;
    int b;
    //Calculate the velocties based on the accelerations
    for (a = 0; a < 6; a++)
    {
      double x_acc_small[7];
      double y_acc_small[7];

      
      
      for (b = 0; b < 7; b++)
      {
        x_acc_small[b] = x_acc[6*a + b];
        y_acc_small[b] = y_acc[6*a + b];
      }
      x_vel[a+1] = simpsonsRule(x_acc_small,.001) + xvi;
      y_vel[a+1] = simpsonsRule(y_acc_small,.001) +yvi;

      //Change the initial velocities
    xvi = x_vel[a+1];
    yvi = y_vel[a+1];
    }

    //Calc the position for these values
    calc_x = simpsonsRule(x_vel,.006) + calc_x;
    calc_y = simpsonsRule(y_vel,.006) + calc_y;

    
 
//Decide which color the light should be  
if (!turnLightGreen && abs(calc_x - x_in) < 1 && abs(calc_y - y_in) < 1 ){
  turnLightGreen = true;
}

else if (turnLightGreen &&  calc_x != x_in || calc_y != y_in){
  turnLightGreen = false;
}

  acc_index = 0;
    }
    else {
      acc_index++;
    }
}

     double simpsonsRule(double y[7], double dt){
  //Difference between time readings

  //Integration Approximation
  return (dt*30.5 / 3 * (y[0] + 4*y[1] + 2*y[2] + 4*y[3] + 2*y[4] + 4*y[5] + y[6]));
   }
