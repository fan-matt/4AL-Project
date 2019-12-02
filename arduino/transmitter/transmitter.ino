/*
* Arduino Wireless Communication Reciever Accelerometer Data
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>


const byte ADDRESS[6] = "94523";      //Any address that has to match with the reciever
const int CHANNEL = 95;
const long BAUD = 115200;

const int MPU_ADDR = 0x68;            // I2C address of the MPU-6050

const int red_pin = 5; // Defines which pin corresponds to red
const int green_pin = 6; //Defines which pin corresponds to green

int redVal=0;
int greenVal=0;
bool turnLightGreen = false;

double x_in = 0;
double y_in = 0;

int acc_index = 0;

double x_acc[37];
double y_acc[37];

double xvi = 0.0;
double yvi = 0.0;

double calc_x = 0.0;
double calc_y = 0.0;

RF24 radio(7,8); // Corresponds to the pins that connect to the wifi chip

long elapsedTime;

double AcX, AcY, AcZ;

// This is our calibration from lab 5
double Xoffset = -160;  
double Yoffset = -340;

// DO NOT TOUCH THIS SECTION
double Xscale=16384.0;
double Yscale=16384.0;

const double g = 980; //acceleration due to gravity measured in cm/s^2


double simpsonsRule(double y[],double dt);


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

  //Specifies that the LEDs are output
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);
}


void loop() 
{  
  // Basic code for the RGB
  if (turnLightGreen) 
  {
    digitalWrite(red_pin, LOW);
    digitalWrite(green_pin, HIGH);
  }
  else
  {
    digitalWrite(green_pin, LOW);
    digitalWrite(red_pin, HIGH);
  }

  //Transmitter Code
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR,8,true);  // request a total of 14 registers
  AcX=double((Wire.read() << 8 | Wire.read()) - Xoffset) * g / Xscale;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=double((Wire.read() << 8 | Wire.read()) - Yoffset) * g / Yscale;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  
  elapsedTime = millis();

  if(abs(AcX) < 25)
  {
    AcX = 0;
  }
  if(abs(AcY) < 25)
  {
    AcY = 0;
  }
  
  double dataArray[2] = {calc_x,calc_y};

  Serial.print(dataArray[0]);
  Serial.print(" , ");   
  Serial.println(dataArray[1]);

  radio.write(&dataArray, sizeof(dataArray));    

  x_acc[acc_index] = AcX;
  y_acc[acc_index] = AcY;

  if(acc_index == 36)
  {
    double x_vel[7];
    double y_vel[7];

    x_vel[0] = xvi;
    y_vel[0] = yvi;
      
    //Calculate the velocties based on the accelerations
    for(int a = 0; a < 6; a++)
    {
      double x_acc_small[7];
      double y_acc_small[7];

      for(int b = 0; b < 7; b++)
      {
        x_acc_small[b] = x_acc[6 * a + b];
        y_acc_small[b] = y_acc[6 * a + b];
      }

      x_vel[a + 1] = simpsonsRule(x_acc_small , .001) + xvi;
      y_vel[a + 1] = simpsonsRule(y_acc_small , .001) + yvi;

      //Change the initial velocities
      xvi = x_vel[a + 1];
      yvi = y_vel[a + 1];
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
  else 
  {
    acc_index++;
  }
}


double simpsonsRule(double y[7], double dt)
{
  //Integration Approximation
  return (dt*30.5 / 3 * (y[0] + 4*y[1] + 2*y[2] + 4*y[3] + 2*y[4] + 4*y[5] + y[6]));
}
