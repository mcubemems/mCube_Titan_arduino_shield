#ifndef Titan_h
#define Titan_h
#include <inttypes.h>


#define cmd_control_disable                                         0x00
#define cmd_control_enable                                          0x01
#define cmd_response_ok                                             0x00
#define cmd_response_nak                                            0x01

struct titan_command_t
{
  uint8_t CMD_HEADER;
  uint8_t CTRL_RESPONSE;
  uint8_t PAYLOAD_LEN;
  uint8_t *PAYLOAD;
  uint8_t CR;
  uint8_t NL;
} ;

typedef enum
{
    PEDOMETER_CONTROL							=0b00010000, //0x10       
    PEDOMETER_CONTROL_RESPONSE                  =0b00010001, //0x11   
    PEDOMETER_RECORD_GET                        =0b00010010, //0x12   
    PEDOMETER_RECORD_GET_RESPONSE               =0b00010011, //0x13   
    PEDOMETER_STEP_GET                          =0b00010100, //0x14   
    PEDOMETER_STEP_GET_RESPONSE                 =0b00010101, //0x15   
    PEDOMETER_NOTIFICATION_ENABLE               =0b00010110, //0x16   
    PEDOMETER_NOTIFICATION_ENABLE_RESPONSE      =0b00010111, //0x17   
    PEDOMETER_NOTIFICATION                      =0b00011000, //0x18   
    
    //SLEEP MONITOR                                                   
    SLEEP_MONITOR_CONTROL                       =0b00100000, //0x20   
    SLEEP_MONITOR_CONTROL_RESPONSE              =0b00100001, //0x21   
    SLEEP_MONITOR_SENSITIVITY_SET_RESPONSE      =0b00100011, //0x23      
    SLEEP_MONITOR_NOTIFICATION_ENABLE           =0b00100100, //0x24
    SLEEP_MONITOR_NOTIFICATION_ENABLE_RESPONSE	=0b00100101, //0x25
    SLEEP_MONITOR_EVENT_NOTIFICATION            =0b00100110, //0x26 
 
	
	//Sensor Control
	SENSOR_CONTROL                              =0b01110000, //0x70
    SENSOR_CONTROL_RESPONSE                     =0b01110001, //0x71

/**
 * Reporter commands
 */
	REPORTER_CONTROL                            =0b10000000, //0x80
	REPORTER_CONTROL_RESPONSE                   =0b10000001, //0x81
    


}   command_header_t;
class Titan_UNO
{

public:
  uint8_t inputCmd;
  void commandTRANS(uint8_t bCmdHeader, uint8_t* baCmdBytes, uint8_t len, int RX, int TX);
  void init(void);
  bool commandRCV(void);
  bool checkAscii(uint8_t thisChar);
  void printHeaderCmd(uint8_t cmdBytes);
  void PrintHex(uint8_t data);
};

#endif
