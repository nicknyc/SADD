#include "Volume3.h"
#include <Wire.h>
#include <MPU6050.h>

#define speakerLPin 9
#define speakerRPin 10
#define VOL_MAX 255

MPU6050 mpu;
float x,y,z;
int v;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  
  checkSettings();

  for(int i = 0; i < 5; i++){
    Vector normAccel = mpu.readRawAccel();
    x += normAccel.XAxis;
    y += normAccel.YAxis;
    z += normAccel.ZAxis;
  }

  x /= 5;
  y /= 5;
  z /= 5;

  v = 90;
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
}

void loop() {
  /*
  for(int i = 0; i < 256; i++){
    vol.tone(speakerLPin, 1050, i);
    delayMicroseconds(20*64);
    vol.tone(speakerRPin, 1050, VOL_MAX - i);
    delayMicroseconds(40*64);
  }*/
  float x2 = 0;
  float y2 = 0;
  float z2 = 0;
  for(int i = 0; i < 5; i++){
    Vector normAccel = mpu.readRawAccel();
    x2 += normAccel.XAxis;
    y2 += normAccel.YAxis;
    z2 += normAccel.ZAxis;
  }

  x2 /= 5;
  y2 /= 5;
  z2 /= 5;
  
  float delX = x2 - x;
  float delY = y2 - y;
  float delZ = z2 - z;

  Serial.print(delX);
  Serial.print(" ");
  //Serial.print(delY);
  Serial.print(" ");
  //Serial.println(delZ);

  int f = 500;//map(delZ, -600, 600, 100, 2000);
  int delV = map(delX, -600, 600, -10, 10);

  Serial.print(delV);
  Serial.print(" ");
  v -= delV;
  Serial.println(v);
  if(v <= 0) v = 0;
  else if(v >= 150) v = 150;
  
    vol.tone(LPin, f, 100+v);
    vol.tone(RPin, f, 150-v);
  x = x2;
  y = y2;
  z = z2;
}
