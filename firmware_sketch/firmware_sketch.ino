
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
#define V_REF 2.49856
#define V_RES 0.00488
#define R_RATIO 0.025
#define SEPARATOR ";"

// Function prototypes
void read4Temps(int[], int, double[], int);
void readNAnalogs(int[], int, int[]);
String toStrVolts(int);
String toStrAmpere(int);
String toStrBar(int);

String doubleToString(double, uint8_t);

// Global variables
int dPins[4] = {T_DO, T_D1, T_D2, T_D3};
int analogInputs[3] = {VOLTAGE_ANALOG, CURRENT_ANALOG, PRESSURE_ANALOG};
double tCel[4] = {0.0, 0.0, 0.0, 0.0};
int analogReads[3] = {0, 0, 0};

void setup(){

    Serial.begin(115200);
    delay(500);
}

void loop(){
  
    // Reading sensors
    readNTemps(dPins, (sizeof(dPins)/sizeof(dPins[0])), tCel, (sizeof(tCel)/sizeof(tCel[0])));
    
    readNAnalogs(analogInputs, (sizeof(analogInputs)/sizeof(analogInputs[0])), analogReads);
    
    // Sending measurements to serial port
    String measurements = String("t0;t1;t2;t3;v;i;p;");
    
    // Tokens replacement loop
    for (int i = 0; i < 7; i++)
    {
        if (i < 4)
        {
            String temp = doubleToString(tCel[i], PRECISION);
            String tokenTemp = "t" + String(i);
            measurements.replace(tokenTemp, temp);
        }
        
        if ((i > 3) && (i < 5))
        {
            String voltage = toStrVolts(analogReads[i - 4]);
            String tokenVolt = "v";
            measurements.replace(tokenVolt, voltage);
        }
        if ((i > 4) && (i < 6))
        {
            String current = toStrAmpere(analogReads[i - 4]);
            String tokenCurrent = "i";
            measurements.replace(tokenCurrent, current);
        }
        if (i > 5)
        {
            String pressure = toStrBar(analogReads[i - 4]);
            String tokenPressure = "p";
            measurements.replace(tokenPressure, pressure);
        }
    }
   
    Serial.print(measurements + "\n");
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

String toStrVolts(int adcOut)
{
    double dVoltage = (double)adcOut * V_RES * (1/R_RATIO);
    String strVoltage = doubleToString(dVoltage, PRECISION);
    return strVoltage;
}

String toStrAmpere(int adcOut)
{
    double vOffset = V_REF; // @ 0A +- 0.030V
    double vOutRaw = (adcOut * V_RES) - V_REF;
    double iOut = vOutRaw/0.015;
    String  strI = doubleToString(iOut, PRECISION);
    return strI;
}

String toStrBar(int adcOut)
{
    double pRaw = (adcOut - 100)*V_RES;
    double pOut = (172.3689 * pRaw) / 4.02;
    String strP = doubleToString(pOut, PRECISION);  // dummy output
    return strP;
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




