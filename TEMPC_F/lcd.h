/*------------------------------------------------------------------------------
  Project Title : LCD Display header file
  PIN  Details  : data pins are connected from P1.4 to P1.7
                  Enable pin = P1.3  and reset = P1.2   
  Designer      : "Elegant Embedded Solutions" Hyderabad 
  Controller    : 8051 Microcontroller
  Software      : keil3
 ------------------------------------------------------------------------------*/


#define lcd_data P2							/* LCD DATA TO PORT 1 */

sbit  lcd_rs   =  P2^0;		 			    /* LCD control pin RS P1.2 */
sbit  lcd_en   =  P2^1;						/* LCD control pin EN P1.3 */
 
void lcd_init(void);    					/* Function to initialize LCD */
void lcdcmd(unsigned char value);			/* Function to send command to LCD */
void lcddata(unsigned char value);			/* Function to send data to LCD */
void msgdisplay(unsigned char b[]);			/* Function to send string to LCD */
void delay(unsigned int value);	            /* This function produces a delay in msec.*/               

void lcd_init(void)
{
 lcdcmd(0x02);                           
 lcdcmd(0x28);							    /* for using 4-bit 2 row mode of LCD */
 lcdcmd(0x28);
 lcdcmd(0x0e);	                            /* turn display ON for cursor blinking	*/
 lcdcmd(0x06);							    /* move the cursor to right side */
 lcdcmd(0x01);							    /* clear LCD */
}

void lcdcmd(unsigned char value)		    /* Function to send command to LCD */
{
 lcd_data=value&(0xf0);                     /* send msb 4 bits */
 lcd_rs=0;	                                /* select command register */
 lcd_en=1;	                                /* enable the lcd to execute command */
 delay(3);
 lcd_en=0;  
 lcd_data=((value<<4)&(0xf0));	            /* send lsb 4 bits */
 lcd_rs=0;	                                /* select command register */
 lcd_en=1;	                                /* enable the lcd to execute command */
 delay(3);
 lcd_en=0;  
}  
	
void lcddata(unsigned char value)		    /*Function to send Data to LCD  */
{
 lcd_data=value&(0xf0);                     /* send msb 4 bits */
 lcd_rs=1;	                                /* select command register */
 lcd_en=1;	                                /* enable the lcd to execute command */
 delay(3);
 lcd_en=0;  
 lcd_data=((value<<4)&(0xf0));	            /* send lsb 4 bits */
 lcd_rs=1;	                                /* select command register */
 lcd_en=1;	                                /* enable the lcd to execute command */
 delay(3);
 lcd_en=0;  
}  

void msgdisplay(unsigned char b[])          /* Function to send string to lcd	*/
{
 unsigned char s,count=0;
 for(s=0;b[s]!='\0';s++)				 
 lcddata(b[s]);							    /* send data to lcd till the end of string */
}
	

void delay(unsigned int value)	            /* This function produces a delay in msec.*/
{
  unsigned int i,j;						    /* Delay var */
  for(i=0;i<=value;i++)						  
  for(j=0;j<=200;j++);				  	    /* Delay for 500 Counts */
}