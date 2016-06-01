#include <Wire.h>
#include <MC3630.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerialTRANS(10,11); //(RX,TX)

uint8_t alarmON[] = {0xA4,0x01,0x00,0x00,0x0d,0x0a};
uint8_t alarmOFF[] = {0xA4,0x00,0x00,0x00,0x0d,0x0a};

MC3630 MC3630_acc = MC3630();

int i;
int Position;
int Position_Before;
int Alarm_Check_Flag;
int photocellPin = 2; // photocell connect on anallog pin 2
int photocellVal = 0; // photocell variable
int minLight = 200;   // minimum photocell variable
int ledPin = 9;
int ledState = 0;


void Numbering()
{
	MC3630_acc_t rawAccel = MC3630_acc.readRawAccel();
	if(rawAccel.XAxis_g > 9){
	Position = 1;
	}
	if(rawAccel.XAxis_g < -9){
	Position = 2;
	}
	if(rawAccel.YAxis_g > 9){
	Position = 3;
	}
	if(rawAccel.YAxis_g < -9){
	Position = 4;
	}
	if(rawAccel.ZAxis_g > 9){
	Position = 5;
	}
	if(rawAccel.ZAxis_g < -9){
	Position = 6;
	}   
}

void PositionCheck()
{
	switch(Position){
	case 1:
	if(Position_Before != Position){
		Position_Before = 1;
		i = i+1;
	}
	break;
	case 2:
	if(Position_Before != Position){
		Position_Before = 2;
		i = i+1;
	}
	break;
	case 3:
	if(Position_Before != Position){
		Position_Before = 3;
		i = i+1;
	}
	break;
	case 4:
	if(Position_Before != Position){
		Position_Before = 4;
		i = i+1;
	}
	break;
	case 5:
	if(Position_Before != Position){
		Position_Before = 5;
		i = i+1;
	}
	break;
	case 6:
	if(Position_Before != Position){
		Position_Before = 6;
		i = i+1;
	}
	break;
	}
}

void MorningAlarm()
{
	Serial.println("Alarm - MorningAlarm");
	mySerialTRANS.write(alarmON,6);
	delay(500);
	mySerialTRANS.write(alarmON,6);
	delay(500);
	mySerialTRANS.write(alarmON,6);
	Alarm_Check_Flag = 1;    
	digitalWrite(ledPin, HIGH);
}

void TurnOffAlarm()
{
	Serial.println("--------------------------------------------------------------");
	Serial.println("Correct");
	Serial.println("The alarm is closed !");
	Serial.println("");
	mySerialTRANS.write(alarmOFF,6);
	Alarm_Check_Flag = 0;
	digitalWrite(ledPin, LOW);
	Position_Before = 0;
	i = 0;
}

void digitalClockDisplay()
{
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.println(); 
}

void printDigits(int digits)
{
	Serial.print(":");
	if(digits < 10)
	Serial.print('0');
	Serial.print(digits);
}

void setup() 
{
	Serial.begin(9600);
	mySerialTRANS.begin(9600);
	setTime(8,0,0,5,16,16);                       //Set the real time here, it won't follow your computer's time. (Saturday 8:00:00am May 16 2016)
	Alarm.alarmRepeat(8,0,5, MorningAlarm);       //Set your alarm here. (8:00:05am)
	pinMode(ledPin, OUTPUT);
	MC3630_acc.start(); 
	Serial.println("mCube Alarm by MC3630");
	Serial.println();
	Position_Before = 0;
	i = 0;
	delay(1000);
}

void loop()
{
	digitalClockDisplay();
	Alarm.delay(1000);

	if(Alarm_Check_Flag == 1){

		photocellVal = analogRead(photocellPin);

		if(photocellVal > minLight){
			Numbering();
			PositionCheck();
			switch(i){
			case 1:
			Serial.println("Please roll the cube !");
			break;
			case 2:
			Serial.println("Roll it again !");
			break;
			case 3:
			TurnOffAlarm();
			break;
			}
		}
	}
}
