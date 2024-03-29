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
//#include <SD.h>

#define SD_CS 0x5D
int dispx, dispy;

int but1, but2, but3, but4, but5, but6, but7, but8, but9, but10, but11, but12, but13, but14;
bool hitPointsGlo = 0, once1 = 0;
int previousX = 0, previousY = 0;
int buttonsonce = 0;
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t arial_bold[];
extern uint8_t hallfetica_normal[];
extern uint8_t Arial_round_16x24[];
//extern uint8_t
//extern uint8_t
//extern uint8_t
//extern uint8_t
bool wait = true;
int dispaly5 = 0;
int pressed_button;
int sitebook = 0;
uint8_t addr  = 0x5D;  //CTP IIC ADDRESS
// Declare which fonts we will be using
extern uint8_t GroteskBold24x48[];
extern uint8_t GroteskBold32x64[];
//extern bitmapdatatype cat;
extern bitmapdatatype icon;
void ButtonsDisplay();
void WelcomeDisplay();

extern unsigned short GermanFL[9072];
extern unsigned short EnglischFL[9072];
extern unsigned short RussiaFL[9072];
extern unsigned short CzechFL[9072];
extern unsigned short PolandFL[9072];
extern unsigned short KroatishFL[9072];
extern unsigned short SloveniaFL[9072];
extern unsigned short NederlandFL[9072];
extern unsigned short DankFL[9072];
extern unsigned short SpanishFL[9072];
extern unsigned short FranceFL[9072];
extern unsigned short ItalyFL[9072];

extern unsigned short Hotline[5220];
extern unsigned short Transicon[6400];

extern unsigned short Save[10000];
extern unsigned short Cancel[10000];

extern unsigned short WelcomeLogo[58450];
extern unsigned short HHSLogo[6270];
extern unsigned short zuruuck[11926];

String LanguageOp[30] = {"German", "English", "Russian", "Czech", "Polski", "Hrvatski", "Slovenian", "Nederlands", "Dansk", "Espanol", "Francals", "Italiano", "Japan ", "SouthKorea", "China", "Hindi", "  Vietnamese", "Arabic", "  Hungarian", "Greek", ""};

// Set the pins to the correct ones for your development shield
// Standard Arduino Mega/Due shield

#define GT911_RESET 41   //CTP RESET
#define GT911_INT   48   //CTP  INT
int butskip;
UTFT myGLCD(SSD1963_800480, 38, 39, 40, 41, 1); //(byte model, int RS, int WR, int CS, int RST)
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

void setup()
{
  randomSeed(analogRead(0));

  Serial.begin(9600);
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
  myGLCD.InitLCD(0);
  // -------------------------------------------------------------
  pinMode(8, OUTPUT);  //backlight
  digitalWrite(8, HIGH);//on
  // -------------------------------------------------------------
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  dispx = myGLCD.getDisplayXSize();
  dispy = myGLCD.getDisplayYSize();

  //  Serial.print("Initializing SD card...");
  //  if (!SD.begin(SD_CS)) {
  //    Serial.println("failed!");
  //  }
  //  Serial.println("OK!");

  WelcomeDisplay();  //test1

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
  static uint16_t w = 480;
  static uint16_t h = 800;

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
        //        Serial.print("Time delta = ");
        //        Serial.print(thisTouchTime - lastTouchTime);
        //        Serial.print(", dist = ");
        //        Serial.println( dist(touchLocations[0], lastTouch) );

        lastTouch = touchLocations[0];

        // Serial.println("Locations: ");

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
              myGLCD.setFont(SmallFont);
              //              Serial.print("Xaxis---");
              //              Serial.print(touchLocations[i].x);
              //              Serial.print("---Yaxis---");
              //              Serial.println(touchLocations[i].y);
              snprintf((char*)buf, sizeof(buf), "(%3d,%3d)", touchLocations[i].x, touchLocations[i].y);
              myGLCD.print((const char *)buf, CENTER, 280 + 16 * i);

              // myGLCD.setColor(0, 255, 0);
              // myGLCD.fillRect(0, 0, 799, 479);

              //  myGLCD.setFont(GroteskBold24x48);

              if (touchLocations[i].x > 316 && touchLocations[i].x < 375 && touchLocations[i].y > 390 && touchLocations[i].y < 465)
              {
                myGLCD.clrScr();
                WelcomeDisplay();
              }

              if (touchLocations[i].x > 742  && touchLocations[i].x < 788 && touchLocations[i].y > 4 && touchLocations[i].y < 45)
              {
                myGLCD.clrScr();
                LanguageDisplay();
              }

              if (touchLocations[i].x > 238 && touchLocations[i].x < 273 && touchLocations[i].y > 138 && touchLocations[i].y < 200)
              {
                //                buttonsonce++;
                //                if (buttonsonce == 1) ButtonsDisplay();
                //                sitebook = 0;
                //                Serial.println("Button Display");
                //                dispaly5++;
              }
              if (touchLocations[i].x > 756 && touchLocations[i].x < 786 && touchLocations[i].y > 246 && touchLocations[i].y < 470)
              {
                //                dispaly5 = 0;
                //                WelcomeDisplay();
                //                buttonsonce = 0;
                //                Serial.println("Welcome Display");
              }

              /*  if (true)
                {
                  if (touchLocations[i].x > 340 && touchLocations[i].x < 429 && touchLocations[i].y > 8 && touchLocations[i].y < 113)
                  {
                    myGLCD.setFont(GroteskBold24x48);
                    myGLCD.print("1", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 352 && touchLocations[i].x < 415 && touchLocations[i].y > 162 && touchLocations[i].y < 294)
                  {
                    myGLCD.setFont(GroteskBold24x48);
                    myGLCD.print("2", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 341 && touchLocations[i].x < 414 && touchLocations[i].y > 326 && touchLocations[i].y < 470)
                  {
                    myGLCD.print("3", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 238 && touchLocations[i].x < 309 && touchLocations[i].y > 8 && touchLocations[i].y < 113)
                  {
                    myGLCD.print("4", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 238 && touchLocations[i].x < 309 && touchLocations[i].y > 162 && touchLocations[i].y < 294)
                  {
                    dispaly5++;
                    Serial.println(dispaly5);
                    if (dispaly5 > 2)
                    {
                      myGLCD.print("5", 60 + sitebook, 184, 0);
                      sitebook = sitebook + 30;
                    }
                  }
                  if (touchLocations[i].x > 238 && touchLocations[i].x < 309 && touchLocations[i].y > 326 && touchLocations[i].y < 470)
                  {
                    myGLCD.print("6", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 117 && touchLocations[i].x < 206 && touchLocations[i].y > 8 && touchLocations[i].y < 113)
                  {
                    myGLCD.print("7", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 117 && touchLocations[i].x < 206 && touchLocations[i].y > 162 && touchLocations[i].y < 294)
                  {
                    myGLCD.print("8", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 117 && touchLocations[i].x < 206 && touchLocations[i].y > 326 && touchLocations[i].y < 470)
                  {
                    myGLCD.print("9", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 4 && touchLocations[i].x < 92 && touchLocations[i].y > 162 && touchLocations[i].y < 294)
                  {
                    myGLCD.print("0", 60 + sitebook, 184, 0);
                    sitebook = sitebook + 30;
                  }
                  if (touchLocations[i].x > 4 && touchLocations[i].x < 92 && touchLocations[i].y > 326 && touchLocations[i].y < 470)
                  {
                    //myButtons.setTextFont(SmallFont);
                    // myGLCD.print("Sucessful", 30, 160, 0);
                  }
                }*/

              if (touchLocations[i].x > 4 && touchLocations[i].x < 92 && touchLocations[i].y > 8 && touchLocations[i].y < 113)
              {
                //myButtons.setTextFont(SmallFont);
                // myGLCD.print("clear", 30, 160, 0);
                //sitebook = 0;

                //                int rep = sitebook - 30;
                //
                //                myGLCD.setBackColor(255, 255, 255); /// white backgroung
                //                myGLCD.setColor(255, 255, 255); /// letter color
                //                myGLCD.print("1", rep, 184, 0);
                //                sitebook = sitebook - 30;

                // delay(1000);
                //  myGLCD.print("clear", 30, 160, 0);
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
void WelcomeDisplay()
{
  myGLCD.setColor(255, 255, 255);

  myGLCD.fillRect(0, 0, 799, 879);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 255, 255); /// white backgroung
  myGLCD.setColor(255, 255, 255);

  myGLCD.drawBitmap (240, 20, 190, 33, HHSLogo);  ///190x33 (30 gap)
  myGLCD.drawBitmap (0, 0, 80, 80, Transicon);

  myGLCD.drawBitmap (50, 100, 350, 167, WelcomeLogo); ///(X and Y posion then Size of Image) 350x167
  myGLCD.setColor(0, 0, 0);
  myGLCD.setFont(BigFont);

  myGLCD.drawBitmap (50,  300, 108, 84, GermanFL);   //myGLCD.print("Germany", 70, 395);  108x84
  myGLCD.drawBitmap (185, 300, 108, 84, EnglischFL);// myGLCD.print("English", 200, 395);
  myGLCD.drawBitmap (315, 300, 108, 84, RussiaFL);  // myGLCD.print("Russian", 330, 395);

  myGLCD.drawBitmap (50,  400, 108, 84, CzechFL);  //  myGLCD.print("Czech ", 70, 495);
  myGLCD.drawBitmap (185, 400, 108, 84, PolandFL);  // myGLCD.print("Poland", 200, 495);
  myGLCD.drawBitmap (315, 400, 108, 84, KroatishFL);// myGLCD.print("Croatia", 330, 495);

  myGLCD.drawBitmap (50,  500, 108, 84, SloveniaFL);// myGLCD.print("Slovenia", 70, 595);
  myGLCD.drawBitmap (185, 500, 108, 84, NederlandFL);// myGLCD.print("Nederland", 200, 595);
  myGLCD.drawBitmap (315, 500, 108, 84, DankFL);   //  myGLCD.print("Denmark", 330, 595);

  myGLCD.drawBitmap (50,  600, 108, 84, SpanishFL);//  myGLCD.print("Spain", 70, 695);
  myGLCD.drawBitmap (185, 600, 108, 84, FranceFL); //  myGLCD.print("France", 200, 695);
  myGLCD.drawBitmap (315, 600, 108, 84, ItalyFL);  //  myGLCD.print("Italy", 330, 695);

  myGLCD.drawBitmap (200, 715, 116, 45, Hotline);

  ButtonsDisplay();

  /*
    myGLCD.setColor(0, 255, 255);
    myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.setColor(0, 100, 100);

    int y1 = 30;
    int y2 = 80;
    int x1 = 50;
    int x2 = 100;
    int y22 = 50;
    String Langu[15] = {"German", "English", "Russian", "Czech", "Polski", "Hrvatski", "Slovenian", "Nederlands", "Dansk", "Espanol", "Francals", "Italiano", "Japan ", "SouthKorea", "China"};

    for (int i = 0; i < 12; i++)
    {
      myGLCD.setFont(BigFont);
      myGLCD.setBackColor(255, 0, 0);
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (x1  ,     y1 , x2 , y2);
      myGLCD.drawRoundRect (x1  ,     y1 , x2 , y2);

      myGLCD.drawRoundRect (x1 + 50 , y1 , x2 + 200 , y2);

      myGLCD.fillRoundRect (x1 + 250 , y1 , x2 + 250 , y2);
      myGLCD.drawRoundRect (x1 + 250 , y1 , x2 + 250 , y2);
      myGLCD.setColor(255, 0, 0);
      myGLCD.print(" - ", x1 + 1 , y1 + 20);
      myGLCD.print(" + ", 250 + x1, y1 + 17);
      //  myGLCD.print(" + ", x1 + 231, y1 + 10, 90);

      myGLCD.setBackColor(255, 255, 255);
      myGLCD.setColor(0, 0, 0);
      myGLCD.print(Langu[i], 122, y22);

      y1 = y1 + 62;
      y2 = y2 + 62;
      y22 = y22 + 62;
    }
  */
  //  myGLCD.drawBitmap (122,  50, 90, 30, GermanL);
  //  myGLCD.drawBitmap (122,  50 + 62, 90, 30, EnglishL);
  //  myGLCD.drawBitmap (122,  50 + 62 * 2, 90, 30, RussiaL);
  //  myGLCD.drawBitmap (375,  280, 100, 100, Save);
  //  myGLCD.drawBitmap (375,  400, 100, 100, Cancel);
}

void LanguageDisplay()
{
  myGLCD.setColor(255, 255, 255);

  myGLCD.fillRect(0, 0, 799, 879);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 255, 255); /// white backgroung
  myGLCD.setColor(255, 255, 255);

  myGLCD.setColor(255, 255, 255);
  myGLCD.setColor(0, 255, 255);
  myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.setColor(0, 100, 100);

  int y1 = 30;
  int y2 = 80;
  int x1 = 50;
  int x2 = 100;
  int y22 = 50;
  String Langu[15] = {"German", "English", "Russian", "Czech", "Polski", "Hrvatski", "Slovenian", "Nederlands", "Dansk", "Espanol", "Francals", "Italiano", "Japan ", "SouthKorea", "China"};

  for (int i = 0; i < 12; i++)
  {
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(255, 0, 0);
    myGLCD.setColor(0, 100, 100);
    myGLCD.fillRoundRect (x1  ,     y1 , x2 , y2);
    myGLCD.drawRoundRect (x1  ,     y1 , x2 , y2);
    myGLCD.drawRoundRect (x1 + 50 , y1 , x2 + 200 , y2);
    myGLCD.fillRoundRect (x1 + 250 , y1 , x2 + 250 , y2);
    myGLCD.drawRoundRect (x1 + 250 , y1 , x2 + 250 , y2);
    myGLCD.setColor(255, 0, 0);
    myGLCD.print(" - ", x1 + 1 , y1 + 17);
    myGLCD.print(" + ", 250 + x1, y1 + 17);
    myGLCD.print(" + ", x1 + 283, y1 + 1, 90);

    myGLCD.setBackColor(255, 255, 255);
    myGLCD.setColor(0, 0, 0);
    myGLCD.print(Langu[i], 122, y22);

    y1 = y1 + 62;
    y2 = y2 + 62;
    y22 = y22 + 62;
  }
  myGLCD.drawBitmap (375,  280, 100, 100, Save);
  myGLCD.drawBitmap (375,  400, 100, 100, Cancel);
}

void ButtonsDisplay()
{
  myGLCD.clrScr();
  myGLCD.setBackColor(255, 255, 255); /// white backgroung
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(0, 0, 799, 879);
  myGLCD.setColor(255, 255, 255);

  myGLCD.drawBitmap (240, 20, 190, 33, HHSLogo); 
  myGLCD.drawBitmap (0, 0, 178, 67, zuruuck);   

  myGLCD.drawBitmap (200, 715, 116, 45, Hotline);

  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (80, 200, 70 + 300 , 200 + 40); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.print("Buchungsnumer eingeben", 80, 160); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (50, 300, 50 + 108, 300 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (50, 300, 50 + 108, 300 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("1", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (185, 300, 185 + 108, 300 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (185, 300, 185 + 108, 300 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("2", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (315, 300, 315 + 108, 300 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (315, 300, 315 + 108, 300 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("3", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (50, 400, 50 + 108, 400 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (50, 400, 50 + 108, 400 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("1", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (185, 400, 185 + 108, 400 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (185, 400, 185 + 108, 400 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("2", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (315, 400, 315 + 108, 400 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (315, 400, 315 + 108, 400 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("3", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (50, 500, 50 + 108, 500 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (50, 500, 50 + 108, 500 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("1", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (185, 500, 185 + 108, 500 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (185, 500, 185 + 108, 500 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("2", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (315, 500, 315 + 108, 500 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (315, 500, 315 + 108, 500 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("3", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (50, 600, 50 + 108, 600 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (50, 600, 50 + 108, 600 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("1", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (185, 600, 185 + 108, 600 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (185, 600, 185 + 108, 600 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("2", 50, 384); // Prints the string

  myGLCD.setColor(255, 255, 150); // Sets yellow color
  myGLCD.fillRoundRect (315, 600, 315 + 108, 600 + 84); // Draws filled rounded rectangle
  myGLCD.setColor(0, 0, 0); // Sets color to black
  myGLCD.drawRoundRect (315, 600, 315 + 108, 600 + 84); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(255, 255, 150); // Sets the background color of the area where the text will be printed to green, same as the button
  //myGLCD.print("3", 50, 384); // Prints the string

  /*
    // Button - Distance Sensor
    int x1 = 475;
    int y1 = 795;
    int x2 = 350;//(left to right)
    int y2 = 675;// top to bottom
    int diffX = 175;
    // Button - Distance Sensor
    myGLCD.setColor(128, 255, 128); // Sets green color
    myGLCD.fillRoundRect (x1, y1, x2 - 30, y2 + 15); // Draws filled rounded rectangle
    myGLCD.setColor(128, 255, 128); // Sets color to white
    myGLCD.drawRoundRect (x1, y1, x2 - 30, y2 + 15); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(128, 255, 128); // Sets the background color of the area where the text will be printed to green, same as the button

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (10 + x1 - diffX, y1, x2 - 20 - diffX, y2 + 15); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (10 + x1 - diffX, y1, x2 - 20 - diffX, 15 + y2); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("0", 210 , 715); // Prints the string
    //
    // Button - Distance Sensor
    myGLCD.setColor(255, 128, 128); // Sets green color
    myGLCD.fillRoundRect (20 + x1 - diffX * 2, y1, x2 - (diffX * 2), 15 + y2); // Draws filled rounded rectangle
    myGLCD.setColor(255, 128, 128); // Sets color to white
    myGLCD.drawRoundRect (20 + x1 - diffX * 2, y1, x2 - (diffX * 2), 15 + y2); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (20 + x1 - diffX * 2, y1 - 110, x2 - (diffX * 2), 15 + y2 - 110); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (20 + x1 - diffX * 2, y1 - 110, x2 - (diffX * 2), 15 + y2 - 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("7", 57, 600); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (10 + x1 - diffX , y1 - 110, x2 - 20 - (diffX ), 15 + y2 - 110); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (10 + x1 - diffX , y1 - 110, x2 - 20 - (diffX ), 15 + y2 - 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("8", 210, 600); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (x1 - 0 , y1 - 110, x2 - 28 - (0 ), 15 + y2 - 110); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (x1 - 0 , y1 - 110, x2 - 28 - (0 ), 15 + y2 - 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("9", 380, 600); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (20 + x1 - diffX * 2, y1 - 220, x2 - (diffX * 2), 15 + y2 - 220); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (20 + x1 - diffX * 2, y1 - 220, x2 - (diffX * 2), 15 + y2 - 220); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("4", 50, 490); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (20 + x1 - diffX * 2, y1 - 330, x2 - (diffX * 2), 15 + y2 - 330); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (20 + x1 - diffX * 2, y1 - 330, x2 - (diffX * 2), 15 + y2 - 330); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("1", 50, 384); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (10 + x1 - diffX , y1 - 220, x2 - 20 - (diffX ), 15 + y2 - 220); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (10 + x1 - diffX , y1 - 220, x2 - 20 - (diffX ), 15 + y2 - 220); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("5", 210, 490); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (10 + x1 - diffX , y1 - 330, x2 - 20 - (diffX ), 15 + y2 - 330); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (10 + x1 - diffX , y1 - 330, x2 - 20 - (diffX ), 15 + y2 - 330); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("2", 210, 384); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (x1 - 0 , y1 - 220, x2 - 28 - (0 ), 15 + y2 - 220); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (x1 - 0 , y1 - 220, x2 - 28 - (0 ), 15 + y2 - 220); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("6", 380, 490); // Prints the string

    // Button - Distance Sensor
    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect (x1 - 0 , y1 - 330, x2 - 28 - (0 ), 15 + y2 - 330); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (x1 - 0 , y1 - 330, x2 - 28 - (0 ), 15 + y2 - 330); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("3", 380, 380); // Prints the string

    myGLCD.setColor(192, 192, 192); // Sets green color
    myGLCD.fillRoundRect ( 0, 0, 230, 50); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (0, 0, 230, 50); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(192, 192, 192); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("Zuruck", 60, 17); // Prints the string

    myGLCD.setColor(255, 0, 0); // Sets green color
    myGLCD.fillRoundRect ( 233, 0, 478, 50); // Draws filled rounded rectangle
    myGLCD.setColor(0, 0, 0); // Sets color to white
    myGLCD.drawRoundRect (233, 0, 478, 50); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
    myGLCD.setFont(BigFont); // Sets the font to big
    myGLCD.setBackColor(255, 0, 0); // Sets the background color of the area where the text will be printed to green, same as the button
    myGLCD.print("Abbrechen", 300, 17); // Prints the string

    //  //myGLCD.fillScr(255, 255, 255);
    //  //myGLCD.setColor(0, 0, 0);
    //  // myGLCD.print(" *** A 10 by 7 grid of a 32x32 icon *** ", CENTER, 228);
    //  for (int x = 0; x < 1; x++)
    //    for (int y = 0; y < 1; y++)
    //      myGLCD.drawBitmap (25, 705, 80, 80, YTdT8);
    //  //  delay(5000);
    //  for (int x = 0; x < 1; x++)
    //    for (int y = 0; y < 1; y++)
    //      myGLCD.drawBitmap (350, 705, 80, 80, okay);
    //
    //  myGLCD.drawBitmap (250, 5, 40, 40, butcancel);
    //  myGLCD.drawBitmap (13, 5, 40, 40, Back);
    //
    //  myGLCD.setFont(BigFont);
    //  myGLCD.setBackColor(255, 255, 255); /// white backgroung
    //  myGLCD.setColor(0, 0, 0); /// letter color
    //  myGLCD.print("Bitte geben Sie Ihre", 30, 100);
    //  myGLCD.print("Buchungsnummer ein", 30, 130);
  */
}

void writeGT911TouchRegister( uint16_t regAddr, uint8_t *val, uint16_t cnt)
{
  uint16_t i = 0;
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
#define BUFFPIXEL 20

// These read 16 - and 32 - bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

/*
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
    /* myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);
      myButtons.setButtonColors(VGA_WHITE, VGA_RED, VGA_RED, VGA_RED, VGA_RED);
      myGLCD.setFont(SmallFont);
      myGLCD.setBackColor(255, 255, 255); /// white backgroung
      myGLCD.setColor(0, 0, 0); /// letter color
      myGLCD.print("Bitte geben Sie Ihre Buchungsnummer ein ", 10, 200);
      but1 = myButtons.addButton(0, 258, 150, 120, "1");
      but2 = myButtons.addButton(165, 258, 150, 120, "2");
      but3 = myButtons.addButton(329, 258, 150, 120, "3");

      but4 = myButtons.addButton(0, 398, 150, 120, "4");
      but5 = myButtons.addButton(165, 398, 150, 120, "5");
      but6 = myButtons.addButton(329, 398, 150, 120, "6");

      but7 = myButtons.addButton(0, 538, 150, 120, "7");
      but8 = myButtons.addButton(165, 538, 150, 120, "8");
      but9 = myButtons.addButton(329, 538, 150, 120, "9");

      but10 = myButtons.addButton(0, 0, 230, 90, "<- Zuruck");
      but11 = myButtons.addButton(249, 0, 230, 90, "X Abbrechen");

      but12 = myButtons.addButton(0, 678, 150, 120, "Clear1");
      but13 = myButtons.addButton(165, 678, 150, 120, " 0 ");
      but14 = myButtons.addButton(329, 678, 150, 120, "Clear3");

      //  but1 = myButtons.addButton(329, 478, 150, 120, "a", BUTTON_SYMBOL);
      //  but2 = myButtons.addButton(329, 578, 150, 120, "I", BUTTON_SYMBOL | BUTTON_SYMBOL_REP_3X);
      delay(2000);
      myButtons.setButtonColors(VGA_YELLOW, VGA_GRAY, VGA_BLACK, VGA_RED, VGA_BLUE);

      myButtons.drawButtons();
      delay(2000);
      myButtons.setButtonColors(VGA_WHITE, VGA_GRAY, VGA_GRAY, VGA_GRAY, VGA_GRAY);

      myButtons.drawButtons();*/
/*
 * */
/*
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 80;
  y2 = y2 + 80;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);

  /*
  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);

  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);


  y1 = y1 + 90;
  y2 = y2 + 90;
  myGLCD.setColor(0, 100, 100);
  myGLCD.fillRoundRect (80  , y1 , 130 , y2); /// + 90   +70
  myGLCD.drawRoundRect (80  , y1 , 130 , y2);
  myGLCD.drawRoundRect (130 , y1 , 280 , y2);
  myGLCD.fillRoundRect (280 , y1 , 330 , y2);
  myGLCD.drawRoundRect (280 , y1 , 330 , y2);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" - ", 90, y1 + 20);
  myGLCD.print(" + ", 290, y1 + 20);
  myGLCD.print(" + ", 308, y1 + 15, 90);

  //  myGLCD.setColor(0, 100, 100);
  //  myGLCD.fillRoundRect (80  , 250 , 130 , 300);
  //  myGLCD.drawRoundRect (80  , 250 , 130 , 300);
  //  myGLCD.drawRoundRect (130 , 250 , 280 , 300);
  //  myGLCD.fillRoundRect (280 , 250 , 330 , 300);
  //  myGLCD.drawRoundRect (280 , 250 , 330 , 300);
  //  myGLCD.setColor(255, 0, 0);
  //  myGLCD.print(" - ", 90, 270);  //+ 20
  //  myGLCD.print(" + ", 290, 270);
  //  myGLCD.print(" + ", 308, 265, 90);

  /*
      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (330 , 210 , 410 , 290); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (330 , 210 , 410 , 290); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      //myGLCD.print(" English3 ", 240, 245);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (50 , 350 , 130 , 430); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (50 , 350 , 130 , 430); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      // myGLCD.print(" English1 ", 240, 145);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (190 , 350 , 270 , 430); // Draws filled rounded rectangle +130  +70
      myGLCD.drawRoundRect (190 , 350 , 270 , 430); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      // myGLCD.print(" English2 ", 240, 245);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (330 , 350 , 410 , 430); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (330 , 350 , 410 , 430); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      //myGLCD.print(" English3 ", 240, 245);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (50 , 490 , 130 , 570); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (50 , 490 , 130 , 570); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      // myGLCD.print(" English4 ", 240, 145);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (190 , 490 , 270 , 570); // Draws filled rounded rectangle +130  +70
      myGLCD.drawRoundRect (190 , 490 , 270 , 570); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      //myGLCD.print(" English5 ", 240, 245);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (330 , 490 , 410 , 570); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (330 , 490 , 410 , 570); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      //myGLCD.print(" English6 ", 240, 570);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (50 , 630 , 130 , 710); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (50 , 630 , 130 , 710); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      // myGLCD.print(" English7 ", 240, 145);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (190 , 630 , 270 , 710); // Draws filled rounded rectangle +130  +70
      myGLCD.drawRoundRect (190 , 630 , 270 , 710); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      // myGLCD.print(" English8 ", 240, 245);

      myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
      myGLCD.setColor(0, 100, 100);
      myGLCD.fillRoundRect (330 , 630 , 410 , 710); // Draws filled rounded rectangle
      myGLCD.drawRoundRect (330 , 630 , 410 , 710); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
      //myGLCD.print(" English9 ", 240, 570);

      /* myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
        myGLCD.setColor(0, 100, 100);
        myGLCD.fillRoundRect (60 , 300 , 130 , 380); // Draws filled rounded rectangle
        myGLCD.drawRoundRect (60 , 300 , 130 , 380); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
        myGLCD.print(" English3 ", 240, 345);

        myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
        myGLCD.setColor(0, 100, 100);
        myGLCD.fillRoundRect (60 , 400 , 130 , 480); // Draws filled rounded rectangle
        myGLCD.drawRoundRect (60 , 400 , 130 , 480); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
        myGLCD.print(" English4 ", 240, 445);

        myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
        myGLCD.setColor(0, 100, 100);
        myGLCD.fillRoundRect (60 , 500 , 130 , 580); // Draws filled rounded rectangle
        myGLCD.drawRoundRect (60 , 500 , 130 , 580); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
        myGLCD.print(" English5 ", 240, 545);

        myGLCD.setBackColor(255, 128, 128); // Sets the background color of the area where the text will be printed to green, same as the button
        myGLCD.setColor(0, 100, 100);
        myGLCD.fillRoundRect (60 , 600 , 130 , 680); // Draws filled rounded rectangle
        myGLCD.drawRoundRect (60 , 600 , 130 , 680); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
        myGLCD.print(" English6 ", 240, 645);
*/

//  // Draw filled triangle:
//  myGLCD.setColor(0, 0, 255);  // Set color (0,R,G,B)
//  myGLCD.drawTriangle(50, 159,   70, 130,   160, 159); // (x0,y0,x1,y1,x2,y2)
//  myGLCD.fillTriangle(50, 159,   70, 130,   160, 159); // (x0,y0,x1,y1,x2,y2)
//
//  myGLCD.setColor(0, 0, 255);  // Set color (0,R,G,B)
//  myGLCD.drawTriangle(200, 159,   240, 200,   270, 159); // (x0,y0,x1,y1,x2,y2)
//  myGLCD.fillTriangle(200, 159,   240, 200,   270, 159); // (x0,y0,x1,y1,x2,y2)

//  myButtons.setTextFont(BigFont);
//  //butskip = myButtons.addButton( 85,  219 , 70,  20, "Skip");
//  //Serial.println("Skip created");
//  // myButtons.drawButton(butskip);
//
//  myGLCD.fillRect(0, 0, 799, 879);
//  myGLCD.setColor(255, 255, 255);
//
//  myGLCD.setFont(BigFont);
//  //  myGLCD.setFont(2);
//  myGLCD.fillRect(0, 0, 799, 879);
//  myGLCD.setColor(255, 0, 0);
//  //  myGLCD.print("* Enter the Booking Number!", 300, 150, 0);
//  // Draw the upper row of buttons
//
//  myGLCD.setFont(GroteskBold24x48);
//  myGLCD.setBackColor(255, 255, 255); /// white backgroung
//
//  myGLCD.setColor(0, 255, 0); /// letter color
//  myGLCD.print("WELCOME", 150, 40);
//
//  myGLCD.setColor(0, 0, 0); /// black
//  myGLCD.drawBitmap (230, 92, 95, 42, Hindii);
//
//  myGLCD.setColor(255, 0, 0); /// red color
//  // myGLCD.print(" CHINA ", 80, 110); 95x46
//  myGLCD.drawBitmap (80, 102, 95, 46, China);
//
//  myGLCD.setColor(255, 255, 0); /// yellow color
//  myGLCD.print(" BIENVENIDA ", 120, 155);
//
//  myGLCD.setColor(0, 255, 255); /// blue color
//  myGLCD.print(" WILLKOMMEN ", 70, 210);
//
//  myGLCD.setColor(150, 0, 255); /// letter color
//  myGLCD.print(" BIENVENUE ", 160, 270);
//
//  myGLCD.setColor(0, 150, 255); /// letter color
//  myGLCD.drawBitmap (20, 325, 130, 34, Japan);
//
//  myGLCD.setColor(0, 255, 0); /// letter color
//  myGLCD.drawBitmap (165, 325, 180, 20, russia);
//
//  myGLCD.setColor(0, 255, 255);
//  myGLCD.drawBitmap (70, 390, 120, 32, urdu);
//
//  myGLCD.setColor(255, 0, 0); /// red color
//  myGLCD.print("BEM-VINDO ", 190, 390);
//
//  // myGLCD.fillRoundRect (90 , 560, 200, 510); // Draws filled rounded rectangle
//  // myGLCD.drawRoundRect (90 , 560, 200, 510); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
//  myGLCD.setFont(BigFont); // Sets the font to big
//
//  //int but15 = myButtons.addButton(90, 630, 128, 90, English);
//  myGLCD.drawBitmap (130, 510, 80, 48, Germany);  /// 100x60
//  myGLCD.drawBitmap (130, 570, 75, 52, English); /// 90x63
//
//  myGLCD.setColor(0, 0, 0); /// red color
//
//
//  // myGLCD.fillRoundRect (90 , 630, 200, 570); // Draws filled rounded rectangle
//  // myGLCD.drawRoundRect (90 , 630, 200, 570); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
//
