/*------------------------------------------------------------------------------
  Project Title : Header file for serial communication between microcontroller to PC and vice versa  
  Designer      : "Elegant Embedded Solutions" Hyderabad OpenSource v1.0
  Controller    : 8051 Microcontroller
  Software      : keil3
 ------------------------------------------------------------------------------*/

// #include<reg51.h>

void UART_init(void);						/* Function to initialize UART */
void send_to_modem (unsigned char s[]);				/* Function to transmit string to Modem */
void enter (void);						/* Function to send ENTER command */
void ch_send_to_modem (unsigned char single_char);		/* Function to transmit single character to Modem */


/* Function to produce delay */
			  
void delay1(unsigned int value)
{
 unsigned int x,y;
 for(x=0;x<100;x++)
 for(y=0;y<value;y++);
} 

/* Function to Initialize UART */

void UART_init(void)
{
 TMOD=0x20;	          					/* TIMER 1,  8-BIT AUTO RELOAD*/
 TH1=0xFD;			  				/* Set BAUD RATE 9600*/
 SCON=0X50;			  				/* SERIAL COUNTER REGISTER IS LOADED WITH 50H INDICATING SERIAL MODE 1 */
                                                              	/* WHERE 8 BIT DATA IS FRAMED WITH START AND STOP BITS */
 TR1=1; 		   	  				/* START TIMER */
}

/* Function to send string to modem */

void send_to_modem (unsigned char s[])
{
 unsigned char r;
 for(r=0;s[r]!='\0';r++)	 				/* For loop till the array charecter is null character */
 {							  
  ch_send_to_modem(s[r]);					/* send the respective character of the string */
  delay1(20); 
 }
}

/* Function to send Enter */

void enter (void)											  
{
 SBUF=13;		  					/* send 13 as \r , it is the carriage return*/
 while(TI==0);	   						/* wait till the character is transimitted */ 
 TI=0;
 SBUF=10;							/* send 10 as \n , it is the newline character*/
 while(TI==0);							/* wait till the character is transimitted */ 
 TI=0;
}

/* Function to send single Charecter to Modem */

void ch_send_to_modem (unsigned char single_char)
{
 SBUF=single_char;						/* send single charecter */
 while(TI==0); 							/* wait till the character is transimitted */
 TI=0;
 delay1(20);
}																										   
