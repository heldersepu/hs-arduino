#ifndef TFT_h
#define TFT_h

#define DCLK     6
#define CS       5
#define DIN      4
#define DOUT     3
#define IRQ      2

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

unsigned int TP_X, TP_Y;

#endif

void WriteCharTo7843(unsigned char num)
{
    unsigned char count=0;
    unsigned char temp;
    temp=num;
    digitalWrite(DCLK,LOW);
    for(count=0;count<8;count++)
    {
        if(temp&0x80)
            digitalWrite(DIN,HIGH);
        else
            digitalWrite(DIN,LOW);
        temp=temp<<1;
        digitalWrite(DCLK,LOW);
        digitalWrite(DCLK,HIGH);
    }
}

unsigned int ReadFromChar(unsigned char cnum)
{
    WriteCharTo7843(cnum);
    digitalWrite(DCLK,HIGH);
    digitalWrite(DCLK,LOW);
    unsigned char count=0;
    unsigned int Num=0;
    for(count=0;count<12;count++)
    {
        Num<<=1;
        digitalWrite(DCLK,HIGH);
        digitalWrite(DCLK,LOW);
        if(digitalRead(DOUT)) Num++;
    }
    return(Num);
}



