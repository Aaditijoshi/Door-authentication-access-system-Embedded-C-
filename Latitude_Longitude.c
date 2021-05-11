#include<LPC17xx.H>
//Sends the latitude and longitude of a place using GPS technology and GSM module to a specific phone number

void gps_send(void);
void uart0_init(void);
void uart3_init(void);
void GSM_init(void);
void gsm_send(void);
void delay(unsigned long int n);

unsigned char rxdval;
unsigned char val;
unsigned char gpsdata[200];
unsigned char lat[]="LATITUDE OF THE PLACE IS:";
unsigned char lon[]="LONGITUDE OF THE PLACE IS:";
unsigned short int i;
unsigned lat1[20];
unsigned lon1[20];
int main()
{
	SystemInit();
	uart0_init();
	uart3_init();
	GSM_init();
	while(1)
	{
		while((LPC_UART3->LSR&0X01)!=0X01)
		{}
		rxdval= LPC_UART3->RBR;
		LPC_UART0->THR=rxdval;
    while((LPC_UART0->LSR&0X20)!=0X20);
		gps_send(); // sending gps data to mcu and mcu to pc
	}
}


void gps_send(void)
{
    do
		{
      while((LPC_UART3->LSR&0X01)!=0X01);
      val=LPC_UART3->RBR;
		}while(val!='$');

		for(i=0;i<100;i++)
		{
			while((LPC_UART3->LSR&0X01)!=0X01);
    	gpsdata[i]=	LPC_UART3->RBR;
		}
		if(gpsdata[2]=='R')
		{
			if(gpsdata[17]=='A')
			{
				for(i=19;i<29;i++)
				{
					lat1[i]=gpsdata[i];
					while((LPC_UART0->LSR&0X20)!=0X20);
				}
				for(i=31;i<42;i++)
				{
					lon1[i]=gpsdata[i];
					while((LPC_UART0->LSR&0X20)!=0X20);
				}

			}
			else;
		}
		else;
	}


void uart0_init(void) //UART 0 initializing
{
	LPC_SC->PCONP=0x00000000;//default power : initialized as high value
	LPC_PINCON->PINSEL0=0X00000050; // initializing the PINSEL register
	LPC_UART0->LCR=0X83; // line control register
	LPC_UART0->DLM=0X00; // Assuming it to be zero to find DLL
	LPC_UART0->DLL=0X75; // found considering baud rate =9600
	LPC_UART0->FDR=0X00000010; // FDR contains MUL VAL and DIV VAL, where MULVAL =1 and DIV VAL=0
	LPC_UART0->LCR=0x83;// line control register
}


void uart3_init(void)
{
	LPC_SC->PCONP=0x02000000;
	LPC_PINCON->PINSEL9=0X0f000000;
	LPC_UART0->LCR=0X83;
	LPC_UART0->DLM=0X00;
	LPC_UART0->DLL=0X75;
	LPC_UART0->FDR=0X00000010;
	LPC_UART0->LCR=0x83;
}

void GSM_init(void)
{
	unsigned char msg1[]="AT\r\n";
	unsigned char msg2[]="AT+CREG?\r\n";
	unsigned char msg3[]="AT+CMGF=1\r\n";


	for(i=0;msg1[i]!='\0';i++)
	{
		LPC_UART0->THR=msg1[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg2[i]!='\0';i++)
	{
		LPC_UART0->THR=msg2[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg3[i]!='\0';i++)
	{
		LPC_UART0->THR=msg3[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);
}


void message_send(void)
{
	unsigned char msg4[]="AT+CMGS=\"8123101045\"\r\n";


	for(i=0;msg4[i]!='\0';i++)
	{
		LPC_UART0->THR=msg4[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg4[i]!='\0';i++)
	{
		LPC_UART0->THR=lat[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg4[i]!='\0';i++)
	{
		LPC_UART0->THR=lat1[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg4[i]!='\0';i++)
	{
		LPC_UART0->THR=lon[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);


	for(i=0;msg4[i]!='\0';i++)
	{
		LPC_UART0->THR=lon1[i];
		while((LPC_UART0->LSR&0X20)!=0X20);
	}
	delay(50000);

	LPC_UART0->THR=0x0D;
	while((LPC_UART0->LSR&0X20)!=0X20);
	delay(50000);
}

void delay(unsigned long int n)
{ unsigned long int p;
	for(p=0;p<n;p++);
}
