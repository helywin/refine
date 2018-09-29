/*调试专用程序,在最后的产品中应该删除本文件及与之对应的.h文件*/
#include <math.h>
#include ".\CANF\can.h"
#include "synctrl.h"
#include ".\Clutch\ClutchCtrl.h"
#include "gearcontrol.h"
#include ".\TLE6228CF\Tle6228.h"
#include ".\TLE7242F\Tle7242.h"
#include ".\TLE82453EF\Tle82453e.h"
#include ".\ADC\ADC.h"
#include ".\DFlash\DFlash.h"
#include ".\LIMP\Error.h"
#include "HardWareTest.h"
#include "Start_Control.h"
#include "diag.h"
#include "Gradient.h"
#include "Engin State Identify.h"
#include "telecontrol\telecontrol.h"
#include "LookTargetGear.h"

volatile unsigned char debug_tx_buff[256];
volatile unsigned char debug_tx_first=0;
volatile unsigned char debug_tx_last=0;

volatile unsigned char debug_rx_buff[256];
volatile unsigned char debug_rx_first=0;
volatile unsigned char debug_rx_last=0;

volatile unsigned char debug_tx_buff_777[256];
volatile unsigned char debug_tx_first_777=0;
volatile unsigned char debug_tx_last_777=0;

volatile unsigned char debug_data[8];
volatile unsigned char command=0;
volatile unsigned char debug_rx_flag=0;
volatile unsigned char debug_len=0;
volatile unsigned char SendStop = 8;
extern unsigned short testw,testw1;
extern unsigned short tempcnt;
extern unsigned char  GTTFlg;

extern unsigned char debug_axil1err=0;
extern unsigned char TrMaxGear;
extern unsigned char OddErroTest;
extern unsigned char EvenErroTest;
extern unsigned char TestEco;
extern unsigned char Dlh;
unsigned char PUMPTEST;
volatile unsigned char Motor_Start_debug = 0;

unsigned short cnt0 = 0;
unsigned short cnt1 = 233;
unsigned short cnt2 = 545;
unsigned short cnt3 = 334;
unsigned char cnt0_flag = 1;
unsigned char cnt1_flag = 1;
unsigned char cnt2_flag = 1;
unsigned char cnt3_flag = 1;

unsigned short cnt4 = 123;
unsigned short cnt5 = 42;
unsigned short cnt6 = 12;
unsigned short cnt7 = 0;
unsigned char cnt4_flag = 0;
unsigned char cnt5_flag = 0;
unsigned char cnt6_flag = 0;
unsigned char cnt7_flag = 0;

unsigned long frame_index = 0;

void Debug_rx_handle(void)
{
	unsigned char i;
	unsigned char temp;
 	if(debug_rx_flag==1)/*收到新消息*/
    {
      	if(debug_len>8)
      		debug_len = 8;
 		for(i=0;i<debug_len;i++)
       	{
        	temp = debug_data[i];
        	if(temp == 8) /*收到退格键*/ 
         	{
          		debug_tx_buff[debug_tx_last] = 8;
          		debug_tx_last++;
          		debug_rx_last--;
         		debug_rx_buff[debug_rx_last] = 0;
         	}
        	else                                         
         	{     
          		debug_tx_buff[debug_tx_last] = temp;
          		debug_tx_last++;
          		debug_rx_buff[debug_rx_last] = temp;
          		debug_rx_last++;
         	}
         
       		if(temp == 0x0d) /*收到回车键*/
        	{
        		command++; 
        	} 
        } 
        debug_rx_flag = 0;
    }
}
unsigned char CommandDealSetV(char *pt,unsigned char ll,unsigned short *value)
{
 	unsigned char i,j;
 	unsigned char temp2;
 	unsigned char len;
 	unsigned short temp;

	if(debug_rx_buff[debug_rx_first]=='S'
		&&debug_rx_buff[debug_rx_first+1]=='E'
		&&debug_rx_buff[debug_rx_first+2]=='T'
		&&debug_rx_buff[debug_rx_first+3]=='_'
		/*&&debug_rx_buff[debug_rx_first+4]=='\0'*/)
	{
		j=0;
		for(i=0;i<ll;i++)
		{
			if(debug_rx_buff[debug_rx_first+i+4]==*(pt+i))
			{
				j = 1;
			}
			else
			{
				j = 0;
				break;
			}
		}
		if(j==1)
		{
			len = 10;
			for(i=0;i<6;i++)
			{
				if(debug_rx_buff[debug_rx_first+4+ll+i]==0x0d)
					break;
				else
					len=i+1;
			}
			if(len<=5)
				j=1;
			else
				j=0;
			temp = 0;
			for(i=0;i<len;i++)
			{
				temp *= 10;
				temp2=debug_rx_buff[debug_rx_first+4+ll+i];
				temp2 = temp2-'0';
				temp += temp2;
			}

			*value = temp;
		}
	}
	else
 		j=0;

 	return j;
}
void send_word_Dec(unsigned short a)
{
	unsigned char b;

	b=(unsigned char)(a/10000)+'0';
	*( debug_tx_buff + debug_tx_last) = b;
	debug_tx_last++;
	a=a%10000;

	b=(unsigned char)(a/1000)+'0';
	*( debug_tx_buff + debug_tx_last) = b;
	debug_tx_last++;
	a=a%1000;

	b=(unsigned char)(a/100)+'0';
	*( debug_tx_buff + debug_tx_last) = b;
	debug_tx_last++;
	a=a%100;

	b=(unsigned char)(a/10)+'0';
	*( debug_tx_buff + debug_tx_last) = b;
	debug_tx_last++;
	a=a%10;

	b=(unsigned char)a+'0';
	*( debug_tx_buff + debug_tx_last) = b;
	debug_tx_last++;
}

void debug_send(char *pt,unsigned char length)
{
 	unsigned char i;
 	unsigned char len;

 	for(i=0;i<length;i++)
   	{
 		len = debug_tx_last+i;
 		debug_tx_buff[len] = *(pt+i);
   	}
 	debug_tx_last += length;
}

unsigned char CommandDeal(char *pt, unsigned char length)
{
 	unsigned char i,j;
 	unsigned char len;

	j=0;
 	for(i=0;i<length;i++)
 	{
 		len = debug_rx_first+i;
 		if(debug_rx_buff[len]==*(pt+i))
  		{
   			j = 1;
  		}
  		else
  		{
   			j = 0;
   			break; 
  		}
 	}
 	if( j==1 )  
 	{
 		len = debug_rx_first+i;
 		if(debug_rx_buff[len] != 0x0d )
 	        j = 0;
 	}
 	return j;
}

unsigned char Digit_ASCII(unsigned char temp) /*将二进制数转换为相应的ASCII字符*/
{
    if(temp>=0x0a)
		temp = temp - 0x0a +'A';
	else
		temp =temp + '0';
	return temp;
}

unsigned char ASCII_Digit(unsigned char temp)
{
  	if((temp >= '0')&(temp <= '9'))
    	temp = temp - '0';
  	else if((temp >= 'A')&(temp <= 'F'))
    	temp = temp - 'A' + 0x0a;
  	else if((temp >= 'a')&(temp <= 'f'))
    	temp = temp - 'a' + 0x0a;;
  	return temp;
}

void send_dword_hex(unsigned long a)
{
	unsigned char b;

                                      
  	a = a %0x01000000;
 
  	b = (unsigned char)(a/0x00100000);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x00100000;
  
  	b = (unsigned char)(a/0x00010000);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x00010000; 
  
  	b = (unsigned char)(a/0x00001000);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x00001000; 
  
  	b = (unsigned char)(a/0x00000100);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x00000100;
  
  	b = (unsigned char)(a/0x00000010);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
   

  	b = (unsigned char)(a%0x0010);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;   
 
}

void send_word_hex(unsigned short a)
{
	unsigned char b;
                                     
  	b = (unsigned char)(a/0x1000);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x1000; 
  
  	b = (unsigned char)(a/0x0100);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
  	a = a %0x0100;
  
  	b = (unsigned char)(a/0x0010);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
   
  	b = (unsigned char)(a%0x0010);
  	b = Digit_ASCII(b);
  	*( debug_tx_buff + debug_tx_last) = b;
  	debug_tx_last++;
 
}

void MyDebug(void)
{
    unsigned char i;
    
    CAN1_Debug_Receive();			//接收调试信息
    Debug_rx_handle();
	CAN1_Debug_Transmit();
	CAN1_Debug_Transmit_777();

	if(command>0) 
	{
 		debug_send("\r\n",2); 
 		if(CommandDeal("WWW",3)==1)			//清标志
		{
 			Write8bitsDataToDFlash(ENTERBOOTLOAD,0xAA);
 			DFlashDataSave();
			Clutch1.TargetPress = 0;
			Clutch2.TargetPress = 0;
			DiagSessionMessage.ProgramLock = 0;
		}
		else if(CommandDeal("RRR",3)==1)		//软件复位
		{
			SoftWareReset();
		}
#if	DEBUGI==0
		else if( CommandDeal("GETAD",sizeof("GETAD")-1) )
 		{
			debug_send("\r\nF13",sizeof("\r\nF13"));
 		    send_word_hex(Shift13.Position);
 		    //send_word_hex(Sensor_13);
 		    debug_send("\r\nF24",sizeof("\r\nF24")); 
 		    send_word_hex(Shift24.Position);
 		    //send_word_hex(Sensor_24);
 		    debug_send("\r\nF5n",sizeof("\r\nF5n")); 
 		    send_word_hex(Shift5n.Position);
 		    //send_word_hex(Sensor_5N);
 		    debug_send("\r\nF6R",sizeof("\r\nF6R")); 
 		    send_word_hex(Shift6r.Position);
 		    //send_word_hex(Sensor_6R);
 		    debug_send("\r\nC1",sizeof("\r\nC1")); 
 		    send_word_hex(Clutch1.Press);
 		    debug_send("\r\nC2",sizeof("\r\nC2")); 
 		    send_word_hex(Clutch2.Press);
 		    debug_send("\r\nIS",sizeof("\r\nIS")); 
 		    send_word_hex(ControlInfo.InShaftSpeed);
 		    debug_send("\r\nOS",sizeof("\r\nOS")); 
 		    send_word_hex(ControlInfo.OutPutShaftSpeed);
 		    debug_send("\r\nC1S",sizeof("\r\nC1S")); 
 		    send_word_hex(Clutch1.ShaftSpeed);
 		    debug_send("\r\nC2S",sizeof("\r\nC1S")); 
 		    send_word_hex(Clutch2.ShaftSpeed);
 		}
 		else if( CommandDeal("GETCLUTCH",sizeof("GETCLUTCH")-1) )
 		{
 		    debug_send("\r\nC1:",sizeof("\r\nC1:")-1); 
 		    debug_send("\r\nP",sizeof("\r\nP")-1); 
 		    send_word_hex(Clutch1.Press);
 		    debug_send("\r\nC",sizeof("\r\nC")-1); 
 		    send_word_hex(Clutch1.SetCurrent);
 		    debug_send("\r\nS",sizeof("\r\nS")-1); 
 		    send_word_hex(Clutch1.ShaftSpeed);
 		    debug_send("\r\nAP",sizeof("\r\nAP")-1); 
 		    send_word_hex(Clutch1.ApartPress);
 		    debug_send("\r\nSF",sizeof("\r\nSF")-1); 
 		    send_word_hex(Clutch1.SuitFlag);
 		    debug_send("\r\nSO",sizeof("\r\nSO")-1);
 		    send_word_hex(Clutch1SuitInfo.StudyOk);
 		    debug_send("\r\nB",sizeof("\r\nB")-1); 
 		    send_word_hex(Clutch1.SuitFlag);
 		    debug_send("\r\nC2:",sizeof("\r\nC2:")-1); 
 		    debug_send("\r\nP",sizeof("\r\nP")-1); 
 		    send_word_hex(Clutch2.Press);
 		    debug_send("\r\nC",sizeof("\r\nC")-1); 
 		    send_word_hex(Clutch2.SetCurrent);
 		    debug_send("\r\nS",sizeof("\r\nS")-1); 
 		    send_word_hex(Clutch2.ShaftSpeed);
 		    debug_send("\r\nAP",sizeof("\r\nAP")-1); 
 		    send_word_hex(Clutch2.ApartPress);
 		    debug_send("\r\nSF",sizeof("\r\nSF")-1); 
 		    send_word_hex(Clutch2.SuitFlag);
 		    debug_send("\r\nSO",sizeof("\r\nSO")-1);
 		    send_word_hex(Clutch2SuitInfo.StudyOk);
 		    debug_send("\r\nB",sizeof("\r\nB")-1); 
 		    send_word_hex(Clutch2.SuitFlag);
 		}
 		else if( CommandDeal("GETCONTROL",sizeof("GETCONTROL")-1) )
 		{
 		    debug_send("\r\nBK",sizeof("\r\nBK")-1); 
 		    send_word_hex(ControlInfo.Brake);
 		    debug_send("\r\nAC",sizeof("\r\nAC")-1); 
 		    send_word_hex(ControlInfo.AccValue);
 		    debug_send("\r\nGT",sizeof("\r\nGT")-1); 
 		    send_word_hex(ControlInfo.TargetGear);
            debug_send("\r\nGC",sizeof("\r\nGC")-1); 
 		    send_word_hex(ControlInfo.CurrentGear);
 		    debug_send("\r\nGP",sizeof("\r\nGP")-1); 
 		    send_word_hex(ControlInfo.GearPosition);
 		    debug_send("\r\nMM",sizeof("\r\nMM")-1); 
 		    send_word_hex(ControlInfo.MainMinCurrent); 
 		}
 		else if( CommandDeal("GETSHIFT",sizeof("GETSHIFT")-1) )
 		{
 		    debug_send("\r\n13T",sizeof("\r\n13T")-1); 
 		    send_word_hex(Shift13.TargetStatus);
 		    debug_send("\r\n13C",sizeof("\r\n13C")-1); 
 		    send_word_hex(Shift13.CurrentStatus);
 		    debug_send("\r\n24T",sizeof("\r\n13T")-1); 
 		    send_word_hex(Shift24.TargetStatus);
 		    debug_send("\r\n24C",sizeof("\r\n13C")-1); 
 		    send_word_hex(Shift24.CurrentStatus);
 		    debug_send("\r\n5NT",sizeof("\r\n13T")-1); 
 		    send_word_hex(Shift5n.TargetStatus);
 		    debug_send("\r\n5NC",sizeof("\r\n13C")-1); 
 		    send_word_hex(Shift5n.CurrentStatus);
 		    debug_send("\r\n6RT",sizeof("\r\n13T")-1); 
 		    send_word_hex(Shift6r.TargetStatus);
 		    debug_send("\r\n6rC",sizeof("\r\n13C")-1); 
 		    send_word_hex(Shift6r.CurrentStatus);
 		    
 		}
 		else if( CommandDeal("GETMID",sizeof("GETMID")-1) )
		{
			debug_send("\r\n13M",sizeof("\r\n13T")-1);
			send_word_hex(Shift13.MidPosition);
			debug_send("\r\n24M",sizeof("\r\n13T")-1);
			send_word_hex(Shift24.MidPosition);
			debug_send("\r\n5NM",sizeof("\r\n13T")-1);
			send_word_hex(Shift5n.MidPosition);
			debug_send("\r\n6RM",sizeof("\r\n13T")-1);
			send_word_hex(Shift6r.MidPosition);
		}
 		else if( CommandDeal("GETC",sizeof("GETC")-1) )
 		{
 		    debug_send("\r\nMC",sizeof("\r\nMC")-1); 
 		    send_word_hex(ControlInfo.MainCurrentSet);
 		    debug_send("\r\nS1C",sizeof("\r\nS1C")-1); 
 		    send_word_hex(ControlInfo.Safe1CurrentSet);
 		    debug_send("\r\nS2C",sizeof("\r\nS2C")-1); 
 		    send_word_hex(ControlInfo.Safe2CurrentSet);
 		    debug_send("\r\nC1C",sizeof("\r\nC1C")-1); 
 		    send_word_hex(Clutch1.SetCurrent);
 		    debug_send("\r\nC2C",sizeof("\r\nC2C")-1); 
 		    send_word_hex(Clutch2.SetCurrent);
 		    debug_send("\r\nMLS",sizeof("\r\nMLS")-1); 
 		    send_word_hex(ControlInfo.MlsvCurrentSet);
 		    debug_send("\r\nCOC",sizeof("\r\nCOC")-1); 
 		    send_word_hex(ControlInfo.SetCoolingValveCurrent);
 		}
 		else if(CommandDeal("PX",2)==1)
	 	{
	  		ControlInfo.DebugMode=0x55;
	 	}
 		else if(CommandDeal("PN",2)==1)
		{
			ControlInfo.DebugMode=0;
		}
 		else if(CommandDeal("AXIL1ERR",sizeof("AXIL1ERR")-1)==1)
 		{
 			if(debug_axil1err==0)
 			{
 				debug_axil1err = 1;
 				debug_send("\r\naxil1err",sizeof("\r\naxil1err")-1);
 			}
 			else
 			{
 				debug_axil1err = 0;
 				debug_send("\r\naxil1 right",sizeof("\r\naxil1 right")-1);
 			}
 		}
 		else if(CommandDeal("GETSLOP",7)==1)
		{
 			debug_send("\r\nSZ",sizeof("\r\nSZ")-1);
 			send_word_hex(xSlopInfo.xSlopZero);
 			debug_send("\r\nSS",sizeof("\r\nSS")-1);
 			send_word_hex(xSlopInfo.xSlop);
 			debug_send("\r\nAD",sizeof("\r\nAD")-1);
 			send_word_hex(xSlopInfo.xSlopFilterAD);
		}
	 	else if(CommandDeal("PXR",3)==1)
	 	{
	  		ControlInfo.DebugMode=0;
	 	}
	 	else if(CommandDeal("C1-",3)==1)
		{
			Clutch1.ApartPress -= 10;
			WriteClutch1InfoToEeprom( );
		}
	 	else if(CommandDeal("C1+",3)==1)
		{
			Clutch1.ApartPress += 10;
			WriteClutch1InfoToEeprom( );
		}
	 	else if(CommandDeal("C2-",3)==1)
		{
			Clutch2.ApartPress -= 10;
			WriteClutch2InfoToEeprom( );
		}
		else if(CommandDeal("C2+",3)==1)
		{
			Clutch2.ApartPress += 10;
			WriteClutch2InfoToEeprom( );
		}
 		else if(CommandDeal("T1",2)==1)
	 	{
 			GTTFlg=1;
	 	}
	 	else if(CommandDeal("T0",2)==1)
	 	{
	  		testw=0;   
	  		GTTFlg=0;
	 	}
	 	else if(CommandDeal("TT1",3)==1)
	 	{
	  		testw1=1;   
	 	}
	 	else if(CommandDeal("TT0",3)==1)
	 	{
	  		testw1=0;   
	 	}
	 	else if(CommandDeal("GETCO",5)==1)
		{
	 		debug_send("\r\nC1=",sizeof("\r\nC1=")-1);
	 		send_word_hex(Clutch1.ClutchCoRp);
	 		debug_send(" C2=",sizeof(" C2=")-1);
	 		send_word_hex(Clutch2.ClutchCoRp);
		}
	 	else if(CommandDeal("SETCO",5)==1)
		{
	 		Clutch1.ClutchCoRp = -5;
	 		Clutch2.ClutchCoRp = -5;
	 		debug_send("\r\nC1=",sizeof("\r\nC1=")-1);
			send_word_hex(Clutch1.ClutchCoRp);
			debug_send(" C2=",sizeof(" C2=")-1);
			send_word_hex(Clutch2.ClutchCoRp);
			Write16bitsDataToDFlash(CLUTCH1CORV,Clutch1.ClutchCoRp);
			Write16bitsDataToDFlash(CLUTCH2CORV,Clutch2.ClutchCoRp);
		}
	 	else if(CommandDeal("PW1",3)==1)
	 	{
	 		ControlInfo.PumpWorkStatus = 1;
	 	}
	 	else if(CommandDeal("PW0",3)==1)
		{
			ControlInfo.PumpWorkStatus = 0;
		}
	 	else if(CommandDeal("SHIFT1",sizeof("SHIFT1")-1)==1)
	 	{
	  		Shift13.TargetStatus = SHIFT_1; 
	  		Shift13.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT3",sizeof("SHIFT3")-1)==1)
	 	{
	  		Shift13.TargetStatus = SHIFT_3; 
	  		Shift13.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT13N",sizeof("SHIFT13N")-1)==1)
	 	{
	  		Shift13.TargetStatus = SHIFT_N; 
	  		Shift13.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT2",sizeof("SHIFT2")-1)==1)
	 	{
	  		Shift24.TargetStatus = SHIFT_2; 
	  		Shift24.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT4",sizeof("SHIFT4")-1)==1)
	 	{
	  		Shift24.TargetStatus = SHIFT_4; 
	  		Shift24.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT24N",sizeof("SHIFT24N")-1)==1)
	 	{
	  		Shift24.TargetStatus = SHIFT_N; 
	  		Shift24.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFTR",sizeof("SHIFTR")-1)==1)
	 	{
	  		Shift6r.TargetStatus = SHIFT_R; 
	  		Shift6r.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT6",sizeof("SHIFT6")-1)==1)
	 	{
	  		Shift6r.TargetStatus = SHIFT_6; 
	  		Shift6r.CurrentStatus = SHIFT_NULL;
	 	}

	 	else if(CommandDeal("SHIFT6RN",sizeof("SHIFT6RN")-1)==1)
	 	{
	  		Shift6r.TargetStatus = SHIFT_N; 
	  		Shift6r.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT5",sizeof("SHIFT5")-1)==1)
	 	{
	  		Shift5n.TargetStatus = SHIFT_5; 
	  		Shift5n.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFT5N",sizeof("SHIFT5N")-1)==1)
	 	{
	  		Shift5n.TargetStatus = SHIFT_N; 
	  		Shift5n.CurrentStatus = SHIFT_NULL;
	 	}
	 	else if(CommandDeal("SHIFTEC",sizeof("SHIFTEC")-1)==1)
		{
			Shiftecd.TargetStatus = EVCG;
			Shiftecd.CurrentStatus = SHIFT_NULL;
		}
	 	else if(CommandDeal("SHIFTED",sizeof("SHIFTED")-1)==1)
		{
	 		Shiftecd.TargetStatus = EVDG;
	 		Shiftecd.CurrentStatus = SHIFT_NULL;
		}
	 	else if(CommandDeal("SHIFTEN",sizeof("SHIFTEN")-1)==1)
		{
			Shiftecd.TargetStatus = EVNG;
			Shiftecd.CurrentStatus = SHIFT_NULL;
		}
	 	else if(CommandDeal("GET6228",7)==1)
	 	{
	  		debug_send("\r\n",sizeof("\r\n")); 
 		    send_word_hex(TLE6228STATUS1);
 		    debug_send("\r\n",sizeof("\r\n")); 
 		    send_word_hex(TLE6228STATUS2);
 		    debug_send("\r\n",sizeof("\r\n")); 
 		    send_word_hex(TLE6228STATUS3);
 		    debug_send("\r\n",sizeof("\r\n")); 
 		    send_word_hex(TLE6228STATUS4);
	 	}
	 	else if(CommandDeal("GETT",4)==1)
	 	{
	  		debug_send("\r\nOT",sizeof("\r\nOT")); 
 		    send_word_hex(OilTemperatureAd);
 		    debug_send(" ",sizeof(" ")); 
 		    send_word_hex(ControlInfo.OilTemperature);
 		    debug_send("\r\nCT",sizeof("\r\nCT")); 
 		    send_word_hex(ClutchTemperatureAd);
 		    debug_send(" ",sizeof(" ")); 
 		    send_word_hex(ControlInfo.ClutchTemperature);
 		    debug_send("\r\nMT",sizeof("\r\nMT")); 
 		    send_word_hex(MainTemperatureAd);
 		    debug_send(" ",sizeof(" ")); 
 		    send_word_hex(ControlInfo.MainBoardTemperature);
 		   debug_send("\r\nPT",sizeof("\r\nPT"));
			send_word_hex(ControlInfo.CpuTemperature);
	 	}
        else if(CommandDeal("SP+",3)==1)
        {
            SendStop++;               
            debug_send("\r\n",2);
            send_word_hex(SendStop);
            if(SendStop==6)
            	DiagSendStop++;
            else
            	DiagSendStop=1;
        }
        else if(CommandDeal("SP-",3)==1)
        {
            if(SendStop>0)
                SendStop--;               
            debug_send("\r\n",2);
            send_word_hex(SendStop);
            if(SendStop==6)
            	DiagSendStop++;
            else
            	DiagSendStop=1;
        }
        else if(CommandDeal("PPT+",4)==1)
	   {
		   debug_send("\r\n",2);
		   PUMPTEST++;
		   send_word_hex(PUMPTEST);
	   }
        else if(CommandDeal("PPT-",4)==1)
	   {
		   debug_send("\r\n",2);
		   if(PUMPTEST>0)
			   PUMPTEST--;
		   send_word_hex(PUMPTEST);
	   }
        else if(CommandDeal("C1C+",4)==1)
		{
        	Clutch1.Status=CLUTCHOPENCTRL;
        	Clutch1.SetCurrent += 10;
		}
        else if(CommandDeal("C1C-",4)==1)
		{
			Clutch1.Status=CLUTCHOPENCTRL;
			if(Clutch1.SetCurrent>10)
				Clutch1.SetCurrent -= 10;
		}
        else if(CommandDeal("C1CR",4)==1)
		{
			Clutch1.Status=CLUTCHCLOSECTRL;
		}
        else if(CommandDeal("C2C+",4)==1)
		{
			Clutch2.Status=CLUTCHOPENCTRL;
			Clutch2.SetCurrent += 10;
		}
		else if(CommandDeal("C2C-",4)==1)
		{
			Clutch2.Status=CLUTCHOPENCTRL;
			if(Clutch2.SetCurrent>10)
				Clutch2.SetCurrent -= 10;
		}
		else if(CommandDeal("C2CR",4)==1)
		{
			Clutch2.Status=CLUTCHCLOSECTRL;
		}
		else if(CommandDeal("C2CR",4)==1)
		{
			Clutch2.Status=CLUTCHCLOSECTRL;
		}
		else if(CommandDeal("SMV",3)==1)
		{
			MainValveInfo.SuitFlag=0x55;
		}
		else if(CommandDeal("SFV",3)==1)
		{
			SafeValve1Info.SuitFlag=0x55;
			SafeValve2Info.SuitFlag=0x55;
		}
		else if(CommandDeal("SCV",3)==1)
		{
			ClutchValve1Info.SuitFlag=0x55;
			ClutchValve2Info.SuitFlag=0x55;
		}
		else if(CommandDeal("SOV",3)==1)
		{
			OilPumpInfo.SuitFlag=0x55;
		}
		else if(CommandDeal("TECO",4)==1)
		{
			TestEco=1;
		}
		else if(CommandDeal("TECOR",4)==1)
		{
			TestEco=0;
		}
		else if(CommandDeal("AAV",3)==1)
		{
			ControlInfo.AtuoTest=0x55;
			OilPumpInfo.SuitOk = 0;
			MainValveInfo.SuitOk = 0;
			SafeValve1Info.SuitOk = 0;
			SafeValve2Info.SuitOk = 0;
			ClutchValve1Info.SuitOk = 0;
			ClutchValve2Info.SuitOk = 0;
		}
		else if(CommandDeal("GETAAV",6)==1)
		{
			debug_send("1\r\n",3);
			send_word_hex(Shift1SuitInfo.SuitOk);
			debug_send("3\r\n",3);
			send_word_hex(Shift3SuitInfo.SuitOk);
			debug_send("13N\r\n",5);
			send_word_hex(Shift13NSuitInfo.SuitOk);
			debug_send("2\r\n",3);
			send_word_hex(Shift2SuitInfo.SuitOk);
			debug_send("4\r\n",3);
			send_word_hex(Shift4SuitInfo.SuitOk);
			debug_send("24N\r\n",5);
			send_word_hex(Shift24NSuitInfo.SuitOk);
			debug_send("\r\n",2);
			send_word_hex(Shift5SuitInfo.SuitOk);
			debug_send("\r\n",2);
			send_word_hex(Shift5NNSuitInfo.SuitOk);
			debug_send("\r\n",2);
			send_word_hex(Shift6SuitInfo.SuitOk);

		}
		else if(CommandDeal("AAS",3)==1)
		{
			ControlInfo.SynTest=0x55;
		}
		else if(CommandDeal("MOTOR",5)==1)
		{
			if(Motor_Start_debug==0)
			{
				Motor_Start_debug = 1;
			}
			else
			{
				Motor_Start_debug = 0;
			}
		}
		else if(CommandDeal("CLC",3)==1)
		{
			diag.cleartroblecode = 1;
		}
		else if(CommandDeal("SCII",4)==1)
		{
			ShiftFData1Info.CurrentStep1 = 500;
			ShiftFData2Info.CurrentStep1 = 500;
			ShiftFData3Info.CurrentStep1 = 500;
			ShiftFData4Info.CurrentStep1 = 500;
			ShiftFData5Info.CurrentStep1 = 500;
			ShiftFData6Info.CurrentStep1 = 500;
			ShiftFDataRInfo.CurrentStep1 = 500;
			ShiftFData1Info.CurrentStep2 = 0;
			ShiftFData2Info.CurrentStep2 = 0;
			ShiftFData3Info.CurrentStep2 = 0;
			ShiftFData4Info.CurrentStep2 = 0;
			ShiftFData5Info.CurrentStep2 = 0;
			ShiftFData6Info.CurrentStep2 = 0;
			ShiftFDataRInfo.CurrentStep2 = 0;
		}
		else if(CommandDeal("XSLOPS",6)==1)
		{
			xSlopInfo.SuitFlag=SUITSTARTF;
		}
		else if(CommandDeal("RSTC",4)==1)
		{
			ControlInfo.Clutch1SuitFlag = 0;
			ControlInfo.Clutch2SuitFlag = 0;
			Write8bitsDataToDFlash(CLUTCH1INITSUITFLG,ControlInfo.Clutch1SuitFlag);
			Write8bitsDataToDFlash(CLUTCH2INITSUITFLG,ControlInfo.Clutch2SuitFlag);
		}
		else if(CommandDeal("GETCI",5)==1)
		{
			debug_send("\r\n",2);
			send_word_hex(Clutch1.PressZero);
			debug_send(" ",1);
			send_word_hex(Clutch1.Press);
			debug_send(" ",1);
			send_word_hex(Clutch1.InitPress);
			debug_send(" ",1);
			send_word_hex(Clutch1.ApartPress);
			debug_send("\r\n",2);
			send_word_hex(Clutch2.PressZero);
			debug_send(" ",1);
			send_word_hex(Clutch2.Press);
			debug_send(" ",1);
			send_word_hex(Clutch2.InitPress);
			debug_send(" ",1);
			send_word_hex(Clutch2.ApartPress);
		}
		else if(CommandDeal("CCCT",4)==1)
		{
			Clutch1.SuitFlag = 0x55;
		}
		else if(CommandDeal("GETCF",5)==1)
		{
			debug_send("\r\n",2);
			send_word_hex(Clutch1.FillCurrent);
			debug_send(" ",1);
			send_word_hex(Clutch2.FillCurrent);
		}
		else if(CommandDeal("GETERRO",7)==1)
		{
			debug_send("\r\n",2);
			send_word_hex(ErroInfo.Even.EvenOk);
			debug_send("\r\n",2);
			send_word_hex(ErroInfo.Odd.OddOk);
			debug_send("\r\n",2);
			send_word_hex(ErroInfo.OddAndEven.OddAndEvenOk);
			debug_send("\r\n",2);
			send_word_hex(ErroInfo.OddAndEven.OddAndEvenBits.OddShaftOk);
		}
		else if(CommandDeal("ODDERR",6)==1)
		{
			OddErroTest=1;
		}
		else if(CommandDeal("EVENERR",6)==1)
		{
			EvenErroTest=1;
		}
		else if(CommandDeal("ODDOK",5)==1)
		{
			OddErroTest=0;
		}
		else if(CommandDeal("EVENOK",5)==1)
		{
			EvenErroTest=0;
		}
 		/*行车记录仪命令*/
		else if(CommandDeal("DBG E",5)==1)
		{
			DiagSendStop = 1;//0;
			SendStop = 8;
		}
		else if(CommandDeal("DBG 0",5)==1)
		{
			DiagSendStop = 1;
			SendStop = 7;
		}
 		/**********/
 		/*CAN通讯调试*/
		else if(CommandDeal("GETID10D",sizeof("GETID10D")-1)==1)
		{
			debug_send("\r\n",2);
			for(i=0;i<8;i++)
			{
				send_word_hex(Can1RxBuffer.bits.ID_10D_Buffer.bytes[i]);
				debug_send("  ",2);
			}
			debug_send("\r\nEngS=",sizeof("\r\nEngS=")-1);
			send_word_hex(ENGINE_SPEED);
			debug_send("\r\nTEngS=",sizeof("\r\nTEngS=")-1);
			send_word_hex(TARGETIDLESPEED);
			debug_send("\r\nVehS=",sizeof("\r\nVehS=")-1);
			send_word_hex(VEHICLESPEED_ECU);
			debug_send("\r\nEngSE=",sizeof("\r\nEngSE=")-1);
			send_word_hex(ENGINE_ERRO);
			debug_send("\r\nIEngSS=",sizeof("\r\nIEngSS=")-1);
			send_word_hex(IDLES_SPEED_STATUS);
			debug_send("\r\nVehSE=",sizeof("\r\nVehSE=")-1);
			send_word_hex(VEHICLESPEED_ECU_ERRO);
			debug_send("\r\nES=",sizeof("\r\nES=")-1);
			send_word_hex(ENGINE_STATUS);
			debug_send("\r\nFC=",sizeof("\r\nFC=")-1);
			send_word_hex(FUEL_CUT_FLAG);
			debug_send("\r\nBK=",sizeof("\r\nBK=")-1);
			send_word_hex(BRAKE_POSITION);
		}
		else if(CommandDealSetV("N93",3,&ControlInfo.N93CurrentSet)==1)
		{
			debug_send("\r\n",2);
			send_word_Dec(ControlInfo.N93CurrentSet);
		}
		else if(CommandDealSetV("N94",3,&ControlInfo.N94CurrentSet)==1)
		{
			debug_send("\r\n",2);
			send_word_Dec(ControlInfo.N94CurrentSet);
		}
		else if(CommandDealSetV("N95",3,&ControlInfo.N95CurrentSet)==1)
		{
			debug_send("\r\n",2);
			send_word_Dec(ControlInfo.N95CurrentSet);
		}
		else if(CommandDealSetV("PUMPSET",7,&ControlInfo.PumpDutySet)==1)
		{
			debug_send("\r\n",2);
			send_word_Dec(ControlInfo.PumpDutySet);
		}
		else if(CommandDeal("GETID342",sizeof("GETID342")-1)==1)
		{
			debug_send("\r\n",2);
			for(i=0;i<8;i++)
			{
				send_word_hex(Can1RxBuffer.bits.ID_342_Buffer.bytes[i]);
				debug_send("  ",2);
			}

			debug_send("\r\nACC=",sizeof("\r\nACC=")-1);
			send_word_hex(ACCPEDALPOSITION_MG);
			debug_send("\r\nACCE=",sizeof("\r\nACCE=")-1);
			send_word_hex(ACCPEDALPOSVALID_MG);
			debug_send("\r\nBK=",sizeof("\r\nBK=")-1);
			send_word_hex(BRAKEPEDALPOSITION_MG);
			debug_send("\r\nBKE=",sizeof("\r\nBKE=")-1);
			send_word_hex(BRAKEPEDALPOSVALID_MG);
			debug_send("\r\nWKM=",sizeof("\r\nWKM=")-1);
			send_word_hex(VEHICLEWORKINGMODE);
			debug_send("\r\nRNM=",sizeof("\r\nRNM=")-1);
			send_word_hex(VEHICLERUNNINGMODE);
			debug_send("\r\nHP=",sizeof("\r\nHP=")-1);
			send_word_hex(HANDLEPOSITION);
		}
		else if(CommandDeal("MOVEON",sizeof("MOVEON")-1)==1)
		{
			debug_send("\r\nStart MoveOn!",sizeof("\r\nStart MoveOn!")-1);
			TelecontrolFlag = 4;
			TeleControlDebug = 1;
		}
		else if(CommandDeal("MOVEBACK",sizeof("MOVEBACK")-1)==1)
		{
			debug_send("\r\nStart MoveBack!",sizeof("\r\nStart MoveBack!")-1);
			TelecontrolFlag = 5;
			TeleControlDebug = 1;
		}
		else if(CommandDeal("EXITTELE",sizeof("EXITTELE")-1)==1)
		{
			debug_send("\r\nExit TeleControl!",sizeof("\r\nExit TeleControl!")-1);
			TelecontrolFlag = 0;
			TeleControlDebug = 0;
		}
		else if(CommandDeal("TC+",sizeof("TC+")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid = 1;
			send_word_Dec(ControlInfo.GenerateElecFlagValid);
		}
		else if(CommandDeal("TC-",sizeof("TC-")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid = 0;
			if(ControlInfo.GenerateElecFlagValid<0)
				ControlInfo.GenerateElecFlagValid = 0;
			send_word_Dec(ControlInfo.GenerateElecFlagValid);
		}
		else if(CommandDeal("TG+",sizeof("TG+")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid1 = 1;
			send_word_Dec(ControlInfo.GenerateElecFlagValid1);
		}
		else if(CommandDeal("TG-",sizeof("TG-")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid1 = 0;
			if(ControlInfo.GenerateElecFlagValid1<0)
				ControlInfo.GenerateElecFlagValid1 = 0;
			send_word_Dec(ControlInfo.GenerateElecFlagValid1);
		}
		else if(CommandDeal("TE+",sizeof("TE+")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid2 += 10;
			send_word_Dec(ControlInfo.GenerateElecFlagValid2);
		}
		else if(CommandDeal("TE-",sizeof("TE-")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid2 -= 10;
			if(ControlInfo.GenerateElecFlagValid2<0)
				ControlInfo.GenerateElecFlagValid2 = 0;
			send_word_Dec(ControlInfo.GenerateElecFlagValid2);
		}
		else if(CommandDeal("TV+",sizeof("TV+")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid3 += 10;
			send_word_Dec(ControlInfo.GenerateElecFlagValid3);
		}
		else if(CommandDeal("TV-",sizeof("TV-")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid3 -= 10;
			if(ControlInfo.GenerateElecFlagValid3<0)
				ControlInfo.GenerateElecFlagValid3 = 0;
			send_word_Dec(ControlInfo.GenerateElecFlagValid3);
		}
		else if(CommandDeal("VO",sizeof("VO")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid4 = 1;
			send_word_Dec(ControlInfo.GenerateElecFlagValid4);
		}
		else if(CommandDeal("VF",sizeof("VF")-1)==1)
		{
			debug_send("\r\n",2);
			ControlInfo.GenerateElecFlagValid4 = 0;
			if(ControlInfo.GenerateElecFlagValid4<0)
				ControlInfo.GenerateElecFlagValid4 = 0;
			send_word_Dec(ControlInfo.GenerateElecFlagValid4);
		}
		/*else if(CommandDealSetV("DG",2,&ControlInfo.DebugTargetGear)==1)
		{
			debug_send("\r\n",2);
			send_word_Dec(ControlInfo.DebugTargetGear);
		}
		else if(CommandDealSetV("TG1",3,&ControlInfo.TargetTorTG1)==1)
		{
			debug_send("TG1\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG1);
		}
		else if(CommandDealSetV("TG2",3,&ControlInfo.TargetTorTG2)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG2);
		}
		else if(CommandDealSetV("TG3",3,&ControlInfo.TargetTorTG3)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG3);
		}
		else if(CommandDealSetV("TG4",3,&ControlInfo.TargetTorTG4)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG4);
		}
		else if(CommandDealSetV("TG5",3,&ControlInfo.TargetTorTG5)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG5);
		}
		else if(CommandDealSetV("TG6",3,&ControlInfo.TargetTorTG6)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.TargetTorTG6);
		}
		else if(CommandDealSetV("TG7",3,&ControlInfo.DTTorque)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.DTTorque);
		}
		else if(CommandDealSetV("TG8",3,&ControlInfo.OilpumpDone)==1)
		{
			debug_send("\r\n",5);
			send_word_Dec(ControlInfo.OilpumpDone);
		}*/
#endif
 		/************/
	 	else
	 	{
	 	    debug_send("\r\n Command Error!\r\n",19);
	 	}
	  	while( debug_rx_buff[debug_rx_first]!=0x0d)
	  	{
	   		debug_rx_first++;
	  	}

	 	if(debug_rx_buff[debug_rx_first]==0x0d)
	  	{
	   		debug_rx_first++; 
	  	}

	  	debug_send("\r\n",2);

	 	command--;
	}
}
 

/*************************************************************************
* 函数名称：SendDebugInfo
* 功能描述：调试信息发送函数
* 函数说明：
* 调用函数：                               
* 全局变量：
* 输入参数：                     
* 返回参数：
* 设计人名：陈胜波		                     
* 修改人名：                                 
* 创建日期：2010-11-5
* 修改日期：
**************************************************************************/
void SendDebugInfo(void)
{
#if 0
	debug_send("\r\nC=",sizeof("\r\nC=")-1);
	send_word_hex(ControlInfo.CurrentGear);
	debug_send(" R=",sizeof(" R=")-1);
	send_word_hex(ControlInfo.TargetGear);
	debug_send(" B=",sizeof(" B=")-1);
	send_word_hex(ControlInfo.Brake);
	debug_send(" E=",sizeof(" E=")-1);
	send_word_hex(ControlInfo.EnginSpeed);
	debug_send(" TE=",sizeof(" TE=")-1);
	send_word_hex(ControlInfo.TargetEnginSpeed);
	debug_send(" 1A=",sizeof(" 1A=")-1);
	send_word_hex(Clutch1.ShaftSpeed);
	debug_send(" 2A=",sizeof(" 2A=")-1);
	send_word_hex(Clutch2.ShaftSpeed);
	debug_send(" VS=",sizeof(" VS=")-1);
	send_word_hex(ControlInfo.VehSpeed);
	debug_send(" AC=",sizeof(" AC=")-1);
	send_word_hex(ControlInfo.AccValue);

	debug_send(" F13=",sizeof(" F13=")-1);
	send_word_hex(Shift13.Position);
	debug_send(" F24=",sizeof(" F24=")-1);
	send_word_hex(Shift24.Position);
	debug_send(" F5N=",sizeof(" F5N=")-1);
	send_word_hex(Shift5n.Position);
	debug_send(" F6R=",sizeof(" F6R=")-1);
	send_word_hex(Shift6r.Position);

	debug_send(" 1C=",sizeof(" 1C=")-1);
	send_word_hex(Clutch1.Press);
	debug_send(" 1T=",sizeof(" 1T=")-1);
	send_word_hex(Clutch1.TargetPress);
	//send_word_hex(Clutch1.Pressa);
	debug_send(" 2C=",sizeof(" 2C=")-1);
	send_word_hex(Clutch2.Press);
	debug_send(" 2T=",sizeof(" 2T=")-1);
	send_word_hex(Clutch2.TargetPress);
	//send_word_hex(Clutch2.Pressa);

	debug_send(" RE=",sizeof(" RE=")-1);
	send_word_hex(ControlInfo.rEnginTorque);
	debug_send(" C1T=",sizeof(" C1T=")-1);
	//send_word_hex(Clutch1.TargetTorque);
	send_word_hex(Clutch1.SetCurrent);
	//send_word_hex(Clutch1.VehSpeedToShaft);
	//send_word_hex(ControlInfo.Safe1CurrentSet);
	debug_send(" C2T=",sizeof(" C2T=")-1);
	//send_word_hex(Clutch2.TargetTorque);
	send_word_hex(Clutch2.SetCurrent);
	//send_word_hex(Clutch2.VehSpeedToShaft);
	//send_word_hex(ControlInfo.Safe2CurrentSet);
	debug_send(" LD=",sizeof(" LD=")-1);
	//send_word_hex(ControlInfo.rEnginTorquea);
	//send_word_hex(ControlInfo.TcuTorque);
	send_word_hex(ControlInfo.MainCurrentSet);
	debug_send(" OD=",sizeof(" OD=")-1);
	send_word_hex(ControlInfo.ClutchTemperature);
	//send_word_hex(ControlInfo.SlipFlag);

	#if 0
	if(Shift13.TargetStatus!=Shift13.CurrentStatus
		||Shift5n.TargetStatus!=Shift5n.CurrentStatus
		||Shift24.TargetStatus!=Shift24.CurrentStatus
		||Shift6r.TargetStatus!=Shift6r.CurrentStatus)
	{
		debug_send(" F1=",sizeof(" F1=")-1);
		send_word_hex(ControlInfo.Safe1CurrentSet);
		debug_send(" F2=",sizeof(" F2=")-1);
		send_word_hex(ControlInfo.Safe2CurrentSet);
	}
	else
	{
		if(ControlInfo.TargetGear<ControlInfo.CurrentGear)
		{
			debug_send(" SC=",sizeof(" SC=")-1);
			send_word_hex(ControlInfo.DownClutchSwitchTime);
			debug_send(" SI=",sizeof(" SI=")-1);
			send_word_hex(ControlInfo.DownInertiaSynTime);
		}
		else
		{
			debug_send(" SC=",sizeof(" SC=")-1);
			send_word_hex(ControlInfo.UpClutchSwitchTime);
			debug_send(" SI=",sizeof(" SI=")-1);
			send_word_hex(ControlInfo.UpInertiaSynTime);
		}
	}
	#endif
	debug_send(" P=",sizeof(" P=")-1);
	send_word_hex(ClutchSlipData.OutPut);
#endif
}
void DebugSend(unsigned char buf[])
{
    unsigned char i;
 	for(i=0;i<8;i++)
   	{
    	*( debug_tx_buff_777 + (unsigned char)(debug_tx_last_777+i) ) = buf[i];
   	}
 	debug_tx_last_777 += 8;
}

void SendDebugInfo777(void)
{
    unsigned char Buf[8];
    ToothedWave(0, Buf, &cnt0, &cnt0_flag, 2, 5000);
    DebugSend(Buf);
    ToothedWave(1, Buf, &cnt1, &cnt1_flag, 4, 5000);
    DebugSend(Buf);
    ToothedWave(2, Buf, &cnt2, &cnt2_flag, 16, 5000);
	DebugSend(Buf);
	ToothedWave(3, Buf, &cnt3, &cnt3_flag, 20, 5000);
	DebugSend(Buf);
	SqureWave(4, Buf, &cnt4, &cnt4_flag, 1, 5000);
	DebugSend(Buf);
	ObliqueWave(5, Buf, &cnt5, 20, 5000);
	DebugSend(Buf);
	SinWave(6, Buf, &cnt6, 1, 5000);
	DebugSend(Buf);
    frame_index += 1;
}

void ToothedWave(unsigned char num, unsigned char *buf, unsigned short *cnt, unsigned char *cnt_flag, short seq, short range)
{
	buf[0] = num;
	buf[1] = 0;
	if (*cnt_flag)
	{
		*cnt += seq;
	}
	else
	{
		*cnt -= seq;
	}
	if(*cnt >= range - 40)
	{
		*cnt_flag = 0;
	} else if (*cnt <= 40)
	{
		*cnt_flag = 1;
	}
	buf[2] = (unsigned char) ((*cnt) & 0x00ff);
	buf[3] = (unsigned char) ((*cnt) >> 8);
	buf[4] = (unsigned char) ((frame_index & 0x000000ff) >> 0);
	buf[5] = (unsigned char) ((frame_index & 0x0000ff00) >> 8);
	buf[6] = (unsigned char) ((frame_index & 0x00ff0000) >> 16);
	buf[7] = (unsigned char) ((frame_index & 0xff000000) >> 24);
}

void SqureWave(unsigned char num, unsigned char *buf, unsigned short *cnt, unsigned char *cnt_flag, short seq, short range)
{
	buf[0] = num;
	buf[1] = 0;
	if (*cnt_flag > 128)
	{
		*cnt = range - 50;
	}
	else
	{
		*cnt = range/4*3 + 50;
	}
	*cnt_flag += seq;
	*cnt_flag %= 0xff;
	buf[2] = (unsigned char) ((*cnt) & 0x00ff);
	buf[3] = (unsigned char) ((*cnt) >> 8);
	buf[4] = (unsigned char) ((frame_index & 0x000000ff) >> 0);
	buf[5] = (unsigned char) ((frame_index & 0x0000ff00) >> 8);
	buf[6] = (unsigned char) ((frame_index & 0x00ff0000) >> 16);
	buf[7] = (unsigned char) ((frame_index & 0xff000000) >> 24);
}
void ObliqueWave(unsigned char num, unsigned char *buf, unsigned short *cnt, short seq, short range)
{
	buf[0] = num;
	buf[1] = 0;
	if (*cnt >= range)
	{
		*cnt = 0;
	}
	else
	{
		*cnt += seq;
	}
	buf[2] = (unsigned char) ((*cnt) & 0x00ff);
	buf[3] = (unsigned char) ((*cnt) >> 8);
	buf[4] = (unsigned char) ((frame_index & 0x000000ff) >> 0);
	buf[5] = (unsigned char) ((frame_index & 0x0000ff00) >> 8);
	buf[6] = (unsigned char) ((frame_index & 0x00ff0000) >> 16);
	buf[7] = (unsigned char) ((frame_index & 0xff000000) >> 24);
}

void SinWave(unsigned char num, unsigned char *buf, unsigned short *cnt, short seq, short range)
{
	buf[0] = num;
	buf[1] = 0;
	*cnt += seq;
	int result = range/4 + (int)((range/4 - 40) * sin(*cnt * 0.1));
	buf[2] = (unsigned char) (result & 0x00ff);
	buf[3] = (unsigned char) (result >> 8);
	buf[4] = (unsigned char) ((frame_index & 0x000000ff) >> 0);
	buf[5] = (unsigned char) ((frame_index & 0x0000ff00) >> 8);
	buf[6] = (unsigned char) ((frame_index & 0x00ff0000) >> 16);
	buf[7] = (unsigned char) ((frame_index & 0xff000000) >> 24);
}
