
#include <Adafruit_MAX31855.h>

int tCLK = 2;
int tCS = 3;
int tDO = 4;
int tD1 = 5;
int tD2 = 6;
int tD3 = 7;

Adafruit_MAX31855 thermocouple0(tCLK, tCS, tDO);
Adafruit_MAX31855 thermocouple1(tCLK, tCS, tD1);
Adafruit_MAX31855 thermocouple2(tCLK, tCS, tD2);
Adafruit_MAX31855 thermocouple3(tCLK, tCS, tD3);


void setup(){

  Serial.begin(115200);
  Serial.println("Multiple thermocouple reader");
  delay(500);

}

void loop(){
  
  Serial.print("Internal Temp Chip C0 = ");
  Serial.println(thermocouple0.readInternal());
  Serial.print("Internal Temp Chip C1 = ");
  Serial.println(thermocouple1.readInternal());
  Serial.print("Internal Temp Chip C2 = ");
  Serial.println(thermocouple2.readInternal());
  Serial.print("Internal Temp Chip C3 = ");
  Serial.println(thermocouple3.readInternal());
  
  double c0 = thermocouple0.readCelsius();
  double c1 = thermocouple1.readCelsius();
  double c2 = thermocouple2.readCelsius();
  double c3 = thermocouple3.readCelsius();
   
  Serial.print("C0 = "); 
  Serial.println(c0);
  Serial.print("C1 = "); 
  Serial.println(c1);
  Serial.print("C2 = "); 
  Serial.println(c2);
  Serial.print("C3 = "); 
  Serial.println(c3);
   
  delay(1000);

}




