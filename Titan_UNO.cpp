#include "Titan_UNO.h"
#include <Arduino.h>
#include <SoftwareSerial.h>




void Titan_UNO::init()
{
	Serial.println("Titan_UNO Init");

}



bool Titan_UNO::checkAscii(uint8_t thisChar)
{
	if (isHexadecimalDigit(thisChar)) 
	{

		return true;
	}
	if (thisChar == '\r')
	{

		return true;
	}
	if (thisChar == '\n')
	{

		return true;
	}
	return false;
}

void Titan_UNO::PrintHex(uint8_t data) // prints 8-bit data in hex with leading zeroes
{
    Serial.print("0x"); 
       
    if (data < 0x10) 
	{
		Serial.print("0");
	} 
    Serial.print(data,HEX); 
    Serial.print(" "); 
}



void Titan_UNO::printHeaderCmd(uint8_t cmdBytes)
{
	
	switch(cmdBytes)
	{
		//PEDOMETER Response
		case  PEDOMETER_CONTROL:  
			Serial.println("PEDOMETER_CONTROL"); 
			break;
		case  PEDOMETER_CONTROL_RESPONSE:  
			Serial.println("PEDOMETER_CONTROL_RESPONSE"); 
			break;
		case  PEDOMETER_RECORD_GET:  
			Serial.println("PEDOMETER_RECORD_GET"); 
			break;
		case  PEDOMETER_STEP_GET:  
			Serial.println("PEDOMETER_STEP_GET"); 
			break;
		case  PEDOMETER_NOTIFICATION_ENABLE:  
			Serial.println("PEDOMETER_NOTIFICATION_ENABLE"); 
			break;		
		case  PEDOMETER_RECORD_GET_RESPONSE:
			Serial.println("PEDOMETER_RECORD_GET_RESPONSE"); 
			break;
		case  PEDOMETER_STEP_GET_RESPONSE:  
			Serial.println("PEDOMETER_STEP_GET_RESPONSE"); 
			break;
		case  PEDOMETER_NOTIFICATION_ENABLE_RESPONSE:  
			Serial.println("PEDOMETER_NOTIFICATION_ENABLE_RESPONSE"); 
			break;
		case  PEDOMETER_NOTIFICATION:  
			Serial.println("PEDOMETER_NOTIFICATION"); 
			break;
			
		//SLEEP MONITOR Response
		case  SLEEP_MONITOR_CONTROL_RESPONSE:  
			Serial.println("SLEEP_MONITOR_CONTROL_RESPONSE"); 
			break;
		case  SLEEP_MONITOR_SENSITIVITY_SET_RESPONSE:  
			Serial.println("SLEEP_MONITOR_SENSITIVITY_SET_RESPONSE"); 
			break;
		case  SLEEP_MONITOR_NOTIFICATION_ENABLE_RESPONSE:  
			Serial.println("SLEEPMONITOR_NOTIFICATION_ENABLE_RESPONSE"); 
			break;	
		case  SLEEP_MONITOR_EVENT_NOTIFICATION:  
			Serial.println("SLEEP_MONITOR_EVENT_NOTIFICATION"); 
			break;
		case  SLEEP_MONITOR_CONTROL:  
			Serial.println("SLEEP_MONITOR_CONTROL"); 
			break;
		case  SLEEP_MONITOR_NOTIFICATION_ENABLE:  
			Serial.println("SLEEP_MONITOR_NOTIFICATION_ENABLE"); 
			break;		
			
		//Sensor Control	
		case  SENSOR_CONTROL:
			Serial.println("BLE_CONTROL"); 
			break;
		case  SENSOR_CONTROL_RESPONSE:  
			Serial.println("SENSOR_CONTROL_RESPONSE"); 
			break;	
		
		//Reporter control
		case  REPORTER_CONTROL:  
			Serial.println("REPORTER_CONTROL"); 
			break;	
		case  REPORTER_CONTROL_RESPONSE:  
			Serial.println("REPORTER_CONTROL_RESPONSE"); 
			break;

		default:
		 	Serial.print("Header Cmd=");
		 	Serial.println(cmdBytes,HEX);
		 	Serial.println();
			break;
	}
}

void Titan_UNO::commandTRANS(uint8_t bCmdHeader, uint8_t* baCmdBytes, uint8_t len, int RX, int TX)
{
		
	SoftwareSerial mySerialTRANS(RX ,TX);	
	mySerialTRANS.begin(9600);
		
 	if(baCmdBytes == NULL)
	{
		Serial.println("Null Command Bytes sent to BT");
		return;
	}
	if(bCmdHeader < 16)
	{
		Serial.println("Wrong Command Header sent to BT");
		return;
	}

    mySerialTRANS.write(baCmdBytes, len);
	delayMicroseconds(100);

  }