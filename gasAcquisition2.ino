/*!
Bhopal 84 Data acquisition script
Roni Bandini June 2022 - @RoniBandini
TinyML via Edge Impulse
MIT License
MiCS-4514 gas sensor https://www.dfrobot.com/product-2417.html
*/

#include "DFRobot_MICS.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CALIBRATION_TIME   3   

#define Mics_I2C_ADDRESS ADDRESS_0
DFRobot_MICS_I2C mics(&Wire, 0x75);

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 32  
#define OLED_RESET     -1  
#define SCREEN_ADDRESS 0x3C  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define imageWidthLogo 128
#define imageHeightLogo 32

const unsigned char bhopal [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 
  0x00, 0x01, 0xfc, 0x77, 0x8f, 0x8f, 0xc7, 0xe7, 0x80, 0xf8, 0x3e, 0x00, 0x3f, 0xf8, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x7b, 0x9f, 0xcf, 0xf7, 0xc7, 0x01, 0xfc, 0x3e, 0x00, 0x08, 0x20, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x7b, 0xdd, 0xef, 0x77, 0xe7, 0x81, 0xde, 0x7e, 0x00, 0x18, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0xef, 0x77, 0xbd, 0xef, 0x67, 0xe7, 0x83, 0xde, 0x6e, 0x00, 0x18, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x77, 0xbd, 0xee, 0x77, 0x67, 0x01, 0xdc, 0x7e, 0x00, 0x18, 0x20, 0x00, 0x00, 
  0x00, 0x01, 0xee, 0x7b, 0x9c, 0xef, 0x76, 0xe7, 0x81, 0xde, 0xee, 0x00, 0x18, 0x70, 0x00, 0x00, 
  0x00, 0x00, 0xfe, 0x7b, 0xdd, 0xef, 0x76, 0xe7, 0x81, 0xf8, 0xde, 0x00, 0x18, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xff, 0x7f, 0xbc, 0xef, 0xfe, 0x77, 0x81, 0xfc, 0xce, 0x00, 0x70, 0x18, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x7f, 0xbd, 0xef, 0xee, 0xf7, 0x03, 0xdf, 0x9e, 0x00, 0x60, 0x1c, 0x00, 0x00, 
  0x00, 0x00, 0xcf, 0x7b, 0xdd, 0xcf, 0x0e, 0x67, 0x81, 0xdf, 0xff, 0x00, 0x4f, 0xec, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x77, 0xbd, 0xee, 0x0f, 0xf7, 0x83, 0xcf, 0xff, 0x01, 0xdf, 0xe6, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x77, 0x9c, 0xef, 0x0f, 0xf7, 0x81, 0xdd, 0xff, 0x01, 0x9f, 0xf3, 0x00, 0x00, 
  0x00, 0x01, 0xe7, 0x7b, 0xdd, 0xce, 0x0e, 0x77, 0x03, 0xde, 0x0e, 0x01, 0xbf, 0xfb, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0x73, 0x9f, 0xcf, 0x0e, 0x77, 0xe0, 0xfc, 0x1e, 0x03, 0x12, 0x29, 0x80, 0x00, 
  0x00, 0x01, 0xfe, 0x7b, 0x87, 0x8f, 0x0e, 0x7f, 0xe0, 0x78, 0x0e, 0x07, 0xaa, 0xa5, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define ADC_PIN   A0
#define POWER_PIN 10

int measuresNumber=4;
int measuresCounter=0;
int measuresTimeFrame=1000; 
float defaultMin=50000;
int printReadings=0;
int timeStamp=0;
int timeStampIncrement=10;
int myCounter=0;

String line1="";
String line2="";
String line3="";

float co2Sum     = 0;
float ch4Sum     = 0;
float c2H5OHSum  = 0;
float h2Sum      = 0;
float nh3Sum     = 0;
float no2Sum     = 0;

float co2Min     = defaultMin;
float ch4Min     = defaultMin;
float c2H5OHMin  = defaultMin;
float h2Min      = defaultMin;
float nh3Min     = defaultMin;
float no2Min     = defaultMin;

float co2Max     = 0;
float ch4Max     = 0;
float c2H5OHMax  = 0;
float h2Max      = 0;
float nh3Max     = 0;
float no2Max     = 0;

float co2Avg     = 0;
float ch4Avg     = 0;
float c2H5OHAvg  = 0;
float h2Avg      = 0;
float nh3Avg     = 0;
float no2Avg     = 0;

void printScreen(){

      display.clearDisplay();
      display.setCursor(0, 0); 
      display.setTextSize(1);            
      display.setTextColor(SSD1306_WHITE); 
      display.println("Bhopal 84");
      display.println(line1); 
      display.println(line2); 
      display.println(line3); 
      display.display(); 
}

void setup() 
{

  // wait for serial
  Serial.begin(115200);
  //while(!Serial);

  Serial.println("Harmful gas TinyML started");
  Serial.println("Roni Bandini");

   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Loop forever
    }

    display.clearDisplay();   
    display.drawBitmap(0, 0,  bhopal, imageWidthLogo, imageHeightLogo, 1); 
    display.display();   
    delay(3000);
    
  while(!mics.begin()){
    Serial.println("No gas sensor detected");
    delay(1000);
  } Serial.println("Gas sensor connected");


  uint8_t mode = mics.getPowerState();
  
  if(mode == SLEEP_MODE){
    mics.wakeUpMode();
    Serial.println("Wake up sensor success");
  }else{
    Serial.println("The sensor is waking up");
  }

   line1="Calibrating";
   line2="sensor...";
   line3="";
   printScreen();       
   
   while(!mics.warmUpTime(CALIBRATION_TIME)){
    Serial.println("Warming up");
    delay(1000);
  }
  
}

void loop() 
{

  
  if (measuresCounter==measuresNumber){
    
    // reached limit, make calculations
    
    measuresCounter=0;
    myCounter++;

    // average
    co2Avg    =co2Sum/measuresNumber;
    ch4Avg    =ch4Sum/measuresNumber;
    c2H5OHAvg =c2H5OHSum/measuresNumber;
    h2Avg     =h2Sum/measuresNumber;
    nh3Avg    =nh3Sum/measuresNumber;
    no2Avg    =no2Sum/measuresNumber;
    line1="Series #"+String(myCounter);
    line2=String(co2Avg)+"-"+String(c2H5OHAvg);
    line3=String(h2Avg)+"-"+String(nh3Avg);
    printScreen();  

    /*
    Serial.println("---------------------------- Average ");
    Serial.print("Carbon Monox: ");
    Serial.print(co2Avg,1);

    Serial.print(" Methane: ");
    Serial.print(ch4Avg,1);
  
    Serial.print(" Ethanol: ");
    Serial.print(c2H5OHAvg,1);

    Serial.print(" Hydrogen: ");
    Serial.print(h2Avg,1);

    Serial.print(" Amonia: ");
    Serial.print(nh3Avg,1);

    Serial.print(" Nitrogen Diox: ");
    Serial.print(no2Avg,1);
    */

    // normalize min

    if (co2Min==defaultMin) {co2Min=0;};
    if (ch4Min==defaultMin) {ch4Min=0;};
    if (c2H5OHMin==defaultMin) {c2H5OHMin=0;};
    if (h2Min==defaultMin) {h2Min=0;};
    if (nh3Min==defaultMin) {nh3Min=0;};
    if (no2Min==defaultMin) {no2Min=0;};
    
    // print csv 

    Serial.println("");

    Serial.print(timeStamp);
    Serial.print(",");
    
    Serial.print(co2Avg);
    Serial.print(",");
    Serial.print(ch4Avg);
    Serial.print(",");
    Serial.print(c2H5OHAvg);
    Serial.print(",");
    Serial.print(h2Avg);
    Serial.print(",");
    Serial.print(nh3Avg);
    Serial.print(",");
    Serial.print(no2Avg);    

    Serial.print(",");
    Serial.print(co2Min);
    Serial.print(",");
    Serial.print(ch4Min);
    Serial.print(",");
    Serial.print(c2H5OHMin);
    Serial.print(",");
    Serial.print(h2Min);
    Serial.print(",");
    Serial.print(nh3Min);
    Serial.print(",");
    Serial.print(no2Min);  

    Serial.print(",");
    Serial.print(co2Max);
    Serial.print(",");
    Serial.print(ch4Max);
    Serial.print(",");
    Serial.print(c2H5OHMax);
    Serial.print(",");
    Serial.print(h2Max);
    Serial.print(",");
    Serial.print(nh3Max);
    Serial.print(",");
    Serial.print(no2Max);  

    delay(2000);

     // reset counters
     co2Sum     = 0;
     ch4Sum     = 0;
     c2H5OHSum  = 0;
     h2Sum      = 0;
     nh3Sum     = 0;
     no2Sum     = 0;
         
     co2Min     = defaultMin;
     ch4Min     = defaultMin;
     c2H5OHMin  = defaultMin;
     h2Min      = defaultMin;
     nh3Min     = defaultMin;
     no2Min     = defaultMin;
    
     co2Max     = 0;
     ch4Max     = 0;
     c2H5OHMax  = 0;
     h2Max      = 0;
     nh3Max     = 0;
     no2Max     = 0;

     timeStamp=timeStamp+timeStampIncrement;
     delay(measuresTimeFrame);

  }
  else
  {
    
    // increase sample counter
    measuresCounter++;

    // gets data from sensor
    float co2data     = mics.getGasData(CO);
    float ch4data     = mics.getGasData(CH4);
    float c2H5OHdata  = mics.getGasData(C2H5OH);
    float h2data      = mics.getGasData(H2);
    float nh3data     = mics.getGasData(NH3);
    float no2data     = mics.getGasData(NO2);

    // sum to calculate average
    co2Sum     = co2Sum+co2data;
    ch4Sum     = ch4Sum+ch4data;
    c2H5OHSum  = c2H5OHSum+c2H5OHdata;
    h2Sum      = h2Sum+h2data;
    nh3Sum     = nh3Sum+nh3data;
    no2Sum     = no2Sum+no2data;


    if (printReadings==1){
      
            Serial.println("----------------------- Plain Readings");
            Serial.print("Carbon Monoxide: ");
            Serial.print(co2data,1);
            Serial.println(" PPM");
        
            Serial.print("Methane: ");
            Serial.print(ch4data,1);
            Serial.println(" PPM");
            
            Serial.print("Ethanol: ");
            Serial.print(c2H5OHdata,1);
            Serial.println(" PPM");
          
            Serial.print("Hydrogen: ");
            Serial.print(h2data,1);
            Serial.println(" PPM");
          
            Serial.print("Amonia: ");
            Serial.print(nh3data,1);
            Serial.println(" PPM");
          
            Serial.print("Nitrogen Dioxide: ");
            Serial.print(no2data,1);
            Serial.println(" PPM");
            Serial.println("");
    }
    

    // higher than max
    
    if (co2data>co2Max){
      co2Max=co2data;      
      }

    if (ch4data>ch4Max){
      ch4Max=ch4data;      
      }

    if (c2H5OHdata>c2H5OHMax){
      c2H5OHMax=c2H5OHdata;      
      }  

    if (h2data>h2Max){
      h2Max=h2data;      
      } 

    if (nh3data>nh3Max){
      nh3Max=nh3data;      
      } 

    if (no2data>no2Max){
      no2Max=no2data;      
      } 

    // lower than min
    
    if (co2data>0 and (co2data<co2Min)){
      co2Min=co2data;      
      }

    if (ch4data>0 and (ch4data<ch4Min)){
      ch4Min=ch4data;      
      }

    if (c2H5OHdata>0 and (c2H5OHdata<c2H5OHMin)){
      c2H5OHMin=c2H5OHdata;      
      }  

    if (h2data>0 and (h2data<h2Min)){
      h2Min=h2data;      
      } 

    if (nh3data>0 and (nh3data<nh3Min)){
      nh3Min=nh3data;      
      } 

    if (no2data>0 and (no2data<no2Min)){
      no2Min=no2data;      
    } 
  
  
  }
  
 
  delay(measuresTimeFrame/measuresNumber);
}
