
#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG_PROCESS    1
/*定义为0选用UART,否则选用CAN*/
#define UART_CAN_SELECT  1
#define SELECT_UART      0
#define SELECT_CAN       1

#define	CAPTURE_ENBLE 		5
#define	ECM_TYPE 		1

extern volatile unsigned char debug_tx_buff[256];
extern volatile unsigned char debug_rx_buff[256];
extern volatile unsigned char debug_tx_buff_777[256];
extern volatile unsigned char debug_data[8];
extern volatile unsigned char debug_tx_first;
extern volatile unsigned char debug_tx_last;
extern volatile unsigned char debug_rx_first;
extern volatile unsigned char debug_rx_last;
extern volatile unsigned char debug_tx_first_777;
extern volatile unsigned char debug_tx_last_777;
extern volatile unsigned char command;
extern volatile unsigned char debug_rx_flag;
extern volatile unsigned char debug_len;
extern volatile unsigned char SendStop;
extern unsigned char debug_axil1err;

extern volatile unsigned char Motor_Start_debug;	//冷却电机测试命令

void SendDebugInfo777(void);

void ToothedWave(unsigned char num, unsigned char *buf, unsigned short *cnt, unsigned char *cnt_flag, short seq, short range);
void SqureWave(unsigned char num, unsigned char *buf, unsigned short *cnt, unsigned char *cnt_flag, short seq, short range);
void ObliqueWave(unsigned char num, unsigned char *buf, unsigned short *cnt, short seq, short range);
void SinWave(unsigned char num, unsigned char *buf, unsigned short *cnt, short seq, short range);

unsigned char ASCII_Digit(unsigned char temp);
void debug_send(char *pt,unsigned char length);
void SendHex(signed short a);
void send_dword_hex(unsigned long a);
void send_word_hex(unsigned short a);
void MyDebug(void);
void send_word_Dec_Byte(unsigned char a);
void SendDebugInfo(void);
void SendDebugInfo1(void);
void send_word_Dec_4(unsigned short a) ;
void SendDebugInfo_Gear(void);
void SendDebugPresSycInfo(void);
unsigned char CommandDeal(char *pt,unsigned char length);
void Debug_rx_handle(void);

#endif
