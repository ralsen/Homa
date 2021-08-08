/*
  ToF.ino

  ToF -> time of Flight Sensor (VL53L1X from ST-Microelectronics)

  !!! for history see end of file !!!

  ToDos:    ???

  hints:    ???
*/


#if (H_TOF == H_TRUE)
#include "ToF.h"
//#include "SparkFun_VL53L1X.h"

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

VL53L1X sensor;

//SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

int ToFPresent;

int SetupToF(){
 DBGLN("SetupToF()");

 Wire.begin();
 Wire.setClock(400000); // use 400 kHz I2C

 sensor.setTimeout(500);

 if (!sensor.init())
 {
   Serial.println("Sensor failed to initialisation. Please check wiring ...");
   ToFPresent = TOF_FALSE;
   //while (1);
   return ToFPresent;
 }

 // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
// You can change these settings to adjust the performance of the sensor, but
// the minimum timing budget is 20 ms for short distance mode and 33 ms for
// medium and long distance modes. See the VL53L1X datasheet for more
// information on range and timing limits.
sensor.setDistanceMode(VL53L1X::Long);
sensor.setMeasurementTimingBudget(50000);

// Start continuous readings at a rate of one measurement every 50 ms (the
// inter-measurement period). This period should be at least as long as the
// timing budget.
sensor.startContinuous(50);
Serial.println("ToF-Sensor detected!");
ToFPresent = TOF_TRUE;
return ToFPresent;
}

int ToFRange;
int ToFarr[TOF_AVR_CNT];
int ToFAvr;
int i=0;
int j=0;

void ToFDistance(void){
 //DBGF("!!! I S R !!! ToFDistance()");
 int k;
 // measurement every 50ms
 if(j<TOF_INTERVAL){
   j++;
   return;
 }
 j = 0;

 if(ToFPresent == TOF_FALSE)
  SetupToF();

 if(i<TOF_AVR_CNT){
   ToFarr[i++] = sensor.read();
 }
 else{
   i = 0;
   ToFAvr = 0;
   for (k=0; k<TOF_AVR_CNT; k++){
     ToFAvr += ToFarr[k];
   }
   ToFAvr /= TOF_AVR_CNT;
   Serial.print(ToFAvr);
   Serial.println(" mm");
 }

 if (sensor.timeoutOccurred()) {
   Serial.print(" TIMEOUT\r\n");
   ToFPresent = TOF_FALSE;
 }
}

#endif //(H_TOF == H_TRUE)
/*

  History:
  --------------------- V1.5j
  27.02.21  average calculation for the ToF-Sensor

  13.01.21  first initial version

  todos:
  ???
 */
