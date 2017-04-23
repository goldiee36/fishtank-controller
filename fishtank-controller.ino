#include <DS3232RTC.h>
#include <TimeLib.h>
#include <Wire.h>

//set dosing ammounts: //decreased all to 2
float carbo = 2; //5 ml easycarbo
float micro = 2; //4 ml profito, 1,5 ml ferro --> decreased to 5
float macro = 2; //3ml DIY PO4, 5 ml DIY NO3+K --> decreased to 5

//set dosing time:
int dosingTime[] = {15,15}; //hour and minute: {0-23, 0-59}

//set light parameters
int minLightPercent = 0;
int maxLightPercent = 90;
int minLightStart[] = {15,20}; //hour and minute: {0-23, 0-59}
int maxLightStart[] = {15,35};
int maxLightEnd[] = {21,30};
int minLightEnd[] = {21,45};

//set correction rates:
int corrRateCarbo = 1400; //ms for 1 ml dose
int corrRateMicro = 1320; //ms for 1 ml dose
int corrRateMacro = 1170; //ms for 1 ml dose

int minLight, maxLight, currentLight, lightDif;
long minLightStartCalc, maxLightStartCalc, maxLightEndCalc, minLightEndCalc, currentTimeCalc;
long rampUpDif, rampDownDif;

void setup(void)
{
    Serial.begin(115200);
    setSyncProvider(RTC.get);
    setSyncInterval(180);
    pinMode(13, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    minLight = (minLightPercent/float(100)*255)+0.5;
    maxLight = (maxLightPercent/float(100)*255)+0.5;
    minLightStartCalc = minLightStart[0]*3600+minLightStart[1]*60;
    maxLightStartCalc = maxLightStart[0]*3600+maxLightStart[1]*60;
    maxLightEndCalc = maxLightEnd[0]*3600+maxLightEnd[1]*60;
    minLightEndCalc = minLightEnd[0]*3600+minLightEnd[1]*60;
    lightDif = maxLight-minLight;
    rampUpDif = maxLightStartCalc-minLightStartCalc;
    rampDownDif = minLightEndCalc-maxLightEndCalc;
    analogWrite(3, 0);
    if (timeStatus() != timeSet)
      blinkLED(3, true); //3 blinks: ERROR
}

void loop(void)
{
  if (timeStatus() == timeNotSet)
    blinkLED(3, true); //3 blinks: ERROR
  if (timeStatus() == timeNeedsSync)
    blinkLED(2, false); //2 blinks: WARNING
  else
    blinkLED(1, false); //1 blink: STATUS OK

  delay(1000);
  if (year() >= 2015 && year() <= 2020 && hour() == dosingTime[0] && minute() == dosingTime[1] && second() >= 1 && second() <= 3) {
    blinkLED(20, false); //20 blinks: dosing is coming!
    digitalWrite(2, HIGH);
    delay(carbo*corrRateCarbo);
    digitalWrite(2, LOW);
    delay(10000);

    digitalWrite(4, HIGH);
    delay(micro*corrRateMicro);
    digitalWrite(4, LOW);
    delay(10000);

    digitalWrite(7, HIGH);
    delay(macro*corrRateMacro);
    digitalWrite(7, LOW);
    delay(10000);
  }
  else {
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
  }
  
  currentTimeCalc = hour()*3600+minute()*60+second();
  if (currentTimeCalc < minLightStartCalc)
    {currentLight = 0;}
  else if (currentTimeCalc >= minLightStartCalc && currentTimeCalc < maxLightStartCalc)
    { currentLight = minLight + lightDif*((currentTimeCalc-minLightStartCalc)/float(rampUpDif));  }
  else if (currentTimeCalc >= maxLightStartCalc && currentTimeCalc < maxLightEndCalc)
    {currentLight = maxLight;}
  else if (currentTimeCalc >= maxLightEndCalc && currentTimeCalc < minLightEndCalc)
    { currentLight = maxLight + 0.5 - lightDif*((currentTimeCalc-maxLightEndCalc)/float(rampDownDif)); }
  else if (currentTimeCalc >= minLightEndCalc)
     {currentLight = 0;}
  analogWrite(3, currentLight);
}

void blinkLED(int errNumber, boolean infinite) {
  do {
    for (int i=1; i<=errNumber; i++) {
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
      delay(200);
    }
    delay(600);
  } while (infinite);
}
