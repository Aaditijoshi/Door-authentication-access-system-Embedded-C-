//DOOR AUTHENTICATION SYSTEM

#include<LPC17xx.H>
void UART0port_init(void);
void UART1port_init(void);
void uart0_init(void);
char call(void);
//char call(char*data);
void rec(void);
void cmp(void);
void send(void);
void gsm_init(void);
void gsm_msgsend(void);
void delay (unsigned int x);
unsigned char a [4][9]={"03919218","0391215","03908817","03914867"};
unsigned char msg1[]="matched";
//unsigned char msg2[]="not matched";
char a1[10],s[9];
unsigned int val,m,n;
char b=0;
unsigned char i,j;
int main(void)
{
SystemInit();
UART0port_init();
UART1port_init();
gsm_init();
while(1)
  { 
    rec();	
    delay(1000);
    b=call();
    delay(10000);
    send();
    delay(1000);
  }
}
void rec(void)
{ char emp=0;
  for (i=0;i<=0x0A;i++)
  { 
    while((LPC_UART1->LSR & 0X01)!=0X01);
    emp=LPC_UART1->RBR;
    a1[i]=emp;
  }
 for(i=0;a1[i]=='\0';i++)
  {
   LPC_UART1->THR=a1[i];
    while ((LPC_UART1->LSR & 0X40)!=0X40);
  }
}
 char call()
{
 for(j=0;j<4;j++)
{
 n=0;
 for(i=0;i<8;i++)
 {
   if(a[j][i]==a1[i])
   {
     n++;
   }
  if(n==8)
 {
  return(1);
 }
}
return(0);
}
}
void send()
{
 j=0;
 if(b==1)
 { while (msg1[j]!='\0')
   {
     LPC_UART0->THR=msg1[j];
     while ((LPC_UART0->LSR & 0X40)!=0X40);
     j++;
   }
 }
//LPC_UART1->THR=0X0D;
//LPC_UART1->THR=0X0A;
else
{
	gsm_msgsend();
}
}
void UART0port_init(void)
{
  LPC_SC->PCONP|=0X00400008;
	LPC_PINCON->PINSEL0=0X00000050;
  LPC_UART0->LCR=0X83;
  LPC_UART0->DLL=0X75;
  LPC_UART0->FDR=0X10;
  LPC_UART0->FCR=0X07;
  LPC_UART0->LCR=0X03;
}
void UART1port_init(void)
{
 LPC_SC->PCONP|=0X02000000;
// LPC_PINCON->PINSEL9=0X0F000000;
LPC_PINCON->PINSEL0=0X40000000;
LPC_PINCON->PINSEL1=0X00000001;
 LPC_UART1->LCR=0X83;
 LPC_UART1->DLL=0X75;
 LPC_UART1->DLM=0X00;
 LPC_UART1->FDR=0X10;
 LPC_UART1->FCR=0X07;
 LPC_UART1->LCR=0X03;
}

void uart0_init(void)
{
LPC_SC->PCONP|=0x00000000;
LPC_SC->PCLKSEL0|=0x00000000;
LPC_PINCON->PINSEL0|=0x00000050;
LPC_UART0->LCR=0x00000083;
LPC_UART0->DLL=0x00000075;
LPC_UART0->DLM=0x00000000;
LPC_UART0->FDR=0x00000010;
LPC_UART0->LCR=0x00000003;
LPC_UART0->FCR=0x00000007;
}

void delay(unsigned int x)
{
 int i=0;
 for (i=0;i<x;i++);
}


void gsm_init(void)
{
unsigned char msg1[]="AT\r\n";
unsigned char msg2[]="AT+CREG?\r\n";
unsigned char msg3[]="AT+CMGF=1\r\n";
unsigned short int gs=0;

for(gs=0;msg1[gs]!='\0';gs++)
{
LPC_UART0->THR=msg1[gs];
while((LPC_UART0->LSR&0X20)!=0x20);
}
delay(0x60000);

 for(gs=0;msg2[gs]!='\0';gs++)
{
LPC_UART0->THR=msg2[gs];
while((LPC_UART0->LSR&0X20)!=0x20);
}
delay(0x60000);

for(gs=0;msg3[gs]!='\0';gs++)
{
LPC_UART0->THR=msg3[gs];
while((LPC_UART0->LSR&0X20)!=0x20);
}
delay(0x60000);
}

void gsm_msgsend(void)
{
unsigned short int gs=0;
unsigned char msg4[]="AT+CMGS=\"8123101045\"r\n";
unsigned char msg5[]="Door accessed by unauthorised persons";

for(gs=0;msg4[gs]!='\0';gs++)
{
LPC_UART0->THR=msg4[gs];
while((LPC_UART0->LSR&0x20)!=0x20);
}
delay(0x60000);

for(gs=0;msg5[gs]!='\0';gs++)
{
LPC_UART0->THR=msg5[gs];
while((LPC_UART0->LSR&0x20)!=0x20);
}
LPC_UART0->THR=0x1A; //Ascii value of ctrl Z
while((LPC_UART0->LSR&0x20)!=0x20); //sends an alert when the value is high
delay(0x60000);
}