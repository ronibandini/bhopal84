/*!
Bhopal 84 gas detector
MIT License
Roni Bandini June 2022 - @RoniBandini
TinyML via Edge Impulse
Oled Screen VCC to Arduino 3.3v, GND to Arduino GND, SDA to A4, SCL to A5
MiCS-4514 Gravity Gas Sensor to Arduino 3.3v, GND, SDA to A4, SCL to A5
*/

#include "DFRobot_MICS.h"
#include <Bhopal84_inferencing.h>
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
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x38, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xfc, 0x73, 0x87, 0x8f, 0xc3, 0xc7, 0x00, 0xf8, 0x3e, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x73, 0x9f, 0xcf, 0xe7, 0xc7, 0x01, 0xfc, 0x3e, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xee, 0x73, 0x9d, 0xee, 0x77, 0xe7, 0x01, 0xdc, 0x3e, 0x00, 0x60, 0x30, 0x00, 0x00, 
  0x00, 0x01, 0xee, 0x73, 0x9d, 0xee, 0x77, 0xe7, 0x01, 0xdc, 0x7e, 0x00, 0x60, 0x38, 0x00, 0x00, 
  0x00, 0x01, 0xee, 0x73, 0x9d, 0xee, 0x77, 0xe7, 0x01, 0xdc, 0x6e, 0x00, 0x60, 0x18, 0x00, 0x00, 
  0x00, 0x01, 0xee, 0x73, 0x9d, 0xee, 0x76, 0x67, 0x01, 0xdc, 0x4e, 0x00, 0xe0, 0x1c, 0x00, 0x00, 
  0x00, 0x01, 0xfc, 0x73, 0x9d, 0xee, 0x76, 0x67, 0x00, 0xf8, 0xce, 0x00, 0xc0, 0x0c, 0x00, 0x00, 
  0x00, 0x01, 0xfe, 0x7f, 0x9d, 0xef, 0xf6, 0x67, 0x01, 0xfc, 0xce, 0x01, 0x80, 0x0e, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x7f, 0x9d, 0xef, 0xee, 0x77, 0x01, 0xde, 0x8e, 0x03, 0x80, 0x06, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x73, 0x9d, 0xee, 0x0e, 0x77, 0x01, 0xdf, 0xfe, 0x03, 0x00, 0x07, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x73, 0x9d, 0xee, 0x0f, 0xf7, 0x01, 0xdf, 0xfe, 0x07, 0x3f, 0xe3, 0x00, 0x00, 
  0x00, 0x01, 0xef, 0x73, 0x9d, 0xee, 0x0f, 0xf7, 0x01, 0xdf, 0xfe, 0x06, 0x3f, 0xf3, 0x80, 0x00, 
  0x00, 0x01, 0xef, 0x73, 0x9d, 0xee, 0x0e, 0x77, 0x01, 0xdc, 0x0e, 0x0e, 0x7f, 0xf9, 0x80, 0x00, 
  0x00, 0x01, 0xfe, 0x73, 0x9f, 0xce, 0x0e, 0x77, 0xe1, 0xfc, 0x0e, 0x0c, 0x7f, 0xf9, 0xc0, 0x00, 
  0x00, 0x01, 0xfe, 0x73, 0x87, 0x8e, 0x0e, 0x77, 0xe0, 0xf8, 0x0e, 0x1c, 0xff, 0xfc, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xff, 0xfc, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xff, 0xfe, 0x60, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x70, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char edge [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0b, 0xb6, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x92, 0x40, 0x0f, 0xe7, 0xc0, 0x78, 0xfe, 0x0c, 0x40, 0x8f, 0x84, 0x11, 0x01, 0xf1, 0xfc, 
  0x08, 0x00, 0x10, 0x07, 0xe7, 0xe1, 0xfc, 0xfe, 0x0c, 0xe1, 0xdf, 0xee, 0x3b, 0x03, 0xf9, 0xf8, 
  0x00, 0x00, 0x00, 0x0c, 0x06, 0x31, 0x80, 0x40, 0x0e, 0xe3, 0x88, 0xe6, 0x33, 0x83, 0x01, 0x80, 
  0x04, 0x80, 0x00, 0x0c, 0x06, 0x3b, 0x00, 0xc0, 0x04, 0x73, 0xdc, 0x6c, 0x33, 0x03, 0x81, 0x80, 
  0x02, 0x34, 0x50, 0x0f, 0xc6, 0x1b, 0x0c, 0xfc, 0x0c, 0xf7, 0x98, 0x6e, 0x33, 0x01, 0xf1, 0xf8, 
  0x02, 0x92, 0x08, 0x0f, 0xc6, 0x33, 0x3c, 0xfc, 0x0c, 0x7e, 0x8f, 0xc4, 0x33, 0x80, 0x79, 0xf0, 
  0x00, 0x00, 0x00, 0x04, 0x06, 0x1b, 0x8c, 0xc0, 0x0c, 0xdd, 0xdf, 0x8e, 0x33, 0x00, 0x19, 0x80, 
  0x00, 0x00, 0x00, 0x0d, 0x26, 0xf1, 0xce, 0x60, 0x0e, 0xc5, 0x98, 0x07, 0x73, 0x42, 0x99, 0x88, 
  0x15, 0x28, 0xa4, 0x0f, 0xe7, 0xe0, 0xfc, 0xfe, 0x0c, 0xc1, 0x9c, 0x03, 0xf3, 0xfb, 0xf9, 0xfc, 
  0x0d, 0xae, 0xb6, 0x06, 0xc5, 0x40, 0x50, 0x7a, 0x04, 0xc0, 0x80, 0x01, 0x41, 0x51, 0xa1, 0xb0, 
  0x12, 0x92, 0xa2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x44, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

# define ADC_PIN   A0
# define POWER_PIN 10
# define pinBuzzer 2

int   measuresNumber=4;
int   measuresCounter=0;
int   measuresTimeFrame=1500; // 1.5 seconds
float defaultMin=50000;
int   printReadings=1;
float scoreLimit=0.80;

String line1="";
String line2="";

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

float myScoreHarmful = 0;
float myScoreRegular = 0;

// Axis array
float features[18];

int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void printScreen(){

      display.clearDisplay();
      display.setCursor(0, 0); 
      display.setTextSize(1);            
      display.setTextColor(SSD1306_WHITE); 
      display.println("Bhopal 84");
      display.println("Roni Bandini"); 
      display.println(line1); 
      display.println(line2); 
      display.display(); 
}
  
// buzzer beep
void myBeep(){     
     tone(pinBuzzer, 349, 500);
     delay(500);                        
     tone(pinBuzzer, 200, 500);
     delay(150); 
     tone(pinBuzzer, 150, 500);
     delay(500);   
     noTone(pinBuzzer);
}


void setup()
{
    pinMode(pinBuzzer, OUTPUT);       
    
    // wait for serial  
    Serial.begin(115200);
    delay(5000);
    //while(!Serial);

    Serial.println("Bhopal 84");
    Serial.println("TinyML via Edge Impulse");
    Serial.println("Roni Bandini, June 2022");
    Serial.println("@RoniBandini");
    Serial.println("------------------------------------");

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Loop forever
    }

    display.clearDisplay();   
    display.drawBitmap(0, 0,  bhopal, imageWidthLogo, imageHeightLogo, 1); 
    display.display();   
    delay(3000);
    
    display.clearDisplay();   
    display.drawBitmap(0, 0,  edge, imageWidthLogo, imageHeightLogo, 1); 
    display.display();   
    delay(3000);

    while(!mics.begin()){
    Serial.println("No gas sensor detected");
    delay(1000);
  } Serial.println("Gas sensor connected");

   line1="Calibrating...";
   printScreen();    
   delay(2000);   

  uint8_t mode = mics.getPowerState();
  
  if(mode == SLEEP_MODE){
    mics.wakeUpMode();
    Serial.println("Wake up sensor success");
  }else{
    Serial.println("The sensor is waking up");
  }

   while(!mics.warmUpTime(CALIBRATION_TIME)){
    Serial.println("Calibrating...");
    delay(1000);
  }

   myBeep(); 
   
   line1="Sensor ready...";
   line2="";
   printScreen();    
   delay(2000); 
    
}


void loop()
{

    if (measuresCounter==measuresNumber){
    
      // reached limit, make calculations and inference
      
      measuresCounter=0;
  
      // average
      co2Avg    =co2Sum/measuresNumber;
      ch4Avg    =ch4Sum/measuresNumber;
      c2H5OHAvg =c2H5OHSum/measuresNumber;
      h2Avg     =h2Sum/measuresNumber;
      nh3Avg    =nh3Sum/measuresNumber;
      no2Avg    =no2Sum/measuresNumber;
  
  
      // normalize min
  
      if (co2Min==defaultMin) {co2Min=0;};
      if (ch4Min==defaultMin) {ch4Min=0;};
      if (c2H5OHMin==defaultMin) {c2H5OHMin=0;};
      if (h2Min==defaultMin) {h2Min=0;};
      if (nh3Min==defaultMin) {nh3Min=0;};
      if (no2Min==defaultMin) {no2Min=0;};
      
       // send to inference

       if (printReadings==1){
      
            Serial.println("----------------------- Sending to inference");            
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
            Serial.println(",");


            Serial.print(co2Min,1);            
            Serial.print(",");            
                    
            Serial.print(ch4Min,1);           
            Serial.print(",");            
                        
            Serial.print(c2H5OHMin,1);   
            Serial.print(",");                     
                      
            Serial.print(h2Min,1);       
            Serial.print(",");                 
                    
            Serial.print(nh3Min,1);      
            Serial.print(",");                  
                    
            Serial.print(no2Min,1);            
            Serial.println(",");


            Serial.print(co2Max,1);            
            Serial.print(",");            
                    
            Serial.print(ch4Max,1);           
            Serial.print(",");            
                        
            Serial.print(c2H5OHMax,1);   
            Serial.print(",");                     
                      
            Serial.print(h2Max,1);       
            Serial.print(",");                 
                    
            Serial.print(nh3Max,1);      
            Serial.print(",");                  
                    
            Serial.print(no2Max,1);            
            Serial.println("");
            
       } // print values sent to inference
  
       features[0] = co2Avg;
       features[1] = ch4Avg;
       features[2] = c2H5OHAvg;
       features[3] = h2Avg;
       features[4] = nh3Avg;
       features[5] = no2Avg;
        
       features[6]  = co2Min;
       features[7]  = ch4Min;
       features[8]  = c2H5OHMin;
       features[9]  = h2Min;
       features[10] = nh3Min;
       features[11] = no2Min;
  
       features[12] = co2Max;
       features[13] = ch4Max;
       features[14] = c2H5OHMax;
       features[15] = h2Max;
       features[16] = nh3Max;
       features[17] = no2Max;
  
  
       
      // Check no sensor readings
      
      if (features[0] ==0 and features[1] ==0 and features[2] ==0 and features[3] ==0 and features[4] ==0 and features[5] ==0){
        Serial.println("All gas readings 0");
  
         line1="All readings are 0";
         line2="No leaks detected";
         printScreen();  
         delay(2000);
                
        }
        else
        {
          Serial.println("Gas readings ok");               
        
          if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
              ei_printf("The size of your 'features' array is not correct. Expected %lu items, but had %lu\n",
                  EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
              delay(1000);
              return;
          }
      
        ei_impulse_result_t result = { 0 };
    
        // the features are stored into flash, and we don't want to load everything into RAM
        signal_t features_signal;
        features_signal.total_length = sizeof(features) / sizeof(features[0]);
        features_signal.get_data = &raw_feature_get_data;
    
        // invoke the impulse
        EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
        ei_printf("run_classifier returned: %d\n", res);
      
        if (res != 0) return;
    
        // print the predictions
        ei_printf("Predictions ");
        ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
            result.timing.dsp, result.timing.classification, result.timing.anomaly);
        ei_printf(": \n");
        ei_printf("[");
        
       for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
              ei_printf("%.5f", result.classification[ix].value);
              
              #if EI_CLASSIFIER_HAS_ANOMALY == 1
                ei_printf(", ");
              #else
                if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
                  ei_printf(", ");
                }
              #endif
         }
          
          #if EI_CLASSIFIER_HAS_ANOMALY == 1
              ei_printf("%.3f", result.anomaly);
          #endif
              ei_printf("]\n");

          myScoreRegular = 0;
          myScoreHarmful = 0;
          
          // human-readable predictions
          for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
              ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
                              
  
               if (result.classification[ix].label=="regular"){
                myScoreRegular=result.classification[ix].value;                             
               } // regular

                           
              if (result.classification[ix].label=="harmful"){
                myScoreHarmful=result.classification[ix].value;                                        
              } // harmful
  

                                    
          } // iterate labels

          if (myScoreHarmful<scoreLimit and myScoreRegular<scoreLimit){
  
                    line1="Harmful : " + String(myScoreHarmful*100)+"%";
                    line2="Regular : " + String(myScoreRegular*100)+"%";
                    printScreen();    
              
           }

           if (myScoreRegular>scoreLimit){
  
                  // regular gas detected
                  
                  myBeep();
                  
                  line1="Regular gas";
                  line2=String(myScoreRegular*100)+"%";
                  printScreen();    
  
                  delay(5000);
                  
          }

           if (myScoreHarmful>scoreLimit){
  
                    // harmful gas detected
                    
                    myBeep();
                    
                    line1="Harmful gas";
                    line2=String(myScoreHarmful*100)+"%";
                    printScreen();    
  
                    delay(5000);
                   
              }
          
          #if EI_CLASSIFIER_HAS_ANOMALY == 1
              ei_printf("    anomaly score: %.3f\n", result.anomaly);
          #endif
  
                  
          }// gas readings ok
  
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
      
            Serial.println("----------------------- Plain Readings "+String(measuresCounter));            
            Serial.print(co2data);            
            Serial.print(",");            
                    
            Serial.print(ch4data);           
            Serial.print(",");            
                        
            Serial.print(c2H5OHdata);   
            Serial.print(",");                     
                      
            Serial.print(h2data);       
            Serial.print(",");                 
                    
            Serial.print(nh3data);      
            Serial.print(",");                  
                    
            Serial.print(no2data);            
            Serial.println("");
            
    } // print readings
    

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
