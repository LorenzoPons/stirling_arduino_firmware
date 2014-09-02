
#include <Adafruit_MAX31855.h>

// I/O definitions
#define T_CLK 2
#define T_CS 3
#define T_DO 4
#define T_D1 5
#define T_D2 6
#define T_D3 7
#define VOLTAGE_ANALOG 0
#define CURRENT_ANALOG 1
#define PRESSURE_ANALOG 2

#define PRECISION 2
#define SEPARATOR ";"

// Function prototypes
void read4Temps(int[], int, double[], int);

// Global variables
int dPins[4] = {T_DO, T_D1, T_D2, T_D3};
int analogInputs[3] = {VOLTAGE_ANALOG, CURRENT_ANALOG, PRESSURE_ANALOG};
double tCel[4] = {0.0, 0.0, 0.0, 0.0};
int analogReads[4] = {0, 0, 0, 0};

void setup(){

    Serial.begin(115200);
    delay(500);

}

void loop(){
  
    // Reading sensors
    readNTemps(dPins, (sizeof(dPins)/sizeof(dPins[0])), tCel, (sizeof(tCel)/sizeof(tCel[0])));
    
    readNAnalogs(analogInputs, (sizeof(analogInputs)/sizeof(analogInputs[0])), analogReads);
    
    String measurements = String("t0;t1;t2;t3;v;i;p;");
    
    for (int i = 0; i < 6; i++)
    {
        String temp = doubleToString(tCel[i], PRECISION);
        String tokenTemp = "t" + String(i);
        measurements.replace(tokenTemp, temp);
    }
    
    Serial.print(measurements + "\n");
    
    /*
    // Sending to serial port
    for (int i = 0; i < 4; i++)
    {
        Serial.print(tCel[i], PRECISION);
        Serial.print(SEPARATOR);
    }
    for (int i = 0; i < 3; i++)
    {
        Serial.print(analogReads[i]);
        Serial.print(SEPARATOR);
    }
    Serial.println();
    
    */
    
    delay(100);

}

void readNTemps(int pins[], int pLen, double temps[], int tLen)
{
    // Cyclic thermocouples reader
    for (int i = 0; (i < pLen) && (i < tLen); i++)
    {
        Adafruit_MAX31855 thermocouple(T_CLK, T_CS, dPins[i]);
        temps[i] = thermocouple.readCelsius();
    }
}

void readNAnalogs(int analogIns[], int inLen, int analogReads[])
{
    for (int i = 0; i < inLen; i++)
        {
            analogReads[i] = analogRead(analogIns[i]);
        }
}

String doubleToString(double number, uint8_t digits) 
{ 
  String str = String("");
  
  if (isnan(number)) return String("nan");
  if (isinf(number)) return String("inf");
  if (number > 4294967040.0) return String("ovf");  // constant determined empirically
  if (number <-4294967040.0) return String("ovf");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
     str += "-";
     number = -number;
  }
  else
  {
     str += "+";
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  str += int_part;

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    str += "."; 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    str += toPrint;
    remainder -= toPrint; 
  } 
  
  return str;
}




