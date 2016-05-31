
#include <Arduino.h>
#include <Titan_UNO.h>
#include <SoftwareSerial.h>


uint8_t cmd[20]; // received bytes
unsigned char toBTCommand[20];
unsigned char i = 0;
unsigned char payLoadIndex = 0;
unsigned char cmdEndIndex = 0;
boolean isEnd = false;
String cmdString = "";
uint8_t inputCmd = 0;
uint8_t rcvIndex = 0;
unsigned char idx = 0;

Titan_UNO ble = Titan_UNO();
titan_command_t trans_msg_t;   
titan_command_t recv_msg_t; 

int RX=10;  //Set your RX pin here.
int TX=11; //Set your TX pin here.

SoftwareSerial mySerialRCV(RX,TX);

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerialRCV.begin(9600);
  	
  while (!Serial) 
	  {
    	; // wait for serial port to connect. Needed for native USB port only
  	}
  Serial.println("Send Control Bytes...");
  
	trans_msg_t.PAYLOAD = (uint8_t *)malloc(sizeof(char[16+1]));                                                                                
	if(trans_msg_t.PAYLOAD == NULL)                                                                                                                  
  Serial.println("Memory Allocation Failed");  
	
	recv_msg_t.PAYLOAD = (uint8_t *)malloc(sizeof(char[16+1]));                                                                                
	if(recv_msg_t.PAYLOAD == NULL)                                                                                                                  
  Serial.println("Memory Allocation Failed"); 
		
	ble.init();		
  
}

uint8_t Ascii2Hex(uint8_t inbyte)
{
	uint8_t tmp;
	if (inbyte >= '0' && inbyte <= '9')
	{
		tmp =  (uint8_t)((unsigned char) inbyte - 48 );

	}
	else if (inbyte >= 'A' && inbyte <= 'F')
	{
		tmp =  (uint8_t)((unsigned char) inbyte - 65 + 10 );

	}
	else if (inbyte >= 'a' && inbyte <= 'f')
	{
		tmp =  (uint8_t)((unsigned char) inbyte - 97 + 10);

	}
	return tmp;
}


void clear()
{
	cmdString = "";
	payLoadIndex = 0;
	cmdEndIndex = 0;
	isEnd = false;
	memset(cmd,0,sizeof(cmd));
	memset(toBTCommand,0,sizeof(toBTCommand));
	memset(&trans_msg_t, 0, sizeof(struct titan_command_t));	
	i=0;
}

void commandRCV()
{
	
  mySerialRCV.listen();
	while (mySerialRCV.available() > 0)
	{

		uint8_t ucRcvByte = mySerialRCV.read();
		
		if (ucRcvByte != '\n')
		{
			Serial.println();
			if(rcvIndex < 20)
			{
				if (rcvIndex == 0)
				{	
					ble.printHeaderCmd(ucRcvByte);	
					Serial.print("CMD_HEADER: ");
					ble.PrintHex(ucRcvByte);
					recv_msg_t.CMD_HEADER = ucRcvByte;					
					rcvIndex++;
						
				}
				else if (rcvIndex == 1)
				{
					Serial.print("RESPONSE: ");
					recv_msg_t.CTRL_RESPONSE = ucRcvByte;
					ble.PrintHex(ucRcvByte);
					rcvIndex++;
				}
				else if (rcvIndex == 2)
				{
					Serial.print("PAYLOAD_LENGTH: ");
					ble.PrintHex(ucRcvByte);
					recv_msg_t.PAYLOAD_LEN = ucRcvByte;
					rcvIndex++;
				}
				else if (rcvIndex == 3)
				{					
					if (recv_msg_t.PAYLOAD_LEN >= 1)
					{
						Serial.print("PAYLOAD:[");
						Serial.print(idx);
						Serial.print("]: ");
						if (idx < recv_msg_t.PAYLOAD_LEN)
						{	
							*(recv_msg_t.PAYLOAD+idx)= (uint8_t) ucRcvByte;
							ble.PrintHex(ucRcvByte);
							idx++;	
							if(idx == recv_msg_t.PAYLOAD_LEN)
							{	
								idx=0;
								rcvIndex++;
							}
						}
					}
					else if (recv_msg_t.PAYLOAD_LEN == 0)
					{
						*(recv_msg_t.PAYLOAD) = ucRcvByte;	
						Serial.print("PAYLOAD=");
						ble.PrintHex(ucRcvByte);	
						rcvIndex++;
					}
				}
			}
		}
		else if(ucRcvByte == '\r')
		{
			//Serial.println("The Carriage Return Byte Received");
			recv_msg_t.CR = ucRcvByte;
			rcvIndex++;
		}
		else if(ucRcvByte == '\n')
		{
			//Serial.println("The New Line Byte Received");
			recv_msg_t.NL = ucRcvByte;
			//20151211
			rcvIndex = 0;

		}
	}
	//Serial.print("=");
	delayMicroseconds(500);
}

void loop()
{

	if(Serial.available() > 0)
	{
		uint8_t inByte = Serial.read();

		if (!ble.checkAscii(inByte))
		{
		  Serial.println();
		  Serial.println("Not Valid Command Format, Please input Hexdecimal digit (i.e. 0 - 9, a - F, or A - F) !!");
		  clear();
		  return;
		}
		if (inByte != '\r' && inByte != '\n')
		{
			cmdString += (char)inByte;


			if (i < (20 - 4))
			{
				cmd[(i) % (20 - 4)] = (uint8_t)Ascii2Hex(inByte);

					
				if (isEnd)
				{
					Serial.println("Wrong Command - No end bytes 0x0d 0x0a");
					clear();
					return;
				}
                               
				/*Command Header*/
				if (i == 1)
				{
					trans_msg_t.CMD_HEADER = (cmd[0]) << 4 | (cmd[1]);
					toBTCommand[i/2] = trans_msg_t.CMD_HEADER;
					ble.printHeaderCmd(trans_msg_t.CMD_HEADER);
					Serial.print("CMD_HEADER: ");                                        
					ble.PrintHex(trans_msg_t.CMD_HEADER);
					Serial.println();
					

					
				}
				/* Control or Response*/
				else if (i == 3)
				{
					trans_msg_t.CTRL_RESPONSE = (cmd[2]) << 4 | (cmd[3]);
					toBTCommand[i/2] = trans_msg_t.CTRL_RESPONSE;
					Serial.print("CONTROL: ");
					ble.PrintHex(trans_msg_t.CTRL_RESPONSE);
					Serial.println();

				}
				/*Payload length*/
				else if (i == 5)
				{
					trans_msg_t.PAYLOAD_LEN = (cmd[4]) << 4 | (cmd[5]);
					toBTCommand[i/2] = trans_msg_t.PAYLOAD_LEN;
					Serial.print("PAYLOAD_LEN: ");
					ble.PrintHex(trans_msg_t.PAYLOAD_LEN);
          Serial.println();    

				}
				/*Payload*/
				else if (i >= 6 && i <= 20)
				{
					if (trans_msg_t.PAYLOAD_LEN > 0)
					{
						if ((i <= (6 + trans_msg_t.PAYLOAD_LEN * 2)) && (i % 2 == 1))
						{
							*(trans_msg_t.PAYLOAD+payLoadIndex)= (cmd[i - 1]) << 4 | (cmd[i]);                                                                                                                                 
							toBTCommand[i/2] = (cmd[i-1]) << 4 | (cmd[i]);    
						  
							Serial.print("PAYLOAD[");
							Serial.print(payLoadIndex);
							Serial.print("]: ");
							ble.PrintHex(trans_msg_t.PAYLOAD[payLoadIndex]);  
                            Serial.println();
							if(payLoadIndex < trans_msg_t.PAYLOAD_LEN)
								payLoadIndex++;
						}
						else
						{
							if (payLoadIndex == trans_msg_t.PAYLOAD_LEN)
							{
								cmdEndIndex = i;

								payLoadIndex++;
							}
						}
					}
					/* Payload Length = 0 */
					else if ( trans_msg_t.PAYLOAD_LEN == 0 && (i % 2 == 1) && !isEnd)
					{				
						*(trans_msg_t.PAYLOAD) = (uint8_t) 0; // (cmd[6]) << 4 | (cmd[7]);                                               
						toBTCommand[i/2] = (uint8_t) 0;  					
						Serial.print("PAYLOAD: ");
						ble.PrintHex(*(trans_msg_t.PAYLOAD));
						Serial.println();			
					}
				}
				i++;
			}
		}
		/* Carriage Return Byte */
		else if (inByte == '\r')
		{
			//Serial.println("CR");
			//ardprintf("inByte= %d\n" , inByte ); //13 0x0d
			trans_msg_t.CR = B00001101;
			i++;

			toBTCommand[i/2] = B00001101;
			cmd[20-4] = (uint8_t) 0;
			cmd[20-3] = (uint8_t) 13;
		}
		/* New Line Byte */
		if (inByte == '\n')
		{
			cmd[20-2] = (uint8_t) 0;
			cmd[20-1] = (uint8_t) 10;
			i = i + 2;

			toBTCommand[i/2] = B00001010;
			
			Serial.print("Send Command Bytes: 0x");
			Serial.println(cmdString);
			
			/* Input Command Foarmat Check */
			if (payLoadIndex != trans_msg_t.PAYLOAD_LEN)
			{
				Serial.println("Wrong Payload Content");
				clear();
				return;
			}
			/* Input Command Foarmat Check */
			else if (i & 1 == 0)
			{
				Serial.println("Wrong Input Format, Please send correct amount of hexdecimal digits");
				clear();
				return;       
			}
			else
			{
				cmdEndIndex = (i+1) >> 1 ;
				delayMicroseconds(10);
				ble.commandTRANS(trans_msg_t.CMD_HEADER, &toBTCommand[0], cmdEndIndex, RX ,TX);
			}
			cmdString = "";
			payLoadIndex = 0;
			isEnd = false;
			memset(cmd,0,sizeof(cmd));	  
			i = 0;
		}

		delayMicroseconds(5000);
	
    }
	commandRCV();

}
