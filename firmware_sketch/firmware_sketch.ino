
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




