#include <Wire.h>
#include <MC3630.h>

MC3630 MC3630_acc = MC3630();

void setup() 
{
  	Serial.begin(9600);
	  MC3630_acc.start(); 
  	Serial.println("mCube Accelerometer MC3630:");
  	checkRange();
  	checkResolution();
  	checkSamplingRate();
  	Serial.println();
}

void checkRange()
{
	switch(MC3630_acc.GetRangeCtrl())
	{
		case MC3630_RANGE_16G:            Serial.println("Range: +/- 16 g"); break;     
		case MC3630_RANGE_12G:            Serial.println("Range: +/- 12 g"); break;         
		case MC3630_RANGE_8G:             Serial.println("Range: +/- 8 g"); break;
		case MC3630_RANGE_4G:             Serial.println("Range: +/- 4 g"); break;
		case MC3630_RANGE_2G:             Serial.println("Range: +/- 2 g"); break;
		default:                          Serial.println("Range: +/- 8 g"); break;
	}   
}  

void checkResolution()
{
	switch(MC3630_acc.GetResolutionCtrl())
	{
		case MC3630_RESOLUTION_6BIT:            Serial.println("Resolution: 6bit"); break;     
		case MC3630_RESOLUTION_7BIT:            Serial.println("Resolution: 7bit"); break;         
		case MC3630_RESOLUTION_8BIT:            Serial.println("Resolution: 8bit"); break;
		case MC3630_RESOLUTION_10BIT:           Serial.println("Resolution: 10bit"); break;
		case MC3630_RESOLUTION_14BIT:           Serial.println("Resolution: 14bit"); break;
		case MC3630_RESOLUTION_12BIT:           Serial.println("Resolution: 12bit"); break;
		default:                                Serial.println("Resolution: 14bit"); break;
	} 
}

void checkSamplingRate()
{
	switch(MC3630_acc.GetCWakeSampleRate())
	{
		MC3630_CWAKE_SR_DEFAULT_50Hz:		Serial.println("Output Sampling Rate: 50 Hz"); break;
		MC3630_CWAKE_SR_0p4Hz:                  Serial.println("Output Sampling Rate: 0.4 Hz"); break;
		MC3630_CWAKE_SR_0p8Hz:                  Serial.println("Output Sampling Rate: 0.8 Hz"); break; 
		MC3630_CWAKE_SR_2Hz:                    Serial.println("Output Sampling Rate: 2 Hz"); break; 
		MC3630_CWAKE_SR_6Hz:                    Serial.println("Output Sampling Rate: 6 Hz"); break; 
		MC3630_CWAKE_SR_13Hz:                   Serial.println("Output Sampling Rate: 13 Hz"); break; 
		MC3630_CWAKE_SR_25Hz:        		Serial.println("Output Sampling Rate: 25 Hz"); break;	
		MC3630_CWAKE_SR_50Hz:                   Serial.println("Output Sampling Rate: 50 Hz"); break;
		MC3630_CWAKE_SR_100Hz:                  Serial.println("Output Sampling Rate: 100 Hz"); break; 
		MC3630_CWAKE_SR_200Hz:                  Serial.println("Output Sampling Rate: 200 Hz"); break; 
		default:                                Serial.println("Output Sampling Rate: 50 Hz"); break;
	}   
}  

void loop()
{
	// Read the raw sensor data count
	MC3630_acc_t rawAccel = MC3630_acc.readRawAccel();
	delay(10);
	Serial.print("X:\t"); Serial.print(rawAccel.XAxis); Serial.print("\t");
	Serial.print("Y:\t"); Serial.print(rawAccel.YAxis); Serial.print("\t");
	Serial.print("Z:\t"); Serial.print(rawAccel.ZAxis); Serial.print("\t");
	Serial.println("counts");
	
	// Display the results (acceleration is measured in m/s^2)
	Serial.print("X: \t"); Serial.print(rawAccel.XAxis_g); Serial.print("\t");
	Serial.print("Y: \t"); Serial.print(rawAccel.YAxis_g); Serial.print("\t");
	Serial.print("Z: \t"); Serial.print(rawAccel.ZAxis_g); Serial.print("\t");
	Serial.println("m/s^2");

  Serial.println("--------------------------------------------------------------");
  delay(1000);
}
