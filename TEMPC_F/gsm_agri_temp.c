#include<reg51.h>
#include<intrins.h>
#include<string.h>
#include"uart.h"
#include"lcd.h"
#define fac 1.8

sbit dq = P3^5; // connect with DS1820 Data pin

bit l1,l2,l3;
  
unsigned char mobilenum[20],msg[40];
unsigned  char numcnt,b11,XX,newmsg=0,v,c,temp;
unsigned int i,k;
void readmsg(void); 
void sendmsg(int);
//void LCD_conv(unsigned char );
void delay_ms(int j)
{
  unsigned char i;

  for(;j;j--)
    for(i=122;i<=0;i--);
}
void delayus(int us)
{
  int i;
  for (i=0; i<us; i++);
}
bit reset(void)
{
  bit presence;
  dq = 0;
  delayus(29);
  dq = 1;
  delayus(3);
  presence = dq;
  delayus(25);
  return(presence);
}

bit readbit(void)
{
  unsigned char i=0;
  dq = 0;
  dq=1;
  for (i=0; i < 3; i++);
  return(dq);
}


void writebit(bit Dbit)
{
  unsigned char i=0;
  dq=0;
  dq = Dbit?1:0;
  delayus(5);
  dq = 1;
}

unsigned char readbyte(void)
{
  unsigned char i;
  unsigned char din = 0;
  for (i=0;i<8;i++)
  {
    din|=readbit()? 0x01<<i:din;
    delayus(6);
  }
  return(din);
}

void writebyte(unsigned char dout)
{
  unsigned char i;
  for (i=0; i<8; i++)
  {
    writebit((bit)(dout & 0x1));
    dout = dout >> 1;
  }
  delayus(5);
}

unsigned char * ReadTemp()
{
  unsigned char n;
  unsigned char buff[2]=0;
  reset();

  writebyte(0xcc);
  writebyte(0x44);

  while (readbyte()==0xff);
  delay_ms(500);

  reset();
  writebyte(0xcc);
  writebyte(0xbe);

  for (n=0; n<9; n++)
    buff[n]=readbyte();
	
  return buff;
}

void gsm_conv(unsigned char h)
{
  unsigned char temp,x,y,x1,y1;
  temp=h;
  x=temp/100;						
  y=temp%100;
  x1=y/10;
  y1=y%10;
  x=x|0x30;
  x1=x1|0x30;
  y1=y1|0x30;
  ch_send_to_modem(x);
  ch_send_to_modem(x1);
  ch_send_to_modem(y1);
  delay(500); 
}
/*void gsm_conv(unsigned char h1)
{
  unsigned char temp,x,y,x1,y1;
  temp=h1;
  x=temp/100;						
  y=temp%100;
  x1=y/10;
  y1=y%10;
  x=x|0x30;
  x1=x1|0x30;
  y1=y1|0x30;
  ch_send_to_modem(x);
  ch_send_to_modem(x1);
  ch_send_to_modem(y1);
  ch_send_to_modem(223);	
  ch_send_to_modem('F');
  delay(500);
}*/	  
void LCD_conv(unsigned char g)
{  
  unsigned char temp;
  temp=g;
  //x=temp/100;						
  //y=temp%100;
  //x1=y/10;
  //y1=y%10;
  //x=x|0x30;
  //x1=x1|0x30;
  //y1=y1|0x30;

  delay(10); 
  lcddata((temp/1000)|0x30);
  lcddata(((temp/100)%10)|0x30);
  lcddata(((temp/10)%10)|0x30);

  lcddata((temp%10)|0x30);

  //delay(100); 
}

/*** serial interrupt function to recieve the data from gsm modem****/
void serintr(void) interrupt 4
{
  if(RI==1)
  {  	         
    XX=SBUF;					
    if(XX=='+')
      newmsg=1;
    RI=0;		
  }
}

unsigned char tp;
unsigned int tf;

void main()
{ 
   unsigned char msgtype=0; 
   unsigned char *temp,t=0x00;
   //float b;
   //int f;

  l1=l2=1;
  l3=0;

  lcd_init();	           //intialization of lcd
  lcd_init();	           //intialization of lcd
  UART_init();
  msgdisplay("Welcome");	   //display string on lcd of lcd
  lcdcmd(0xc0);			   // move the cursor to 0xc0 location on lcd
  delay(1000); 
  lcdcmd(0x01);			   
  msgdisplay("Checking for");
  lcdcmd(0xc0);
  msgdisplay("GSM modem");
  delay(300); 
  send_to_modem("AT&F");   //to avoid echo signals,
  enter();
  delay(500);
  
  send_to_modem("ATE0");   //to avoid echo signals,
  enter();
  delay(500);
  RI=0;

  send_to_modem("AT");  // TO CHECKING GSM MODEM...
  enter();
  delay(1000);
  //if(!RI)              // Here we are waiting for data whitch is sending by GSM modem  
  // goto again;

  while(RI==0);
    RI=0;
     
    EA=1;
    ES=1;
 
  lcdcmd(0x01);
  msgdisplay("SYSTEM");
  lcdcmd(0xc3); 
  msgdisplay("CONNECTED");
  delay(100);
  send_to_modem("AT+CREG=0");  // 
  enter();
  delay(1000);
  newmsg=0;
	 
  lcdcmd(0x01);
  msgdisplay("CHEKING SIM");
  send_to_modem("AT+CPIN?");  
  enter();
  delay(1500); 

  while(newmsg==0); 	 
  
  lcdcmd(0xC0);
  msgdisplay("SIM CONNECTED");
  delay(500);
	 send_to_modem("AT+CNMI=1,1,0,0");  // tr set message format astext mode
     enter();
   	 delay(700);
	 send_to_modem("AT+CREG=0");  // tr set message format astext mode
     enter();
   	 delay(700);
	 send_to_modem("AT+CMGF=1");  // to set message format astext mode
     enter();
   	 delay(100);
	 send_to_modem("AT+CMGD=1");  // to set message format astext mode
     enter();
   	 delay(100);
	 send_to_modem("AT+CMGD=2");  // to set message format astext mode
     enter();
	 delay(100);
   	 send_to_modem("AT+CMGD=3");  // to set message format astext mode
     enter();
   	 delay(100);
	
	 newmsg=0;
	
     delay(500);
	 ES=0;

  start:
  delay(500);
  lcdcmd(0x01);

   //delay(500);
   ES=0;
  newmsg=0;
  
  ES=1;
   
  while(newmsg==0)
  {	
    temp=ReadTemp();

    temp[1]=temp[1]&0x07;

    tp=temp[0]>>4;
    temp[1]=temp[1]<<4;

    tp=tp+temp[1];
    tf=(tp*fac)+32;

    lcdcmd(0x80);
    delay(10); 
    msgdisplay("TEMP:"); LCD_conv(tp);
    lcddata(223);
    lcddata('c');



   
   


   lcdcmd(0xc0);
   delay(10); 
   msgdisplay("TEMP:");LCD_conv(tf);

   lcddata(223);
   lcddata('F');
   lcdcmd(0x02);    
 }  
 delay(500);
   	  
 ES=0;
 delay(100);
   
 readmsg();  //read message 
 lcdcmd(0x01);  
 msgdisplay(msg);	 //display message
 delay(90);	
   
 lcdcmd(0xc0);
 msgdisplay(mobilenum);
 delay(500);    
   
 lcdcmd(0x01);
 msgdisplay("Sendng Message....");
 
 if(!strcmp(msg,"GET TEMPC"))
 {
   // msgdisplay("TEMP:");
   msgtype=6;
   sendmsg(6);
 }
   
 else if(!strcmp(msg,"GET TEMPF"))
 {
   // msgdisplay("TEMP:");
   msgtype=7;
   sendmsg(7);
 }
  else;
  goto start;
}

void sendmsg(int msgtype)
{
  delay(1000);
  lcdcmd(0xc0);
  send_to_modem("AT+CMGS=");	//send messsage	sending command to GSM modem
  ch_send_to_modem('"');
  send_to_modem(mobilenum);	// send mobile number
  ch_send_to_modem('"');
  enter();				  //select message
  delay(500);
  if(msgtype==6)
  {
    send_to_modem("Temp_c:");
    delay(1000);
    gsm_conv(tp);
    send_to_modem("°c");
    enter();
    //delay(100);
    //gsm_conv(tf);
    //send_to_modem("°f");
    //enter();
    //delay(100);
  }
  if(msgtype==7)
  {
    send_to_modem("Temp_f:");
    delay(1000);
    //gsm_conv(tp);
    //send_to_modem("°c");
    // enter();
    //delay(100);
    gsm_conv(tf);
    send_to_modem("°f");
    enter();
    delay(100);
  } 								 
  else;	 
  delay(1500);
  ch_send_to_modem(0x1A);   //CONTROL Z
  delay(500);
  ch_send_to_modem(0x1A);
  ES=1;
  delay(2000);

  lcdcmd(0x01);
  msgdisplay("MESSAGE SENT");
  delay(1500);
  send_to_modem("AT+CMGD=1");
  enter();
  ES=0;
  delay(500);
		
			 
  delay(100);	 
  RI=0;
  lcdcmd(0xc0);
  msgdisplay("OK");	  	
} 

void readmsg(void)         
{
  unsigned char a9,b9,i9,count9,numcnt;
  
  delay(100);
  ES=1;
  delay(300);
  ES=0;
  
  while(RI==1)
  {
    RI=0;
    delay(100);
  }
  send_to_modem("AT+CMGR=1");
  enter();
 
  count9=0;
  i9=0;
  a9=0;
  numcnt=0;

  while(count9!=3)
  {
    while(RI==0);
    b9=SBUF;
    if((b9==',')||(a9==1))
    {
      if(numcnt<15)
      {
        if(numcnt>4)
	{
     	  mobilenum[numcnt-5]=b9;
	}
    	a9=1;
	numcnt++;
      } 
      else
        a9=0;
    }
      if(count9==2)
      {
        msg[i9++]=SBUF;
      }	    
        RI=0;
	if(b9==10)
	count9+=1;
  }
		      
    msg[--i9]='\0';
    msg[--i9]='\0';
    mobilenum[10]='\0';

    send_to_modem("AT+CMGD=1");
    enter();
    RI=0;	  	 
    delay(1000);
    RI=0;	  	 	 
}
