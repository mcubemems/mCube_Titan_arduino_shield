
#include <Arduino.h>
#include <Wire.h>
#include "MC3630.h"

#define MC3630_CFG_I2C_ADDR    			(0x4C)
#define MC3630_CFG_MODE_DEFAULT    		MC3630_MODE_STANDBY
#define MC3630_CFG_SAMPLE_RATE_CWAKE_DEFAULT    MC3630_CWAKE_SR_DEFAULT_50Hz
#define MC3630_CFG_SAMPLE_RATE_SNIFF_DEFAULT    MC3630_SNIFF_SR_0p4Hz
#define MC3630_CFG_RANGE_DEFAULT         	MC3630_RANGE_8G
#define MC3630_CFG_RESOLUTION_DEFAULT    	MC3630_RESOLUTION_14BIT
#define MC3630_CFG_ORIENTATION_MAP_DEFAULT    	ORIENTATION_TOP_RIGHT_UP

uint8_t CfgRange, CfgResolution;	

 // Read register bit
bool MC3630::readRegisterBit(uint8_t reg, uint8_t pos)
{
    uint8_t value;
    value = readRegister8(reg);
    return ((value >> pos) & 1);
}

// Write register bit
void MC3630::writeRegisterBit(uint8_t reg, uint8_t pos, bool state)
{
    uint8_t value;
    value = readRegister8(reg);

    if (state)
        value |= (1 << pos);
    else
        value &= ~(1 << pos);

    writeRegister8(reg, value);
}

// Read 8-bit from register
uint8_t MC3630::readRegister8(uint8_t reg)
{
    uint8_t value;
    Wire.beginTransmission(MC3630_CFG_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false); //endTransmission but keep the connection active
    Wire.requestFrom(MC3630_CFG_I2C_ADDR, 1); //Once done, bus is released by default
    value = Wire.read();
    return value;
}          

// Write 8-bit to register
void MC3630::writeRegister8(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(MC3630_CFG_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Read 16-bit from register
int16_t MC3630::readRegister16(uint8_t reg)
{
    int16_t value;
    Wire.beginTransmission(MC3630_CFG_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MC3630_CFG_I2C_ADDR, 2);
    while(!Wire.available()) {};
    uint8_t vha = Wire.read();
    uint8_t vla = Wire.read();

    value = vha << 8 | vla;
    return value;
}

// Write 8-bit from register
void MC3630::writeRegister16(uint8_t reg, int16_t value)
{
    Wire.beginTransmission(MC3630_CFG_I2C_ADDR);
    Wire.write(reg);
    Wire.write((uint8_t)(value >> 8));
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

// Repeated Read Byte(s) from register
void MC3630::readRegisters(uint8_t reg, byte *buffer, uint8_t len)
{
    Wire.beginTransmission(MC3630_CFG_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false); //endTransmission but keep the connection active
    Wire.requestFrom(MC3630_CFG_I2C_ADDR, len); //Ask for bytes, once done, bus is released by default

    while(Wire.available() < len); //Hang out until we get the # of bytes we expect
    for(int x = 0 ; x < len ; x++)
	buffer[x] = Wire.read();
}
  
//Set the operation mode  
void MC3630::SetMode(MC3630_mode_t mode)
{
    uint8_t value;
    value = readRegister8(MC3630_REG_MODE_C);
    value &= 0b11110000;
    value |= mode;
    writeRegister8(MC3630_REG_MODE_C, value);
}

//Set the range control
void MC3630::SetRangeCtrl(MC3630_range_t range)
{
    uint8_t value;    
    CfgRange = range;
    SetMode(MC3630_MODE_STANDBY);
    value = readRegister8(MC3630_REG_RANGE_C);
    value &= 0b00000111;
    value |= (range << 4)&0x70 ;
    writeRegister8(MC3630_REG_RANGE_C, value);
}
void MC3630::reset()
{
	writeRegister8(0x24, 0x40);
}
void MC3630::SetSniffAGAIN(MC3630_gain_t gain)
{
	writeRegister8(0x20, 0x00);
	uint8_t value;
    value = readRegister8(MC3630_REG_GAIN);
    value &= 0b00111111;
    value |= (gain << 6);
    writeRegister8(MC3630_REG_GAIN, value);
}
void MC3630::SetWakeAGAIN(MC3630_gain_t gain)
{
	writeRegister8(0x20, 0x01);
	uint8_t value;
    value = readRegister8(MC3630_REG_GAIN);
    value &= 0b00111111;
    value |= (gain << 6);
    writeRegister8(MC3630_REG_GAIN, value);
}
//Set the resolution control
void MC3630::SetResolutionCtrl(MC3630_resolution_t resolution)
{
     uint8_t value;
     CfgResolution = resolution;
     SetMode(MC3630_MODE_STANDBY);
     value = readRegister8(MC3630_REG_RANGE_C);
     value &= 0b01110000;
     value |= resolution;
     writeRegister8(MC3630_REG_RANGE_C, value);
}

//Set the sampling rate
void MC3630::SetCWakeSampleRate(MC3630_cwake_sr_t sample_rate)
{
     uint8_t value;
     SetMode(MC3630_MODE_STANDBY);
     value = readRegister8(MC3630_REG_WAKE_C);
     value &= 0b00000000;
     value |= sample_rate;
     writeRegister8(MC3630_REG_WAKE_C, value);
}

//Get the output sampling rate
MC3630_cwake_sr_t MC3630::GetCWakeSampleRate(void)
{
/* Read the data format register to preserve bits */
     uint8_t value;
     value = readRegister8(MC3630_REG_WAKE_C);
     value &= 0b00000111;
     return (MC3630_cwake_sr_t) (value);
}

//Get the range control
MC3630_range_t MC3630::GetRangeCtrl(void)
{
  	/* Read the data format register to preserve bits */
     uint8_t value;
     value = readRegister8(MC3630_REG_RANGE_C);
     value &= 0x70;
     return (MC3630_range_t) (value >> 4);
}

//Get the range control
MC3630_resolution_t MC3630::GetResolutionCtrl(void)
{
  	/* Read the data format register to preserve bits */
     uint8_t value;
     value = readRegister8(MC3630_REG_RANGE_C);
     value &= 0x07;
     return (MC3630_resolution_t) (value);
}

//Initialize the MC3630 sensor and set as the default configuration
bool MC3630::start(void)
{
     Wire.begin(); // Initialize I2C
	 
	 //Reset
	 reset();
	 
     SetMode(MC3630_MODE_STANDBY);
	 
	 //SetWakeAGAIN
	 SetWakeAGAIN(MC3630_GAIN_1X);
	 //SetSniffAGAIN
	 SetSniffAGAIN(MC3630_GAIN_1X);
	 
     /* Check I2C connection */
     uint8_t id = readRegister8(MC3630_REG_PROD);
     if (id != 0x71)
     {
     /* No MC3630 detected ... return false */
     	Serial.println(id, HEX);
     	return false;
      }
      SetRangeCtrl(MC3630_RANGE_8G); //Range: 8g
      SetResolutionCtrl(MC3630_RESOLUTION_14BIT); //Resolution: 14bit
      SetCWakeSampleRate(MC3630_CWAKE_SR_DEFAULT_50Hz); //Sampling Rate: 50Hz
      SetMode(MC3630_MODE_CWAKE); //Mode: Active
      return true;
}

void MC3630::stop()
{
      SetMode(MC3630_MODE_SLEEP); //Mode: Sleep
}

//Read the raw counts and SI units mearsurement data
MC3630_acc_t MC3630::readRawAccel(void)
{
      float faRange[5] = { 19.614f, 39.228f, 78.456f, 156.912f, 117.684f}; //{2g, 4g, 8g, 16g, 12g}
      float faResolution[6] = { 32.0f, 64.0f, 128.0f, 512.0f, 2048.0f, 8192.0f}; //{6bit, 7bit, 8bit, 10bit, 12bit, 14bit}

      byte rawData[6];
      readRegisters(MC3630_REG_XOUT_LSB, rawData, 6);  // Read the six raw data registers into data array
      x = (short)((((unsigned short)rawData[1]) << 8) | rawData[0]);
      y = (short)((((unsigned short)rawData[3]) << 8) | rawData[2]);
      z = (short)((((unsigned short)rawData[5]) << 8) | rawData[4]);

      AccRaw.XAxis = (short) (x);
      AccRaw.YAxis = (short) (y);
      AccRaw.ZAxis = (short) (z);
      AccRaw.XAxis_g = (float) (x) / faResolution[CfgResolution]*faRange[CfgRange];
      AccRaw.YAxis_g = (float) (y) / faResolution[CfgResolution]*faRange[CfgRange];
      AccRaw.ZAxis_g = (float) (z) / faResolution[CfgResolution]*faRange[CfgRange];

      return AccRaw;
}
