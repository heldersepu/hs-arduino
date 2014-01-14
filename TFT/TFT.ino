#include <UTFT.h>

#define DCLK     6
#define CS       5
#define DIN      4
#define DOUT     3
#define IRQ      2



extern uint8_t SmallFont[];
extern uint8_t BigFont[];
UTFT myGLCD(ITDB32S, 38,39,40,41);
unsigned int TP_X, TP_Y;
long control = 0;
int x, y;

void dFrame()
{
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRect(0, 0, 319, 13);
    myGLCD.setColor(64, 64, 64);
    myGLCD.fillRect(0, 226, 319, 239);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.print("* EasyLights - 240x320 Color Display *", CENTER, 0);
    myGLCD.setColor(255, 128, 128);
    myGLCD.setBackColor(64, 64, 64);
    myGLCD.print("       ", LEFT,  227);
    myGLCD.print("       ", RIGHT, 227);
    myGLCD.setColor(255,255,0);
    myGLCD.print("EasyLights-Lumbre.com", CENTER, 227);
}

void dRunTime()
{
    myGLCD.fillScr(0, 0, 255);
    myGLCD.setColor(255, 0, 0);
    myGLCD.fillRoundRect(80, 70, 239, 169);

    myGLCD.setColor(255, 255, 255);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.print("That's it!", CENTER, 93);
    myGLCD.print("Restarting in a", CENTER, 119);
    myGLCD.print("few seconds...", CENTER, 132);

    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 255);
    myGLCD.print("Runtime: (msecs)", CENTER, 210);
    myGLCD.printNumI(millis(), CENTER, 225);
}

void dSinCosTan()
{
    // Draw crosshairs
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.drawLine(159, 15, 159, 224);
    myGLCD.drawLine(1, 119, 318, 119);
    for (int i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121);
    for (int i=19; i<220; i+=10)
    myGLCD.drawLine(157, i, 161, i);

    // Draw sin-, cos- and tan-lines
    myGLCD.setColor(0,255,255);
    myGLCD.print("Sin", 5, 15);
    for (int i=1; i<318; i++)
    {
        myGLCD.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
    }

    myGLCD.setColor(255,0,0);
    myGLCD.print("Cos", 5, 27);
    for (int i=1; i<318; i++)
    {
        myGLCD.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
    }

    myGLCD.setColor(255,255,0);
    myGLCD.print("Tan", 5, 39);
    for (int i=1; i<318; i++)
    {
        myGLCD.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
    }
}

void dMovingSin()
{
    int buf[318];
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);
    myGLCD.setColor(0, 0, 255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.drawLine(159, 15, 159, 224);
    myGLCD.drawLine(1, 119, 318, 119);

    // Draw a moving sinewave
    x=1;
    for (int i=1; i<(318*60); i++)
    {
        x++;
        if (x==319)
            x=1;
        if (i>319)
        {
            if ((x==159)||(buf[x-1]==119))
                myGLCD.setColor(0,0,255);
            else
                myGLCD.setColor(0,0,0);
            myGLCD.drawPixel(x,buf[x-1]);
        }
        myGLCD.setColor(0,255,255);
        y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
        myGLCD.drawPixel(x,y);
        buf[x-1]=y;
    }
}

void dFilledRect()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);

    // Draw some filled rectangles
    for (int i=1; i<6; i++)
    {
        switch (i)
        {
            case 1:
                myGLCD.setColor(255,0,255);
                break;
            case 2:
                myGLCD.setColor(255,0,0);
                break;
            case 3:
                myGLCD.setColor(0,255,0);
                break;
            case 4:
                myGLCD.setColor(0,0,255);
                break;
            case 5:
                myGLCD.setColor(255,255,0);
                break;
        }
        myGLCD.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
    }
}

void dFilledRoundRect()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);

    // Draw some filled, rounded rectangles
    for (int i=1; i<6; i++)
    {
        switch (i)
        {
            case 1:
                myGLCD.setColor(255,0,255);
                break;
            case 2:
                myGLCD.setColor(255,0,0);
                break;
            case 3:
                myGLCD.setColor(0,255,0);
                break;
            case 4:
                myGLCD.setColor(0,0,255);
                break;
            case 5:
                myGLCD.setColor(255,255,0);
                break;
        }
        myGLCD.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
    }
}

void dFilledCirc()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);

    // Draw some filled circles
    for (int i=1; i<6; i++)
    {
        switch (i)
        {
            case 1:
                myGLCD.setColor(255,0,255);
                break;
            case 2:
                myGLCD.setColor(255,0,0);
                break;
            case 3:
                myGLCD.setColor(0,255,0);
                break;
            case 4:
                myGLCD.setColor(0,0,255);
                break;
            case 5:
                myGLCD.setColor(255,255,0);
                break;
        }
        myGLCD.fillCircle(100+(i*20),60+(i*20), 30);
    }
}

void dSomeLines()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);

    // Draw some lines in a pattern
    myGLCD.setColor (255,0,0);
    for (int i=15; i<224; i+=5)
    {
        myGLCD.drawLine(1, i, (i*1.44)-10, 224);
    }
    myGLCD.setColor (255,0,0);
    for (int i=224; i>15; i-=5)
    {
        myGLCD.drawLine(318, i, (i*1.44)-11, 15);
    }
    myGLCD.setColor (0,255,255);
    for (int i=224; i>15; i-=5)
    {
        myGLCD.drawLine(1, i, 331-(i*1.44), 15);
    }
    myGLCD.setColor (0,255,255);
    for (int i=15; i<224; i+=5)
    {
        myGLCD.drawLine(318, i, 330-(i*1.44), 224);
    }
}

void dRandCirc()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,225);

    // Draw some random circles
    for (int i=0; i<100; i++)
    {
        myGLCD.setColor(random(255), random(255), random(255));
        myGLCD.drawCircle(32+random(256), 45+random(146), random(30));
    }
}

void dRandRect()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,224);

    // Draw some random rectangles
    for (int i=0; i<100; i++)
    {
        myGLCD.setColor(random(255), random(255), random(255));
        myGLCD.drawRect(2+random(316), 16+random(207), 2+random(316), 16+random(207));
    }
}

void dRandRoundRect()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,224);

    // Draw some random rectangles
    for (int i=0; i<100; i++)
    {
        myGLCD.setColor(random(255), random(255), random(255));
        myGLCD.drawRoundRect(2+random(316), 16+random(207), 2+random(316), 16+random(207));
    }
}

void dRandLines()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,224);

    // Draw some random rectangles
    for (int i=0; i<100; i++)
    {
        myGLCD.setColor(random(255), random(255), random(255));
        myGLCD.drawLine(2+random(316), 16+random(207), 2+random(316), 16+random(207));
    }
}

void dRandPixels()
{
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(1,15,318,224);

    for (int i=0; i<5000; i++)
    {
        myGLCD.setColor(random(255), random(255), random(255));
        myGLCD.drawPixel(2+random(316), 16+random(209));
    }
}

void drawButton(int x1, int y1, int x2, int y2)
{
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect(x1, y1, x2, y2);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect(x1, y1, x2, y2); 
}

void drawButtons()
{
    int t;
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(0, 0, 255);    
    // Draw the upper row of buttons
    for (x=0; x<5; x++)
    {
        t = (x * 60);
        drawButton(15 + t, 40, 65 + t, 90);
        myGLCD.printNumI(x, 32 + t, 57);
    }
    // Draw the center row of buttons
    for (x=0; x<5; x++)
    {
        t = (x * 60);
        drawButton(15 + t, 110, 65 + t, 160);
        myGLCD.printNumI(x+5, 32 + t, 127);
    }
    myGLCD.setBackColor (0, 0, 0);
    myGLCD.setFont(SmallFont);
}

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

void setup()
{
    Serial.begin(9600);
    Serial.println("INIT");
    randomSeed(analogRead(0));

    // Setup the LCD   
    for(int p=22;p<42;p++)
        pinMode(p,OUTPUT);
    for(int p=2; p<7;p++)
        pinMode(p,OUTPUT);
    pinMode(DOUT,INPUT);
    pinMode(IRQ,INPUT);  
    myGLCD.InitLCD(LANDSCAPE);
    myGLCD.setFont(SmallFont);
    myGLCD.clrScr();
    dFrame();
    drawButtons();
    control = -1;
    Serial.println("LOOP");
}

void loop()
{
    if (digitalRead(IRQ) == 0)
    {        
        digitalWrite(CS,LOW);
        TP_Y = ReadFromChar(0x90);
        TP_X = ReadFromChar(0xD0);
        digitalWrite(CS,HIGH);

        Serial.print((TP_X-340)*10/144);
        Serial.print(" - ");
        Serial.print(320-((TP_Y-320)/11));
        Serial.println("");  

        
        if (control >= 0)
        {
            Serial.println(control);
            switch (control) {
                case 1: dMovingSin(); break;
                case 2: dFilledRect(); break;
                case 3: dFilledRoundRect(); break;
                case 4: dFilledCirc(); break;
                case 5: dSomeLines(); break;
                case 6: dRandCirc(); break;
                case 7: dRandRect(); break;
                case 8: dRandRoundRect(); break;
                case 9: dRandLines(); break;
                case 0: dSinCosTan(); break;
            }
            control = -1;
            delay(2000);
            drawButtons();
        }
    }
}

