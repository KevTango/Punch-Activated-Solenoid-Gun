// Punch activated water gun

// Code is adapted from Allan Pan's project https://www.hackster.io/Advanced/punch-activated-arm-flamethrowers-real-firebending-95bb80
// This program measures the acceleration from the punch and squirts water if the punch acceleration exceeds a certain acceleration. 
// Created by: Kevin T. and Ryan L.
// Date: 25/06/2018

// Include libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// Software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

// Measuring punches

//Long = 32 bits
unsigned long punchStart = 0; // Variable for non-blocking punch time frame check
const long punchInterval = 200; // Time frame of a punch in ms, from max acceleration to max deceleration
int punchAccel = 20; // The beginning of a punch in m/s^2
int punchDecel = -40; // The end of a punch in m/s^2
int waterTime = 500; // How long the water squirts for in ms

// Testing accelerometer
void setup(void) {
// Initialise serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) { // change this to 0x19 for alternative i2c address
    Serial.println("Couldn't start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_16_G); // http://www.sensorwiki.org/doku.php/sensors/accelerometer (In case the acceleration goes over 8G) 
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");
  
  pinMode(8, OUTPUT); //Solenoid valve
  pinMode(9, OUTPUT); //_____
  digitalWrite(8, LOW); // Setting solenoid valve to close
  digitalWrite(9, LOW); // Setting ____ to close
}

void loop() {
  lis.read();
  sensors_event_t event; 
  lis.getEvent(&event);
  
 // Look for punch starting at least 20 m/s^2
  if (event.acceleration.x > punchAccel){
     Serial.println(event.acceleration.x);
     punchStart = millis();
  }
  
  unsigned long currentMillis = millis();

  //Look for punch ending less than -40 m/s^2
  if (event.acceleration.x < punchDecel && currentMillis - punchStart < punchInterval){
      Serial.println(event.acceleration.x);
      Serial.println("Punch");
      Water(waterTime);
    }
}

// When all the criteria are met, then the solenoid and water valve will open for a brief period. 
void Water(int waterTime){
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(waterTime);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

