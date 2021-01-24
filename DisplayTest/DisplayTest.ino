//web: http://www.buydisplay.com
//EastRising Technology Co.,LTD
//Examples for ER-TFTM080-1 with Capacitive Touch Panel
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
// This demo was made for modules with a screen resolution
// of 800x480 pixels.
// This program requires the UTFT library.
//

#include <stdint.h>
#include <UTFT.h>
#include <SPI.h>
#include <Wire.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>
int but1, but2, but3, but4, but5, but6, but7, but8, but9, but10, but11, but12, but13, but14;
bool hitPointsGlo = 0, once1 = 0;
int previousX = 0, previousY = 0;
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
bool wait = true;
int pressed_button;
int sitebook = 300;
uint8_t addr  = 0x5D;  //CTP IIC ADDRESS
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern bitmapdatatype bell;
extern bitmapdatatype cat;
extern bitmapdatatype bird;

// Set the pins to the correct ones for your development shield
// Standard Arduino Mega/Due shield

#define GT911_RESET 41   //CTP RESET
#define GT911_INT   48   //CTP  INT
int butskip;
UTFT myGLCD(TFTM080_1_16, 38, 39, 40, 41, 1); //(byte model, int RS, int WR, int CS, int RST)
URTouch  myTouch( 6, 5, 4, 3, 2);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);


unsigned char  GTP_CFG_DATA[] =
{
  0x5A, 0x20, 0x03, 0xE0, 0x01, 0x05, 0x0D, 0x00,
  0x01, 0x08, 0x28, 0x08, 0x50, 0x32, 0x03, 0x05,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x88, 0x29, 0x0A, 0x35, 0x37,
  0xD3, 0x07, 0x00, 0x00, 0x01, 0x81, 0x02, 0x1D,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x64, 0x32,
  0x00, 0x00, 0x00, 0x28, 0x5A, 0x94, 0xC5, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x98, 0x2B, 0x00, 0x84,
  0x33, 0x00, 0x74, 0x3C, 0x00, 0x67, 0x46, 0x00,
  0x5C, 0x53, 0x00, 0x5C, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,
  0x12, 0x14, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
  0x04, 0x06, 0x08, 0x0F, 0x10, 0x12, 0x16, 0x18,
  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x25, 0x01
};


struct TouchLocation
{
  uint16_t x;
  uint16_t y;
};
TouchLocation touchLocations[5];

void inttostr(uint16_t value, uint8_t *str);
uint8_t GT911_Send_Cfg(uint8_t * buf, uint16_t cfg_len);
void writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt);
uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len );
uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num );
uint32_t dist(const TouchLocation & loc);
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2);
bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 );


uint8_t buf[80];


uint8_t GT911_Send_Cfg(uint8_t * buf, uint16_t cfg_len)
{
  //uint8_t ret=0;
  uint8_t retry = 0;
  for (retry = 0; retry < 5; retry++)
  {
    writeGT911TouchRegister(0x8047, buf, cfg_len);
    //if(ret==0)break;
    delay(10);
  }
  //return ret;
}


void writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt)
{ uint16_t i = 0;
  Wire.beginTransmission(addr);
  Wire.write( regAddr >> 8 ); // register 0
  Wire.write( regAddr);  // register 0
  for (i = 0; i < cnt; i++, val++) //data
  {
    Wire.write( *val );  // value
  }
  uint8_t retVal = Wire.endTransmission();
}

uint8_t readGT911TouchAddr( uint16_t regAddr, uint8_t * pBuf, uint8_t len )
{
  Wire.beginTransmission(addr);
  Wire.write( regAddr >> 8 ); // register 0
  Wire.write( regAddr);  // register 0
  uint8_t retVal = Wire.endTransmission();

  uint8_t returned = Wire.requestFrom(addr, len);    // request 1 bytes from slave device #2

  uint8_t i;
  for (i = 0; (i < len) && Wire.available(); i++)

  {
    pBuf[i] = Wire.read();
  }

  return i;
}

uint8_t readGT911TouchLocation( TouchLocation * pLoc, uint8_t num )
{
  uint8_t retVal;
  uint8_t i;
  uint8_t k;
  uint8_t  ss[1];

  do
  {

    if (!pLoc) break; // must have a buffer
    if (!num)  break; // must be able to take at least one
    ss[0] = 0;
    readGT911TouchAddr( 0x814e, ss, 1);
    uint8_t status = ss[0];

    if ((status & 0x0f) == 0) break; // no points detected
    uint8_t hitPoints = status & 0x0f;

    Serial.print("number of hit points = ");
    Serial.println( hitPoints );
    hitPointsGlo = hitPoints;

    uint8_t tbuf[32];  uint8_t tbuf1[8];
    readGT911TouchAddr( 0x8150, tbuf, 40);
    readGT911TouchAddr( 0x8150 + 32, tbuf1, 8);

    for (k = 0, i = 0; (i <  4 * 8) && (k < num); k++, i += 8)
    {
      pLoc[k].x = tbuf[i + 1] << 8 | tbuf[i + 0];
      pLoc[k].y = tbuf[i + 3] << 8 | tbuf[i + 2];
    }


    pLoc[k].x = tbuf1[1] << 8 | tbuf1[0];
    pLoc[k].y = tbuf1[3] << 8 | tbuf1[2];



    retVal = hitPoints;

  } while (0);

  ss[0] = 0;
  writeGT911TouchRegister( 0x814e, ss, 1);
  delay(2);
  return retVal;
}



void setup()
{
  randomSeed(analogRead(0));

  Serial.begin(115200);
  Wire.begin();        // join i2c bus (address optional for master)

  delay(300);

  pinMode(GT911_RESET, OUTPUT);
  pinMode     (GT911_INT, OUTPUT);
  digitalWrite(GT911_RESET, LOW);
  delay(20);
  digitalWrite(GT911_INT, LOW);
  delay(50);
  digitalWrite(GT911_RESET, HIGH);
  delay(100);
  pinMode     (GT911_INT, INPUT);
  delay(100);
  uint8_t re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));

  pinMode(GT911_RESET, OUTPUT);
  pinMode     (GT911_INT, OUTPUT);
  digitalWrite(GT911_RESET, LOW);
  delay(20);
  digitalWrite(GT911_INT, LOW);
  delay(50);
  digitalWrite(GT911_RESET, HIGH);
  delay(100);
  pinMode     (GT911_INT, INPUT);
  delay(100);
  re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));


  uint8_t bb[2];
  readGT911TouchAddr(0x8047, bb, 2);
  while (bb[1] != 32)
  {
    Serial.println("Capacitive touch screen initialized failure");
    pinMode(GT911_RESET, OUTPUT);
    pinMode     (GT911_INT, OUTPUT);
    digitalWrite(GT911_RESET, LOW);
    delay(20);
    digitalWrite(GT911_INT, LOW);
    delay(50);
    digitalWrite(GT911_RESET, HIGH);
    delay(100);
    pinMode     (GT911_INT, INPUT);
    delay(100);

    uint8_t re = GT911_Send_Cfg((uint8_t*)GTP_CFG_DATA, sizeof(GTP_CFG_DATA));
  }

  Serial.println("Capacitive touch screen initialized success");

  // Setup the LCD
  myGLCD.InitLCD();
  // -------------------------------------------------------------
  pinMode(8, OUTPUT);  //backlight
  digitalWrite(8, HIGH);//on
  // -------------------------------------------------------------
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  myButtons.setTextFont(BigFont);
  //butskip = myButtons.addButton( 85,  219 , 70,  20, "Skip");
  //Serial.println("Skip created");
  // myButtons.drawButton(butskip);

  myGLCD.setFont(BigFont);
  //  myGLCD.setFont(2);
  myGLCD.fillRect(0, 0, 799, 879);
  myGLCD.setColor(255, 0, 0);
  //  myGLCD.print("* Enter the Booking Number!", 300, 150, 0);
  // Draw the upper row of buttons

  myGLCD.setFont(BigFont);

  myGLCD.setBackColor(255, 255, 255); /// white backgroung
  myGLCD.setColor(0, 255, 0); /// letter color
  myGLCD.print("WELCOME", 750, 130, 90);
  myGLCD.setColor(0, 0, 0); /// black
  myGLCD.print(" HINDI  ", 735, 260, 90);
  myGLCD.setColor(255, 0, 0); /// red color
  myGLCD.print(" CHINA ", 700, 135, 90);
  myGLCD.setColor(255, 255, 0); /// yellow color
  myGLCD.print(" BIENVENIDA ", 660, 190, 90);
  myGLCD.setColor(0, 255, 255); /// blue color
  myGLCD.print(" WILLKOMMEN ", 615, 125, 90);
  myGLCD.setColor(150, 0, 255); /// letter color
  myGLCD.print(" BIENVENUE ", 570, 110, 90);
  myGLCD.setColor(0, 150, 255); /// letter color
  myGLCD.print(" UNKOWN1 ", 570, 250, 90);
  myGLCD.setColor(0, 255, 0); /// letter color
  myGLCD.print(" RUSSI ", 530, 150, 90);
  myGLCD.setColor(0, 255, 255); /// letter color
  myGLCD.print("URDU", 490, 100, 90);
  myGLCD.setColor(255, 0, 0); /// red color
  myGLCD.print("BEM-VINDO ", 490, 200, 90);


  //  but1 = myButtons.addButton(10, 30, 70, 70, "1");
  //  but2 = myButtons.addButton(105, 30, 70, 70, "2");
  //  but3 = myButtons.addButton(195, 30, 70, 70, "3");
  //
  //  but4 = myButtons.addButton(10, 130, 70, 70, "4");
  //  but5 = myButtons.addButton(105, 130, 70, 70, "5");
  //  but6 = myButtons.addButton(195, 130, 70, 70, "6");
  //
  //  but7 = myButtons.addButton(10, 230, 70, 70, "7");
  //  but8 = myButtons.addButton(105, 230, 70, 70, "8");
  //  but9 = myButtons.addButton(195, 230, 70, 70, "9");
  //
  //  but10 = myButtons.addButton(10, 330, 70, 70, "*");
  //  but11 = myButtons.addButton(105, 330, 70, 70, "0");
  //  but12 = myButtons.addButton(195, 330, 70, 70, "#");
  //
  //  but13 = myButtons.addButton(398, 300, 90, 70, "Enter");
  //  but14 = myButtons.addButton(530, 300, 90, 70, "Clear");
  //
  //  myButtons.drawButtons();

  //  int but15 = myButtons.addButton(260, 130, 16, 16, bell);
  //  int but16 = myButtons.addButton(180, 130, 25, 25, bird);
  //  myButtons.drawButtons();
  //  myGLCD.setColor(0, 0, 0); /// letter color
  //  myGLCD.print("Deutsch ", 310, 260, 90);
  //  myGLCD.print("English ", 220, 260, 90);
  //
  //  myGLCD.drawBitmap(200, 200, 16, 16, bell,1);

  // but1 = myButtons.addButton(10, 30, 70, 70, "1");
  //  but2 = myButtons.addButton(105, 30, 70, 70, "2");
  //  but3 = myButtons.addButton(195, 30, 70, 70, "3");
  //
  //  but4 = myButtons.addButton(10, 130, 70, 70, "4");
  //  but5 = myButtons.addButton(105, 130, 70, 70, "5");
  //  but6 = myButtons.addButton(195, 130, 70, 70, "6");
  //
  //  but7 = myButtons.addButton(10, 230, 70, 70, "7");
  //  but8 = myButtons.addButton(105, 230, 70, 70, "8");
  //  but9 = myButtons.addButton(195, 230, 70, 70, "9");
  //
  //  but10 = myButtons.addButton(10, 330, 70, 70, "*");
  //  but11 = myButtons.addButton(105, 330, 70, 70, "0");
  //  but12 = myButtons.addButton(195, 330, 70, 70, "#");
  but13 = myButtons.addButton(530, 300, 90, 70, "Clear");

  but14 = myButtons.addButton(398, 300, 90, 70, "OK");

}


uint32_t dist(const TouchLocation & loc)
{
  uint32_t retVal = 0;

  uint32_t x = loc.x;
  uint32_t y = loc.y;

  retVal = x * x + y * y;

  return retVal;
}
uint32_t dist(const TouchLocation & loc1, const TouchLocation & loc2)
{
  uint32_t retVal = 0;

  uint32_t x = loc1.x - loc2.x;
  uint32_t y = loc1.y - loc2.y;

  retVal = sqrt(x * x + y * y);

  return retVal;
}

bool sameLoc( const TouchLocation & loc, const TouchLocation & loc2 )
{
  return dist(loc, loc2) < 50;
}


void loop()
{
  int buf[798];
  int x, x2;
  int y, y2;
  int r;

  // Clear the screen and draw the frame
  //myGLCD.clrScr();

  uint8_t flag = 1;
  static uint16_t w = 800;
  static uint16_t h = 480;

  float xScale = 1024.0F / w;
  float yScale = 1024.0F / h;


  uint32_t thisTouchTime = millis();
  uint8_t i = 0;

  static
  uint32_t lastTouchTime = thisTouchTime;

  while (flag)
  {

    pinMode     (GT911_INT, INPUT);
    uint8_t st = digitalRead(GT911_INT);
    if (!st)  //Hardware touch interrupt
    {
      Serial.println("Touch: ");

      uint8_t count = readGT911TouchLocation( touchLocations, 5 );

      if (count)
      {
        static TouchLocation lastTouch = touchLocations[0];

        if (((thisTouchTime - lastTouchTime) > 10000) && sameLoc( touchLocations[0], lastTouch ) )
        {
         // myGLCD.setColor(0, 0, 0);
         // myGLCD.fillRect(0, 0, 799, 879);
         // lastTouchTime = thisTouchTime;
        }

        Serial.print("Time delta = ");
        Serial.print(thisTouchTime - lastTouchTime);
        Serial.print(", dist = ");
        Serial.println( dist(touchLocations[0], lastTouch) );

        lastTouch = touchLocations[0];

        Serial.println("Locations: ");

        for (i = 0; i < count; i++)
        {
          if (touchLocations[i].x >= 3 && touchLocations[i].x <= 40 && touchLocations[i].y >= 450 && touchLocations[i].y <= 480 )flag = 0;

          if (hitPointsGlo == 1)
          {
            if ((touchLocations[i].x != previousX) && (touchLocations[i].y != previousY))
            {
            //  myGLCD.setColor(0, 0, 0);
            //  myGLCD.setBackColor(255, 0, 0); /// Red
              hitPointsGlo = 0;
              Serial.print("Xaxis---");
              Serial.print(touchLocations[i].x);
              Serial.print("---Yaxis---");
              Serial.println(touchLocations[i].y);
              snprintf((char*)buf, sizeof(buf), "(%3d,%3d)", touchLocations[i].x, touchLocations[i].y);
              myGLCD.print((const char *)buf, CENTER, 280 + 16 * i, 90);

              // myGLCD.setColor(0, 255, 0);
              // myGLCD.fillRect(0, 0, 799, 479);

              if (touchLocations[i].x > 4 && touchLocations[i].x < 55 && touchLocations[i].y > 8 && touchLocations[i].y < 80)
              {
                // myGLCD.print("1", sitebook, 200, 0);
                // myButtons.addButton(sitebook, 200, 10, 10, "1");
                // myButtons.drawButtons();
                sitebook = sitebook + 30;
              }
              if (touchLocations[i].x > 112 && touchLocations[i].x < 160 && touchLocations[i].y > 8 && touchLocations[i].y < 80)
              {
                //  myGLCD.print("2", sitebook, 200, 0);
                //myButtons.addButton(sitebook, 200, 5, 5, "2");
                // myButtons.drawButtons();
                sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 224 && touchLocations[i].x < 260 && touchLocations[i].y > 8 && touchLocations[i].y < 80)
              {
                // myGLCD.print("3", sitebook, 200, 0);              sitebook = sitebook + 30;
              }
              if (touchLocations[i].x > 4 && touchLocations[i].x < 55 && touchLocations[i].y > 145 && touchLocations[i].y < 200)
              {
                //  myGLCD.print("4", sitebook, 200, 0);               sitebook = sitebook + 30;
              }
              if (touchLocations[i].x > 112 && touchLocations[i].x < 160 && touchLocations[i].y > 148 && touchLocations[i].y < 182)
              {
                //   myGLCD.print("5", sitebook, 200, 0);              sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 210 && touchLocations[i].x < 243 && touchLocations[i].y > 148 && touchLocations[i].y < 182)
              {
                //   myGLCD.print("6", sitebook, 200, 0);              sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 10 && touchLocations[i].x < 65 && touchLocations[i].y > 243 && touchLocations[i].y < 293)
              {
                //  myGLCD.print("7", sitebook, 200, 0);              sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 112 && touchLocations[i].x < 160 && touchLocations[i].y > 231 && touchLocations[i].y < 280)
              {
                //  myGLCD.print("8", sitebook, 200, 0);              sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 210 && touchLocations[i].x < 245 && touchLocations[i].y > 240 && touchLocations[i].y < 288)
              {
                //  myGLCD.print("9", sitebook, 200, 0);              sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 10 && touchLocations[i].x < 65 && touchLocations[i].y > 357 && touchLocations[i].y < 400)
              {
                //  myGLCD.print("*", sitebook, 200, 0);
                sitebook = sitebook + 30;

              }
              if (touchLocations[i].x > 112 && touchLocations[i].x < 160 && touchLocations[i].y > 356 && touchLocations[i].y < 403)
              {
                //  myGLCD.print("0", sitebook, 200, 0);
                sitebook = sitebook + 30;
              }
              if (touchLocations[i].x > 210 && touchLocations[i].x < 245 && touchLocations[i].y > 365 && touchLocations[i].y < 410)
              {
                //  myGLCD.print("#", sitebook, 200, 0);
                sitebook = sitebook + 30;
              }
              if (touchLocations[i].x > 400 && touchLocations[i].x < 476 && touchLocations[i].y > 318 && touchLocations[i].y < 370)
              {
                //                myGLCD.print("Sucessful", 400, 100, 0);
                //                myGLCD.setBackColor(255, 255, 255); /// white backgroung
                //                myGLCD.setColor(255, 255, 255); /// letter color
                //                delay(1000);
                //                myGLCD.print("Sucessful", 400, 100, 0);
              }

              if (touchLocations[i].x > 535 && touchLocations[i].x < 608 && touchLocations[i].y > 321 && touchLocations[i].y < 368)
              {
                //                myGLCD.print("clear", 300, 75, 0);
                //                sitebook = 300;
                //
                //                for (int rep = 300; rep < 661;)
                //                {
                //                  myGLCD.setBackColor(255, 255, 255); /// white backgroung
                //                  myGLCD.setColor(255, 255, 255); /// letter color
                //                  myGLCD.print("1", rep, 200, 0);
                //                  rep = rep + 30;
                //                }
                //
                //                delay(1000);
                //                myGLCD.print("clear", 300, 75, 0);
              }
              previousX = touchLocations[i].x;
              previousY = touchLocations[i].y;
            }
            else
            {

              Serial.print("Xaxis111---");
              Serial.print(previousX);
              Serial.print("---Yaxis111---");
              Serial.println(previousY);
            }
          }
        }
      }
    }
  }
}
