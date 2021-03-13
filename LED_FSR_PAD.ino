#include <Adafruit_DotStar.h>

#define DATAPIN 7
#define CLOCKPIN 6

#define SENSORS 5

#define NUM_PIX0 18
#define NUM_PIX1 18
#define NUM_PIX2 16
#define NUM_PIX3 18
#define NUM_PIX4 18

#define TOP 0xCE2029
#define TOP_TOP_MIDDLE 0xDB481F
#define TOP_MIDDLE 0xE77015
#define TOP_MIDDLE_MIDDLE 0xF3980B
#define MIDDLE 0xFFC000
#define BOTTOM_MIDDLE_MIDDLE 0xC59D29
#define BOTTOM_MIDDLE 0x8A7A52
#define BOTTOM_BOTTOM_MIDDLE 0x4F577B
#define BOTTOM 0x1434A4

int SENSOR[] = {5,4,3,2,1};
int SENSOR_DATA[] = {2, 4, 7, 8, 10};
int SENSOR_CLOCK[] = {3, 5, 6, 9, 11};
int NUMPIXELS[] = {NUM_PIX0, NUM_PIX1, NUM_PIX2, NUM_PIX3, NUM_PIX4};

//q, z, s, c, e
uint8_t bindings[SENSORS] = {0x14, 0x1D, 0x16, 0x06, 0x08};
//USB HID
uint8_t keyBuffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0};

Adafruit_DotStar strip0(NUMPIXELS[0], SENSOR_DATA[0], SENSOR_CLOCK[0], DOTSTAR_BGR);
Adafruit_DotStar strip1(NUMPIXELS[1], SENSOR_DATA[1], SENSOR_CLOCK[1], DOTSTAR_BGR);
Adafruit_DotStar strip2(NUMPIXELS[2], SENSOR_DATA[2], SENSOR_CLOCK[2], DOTSTAR_BGR);
Adafruit_DotStar strip3(NUMPIXELS[3], SENSOR_DATA[3], SENSOR_CLOCK[3], DOTSTAR_BGR);
Adafruit_DotStar strip4(NUMPIXELS[4], SENSOR_DATA[4], SENSOR_CLOCK[4], DOTSTAR_BGR);

// Remember colors are ALWAYS clockwise
uint32_t color0[] = { TOP_TOP_MIDDLE, //Right
                       TOP_TOP_MIDDLE,
                       TOP_MIDDLE,
                       TOP_MIDDLE,
                       TOP_MIDDLE_MIDDLE,
                       TOP_MIDDLE_MIDDLE, //Bottom
                       TOP_MIDDLE_MIDDLE,
                       TOP_MIDDLE,
                       TOP_MIDDLE,
                       TOP_MIDDLE, // left
                       TOP_TOP_MIDDLE,
                       TOP_TOP_MIDDLE,
                       TOP,
                       TOP,
                       TOP, //Top
                       TOP,
                       TOP_TOP_MIDDLE,
                       TOP_TOP_MIDDLE };

uint32_t color1[] = { BOTTOM_MIDDLE_MIDDLE, //Right
                       BOTTOM_MIDDLE_MIDDLE,
                       BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE, //Bottom
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM,
                       BOTTOM,
                       BOTTOM, // left
                       BOTTOM,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE, //Top
                       BOTTOM_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE };

uint32_t color2[] = { TOP_MIDDLE_MIDDLE, //Right
                       MIDDLE,
                       MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE, //Bottom
                       MIDDLE, 
                       MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE, // left
                       MIDDLE, 
                       MIDDLE,
                       TOP_MIDDLE_MIDDLE,
                       TOP_MIDDLE_MIDDLE, //Top
                       MIDDLE,
                       MIDDLE,
                       TOP_MIDDLE_MIDDLE,
                       0,
                       0};

uint32_t color3[] = { BOTTOM_BOTTOM_MIDDLE, //Right
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM,
                       BOTTOM,
                       BOTTOM, //Bottom
                       BOTTOM,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE, // left
                       BOTTOM_BOTTOM_MIDDLE,
                       BOTTOM_MIDDLE,
                       BOTTOM_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE,
                       BOTTOM_MIDDLE_MIDDLE, //Top
                       BOTTOM_MIDDLE,
                       BOTTOM_MIDDLE,
                       BOTTOM_BOTTOM_MIDDLE };

uint32_t color4[] = {  TOP, //Rigbt
                       TOP,
                       TOP_TOP_MIDDLE,
                       TOP_TOP_MIDDLE,
                       TOP_MIDDLE,
                       TOP_MIDDLE, //Bottom
                       TOP_MIDDLE,
                       TOP_MIDDLE_MIDDLE,
                       TOP_MIDDLE_MIDDLE,
                       TOP_MIDDLE_MIDDLE,//LEFT
                       TOP_MIDDLE,
                       TOP_MIDDLE,
                       TOP_TOP_MIDDLE,
                       TOP_TOP_MIDDLE,
                       TOP_TOP_MIDDLE, //Top
                       TOP_TOP_MIDDLE,
                       TOP,
                       TOP };


int i, current;

int LIMITS[]= {330, 305, 295, 270, 270};

Adafruit_DotStar* strips[SENSORS] = {&strip0, &strip1, &strip2, &strip3, &strip4};

uint32_t (*colors[])[18] = {&color0, &color1, &color2, &color3, &color4};

#define DEBUG false

void setup() {
  // put your setup code here, to run once:
  for ( int i = 0; i < SENSORS; ++i) {
    strips[i]->begin();
    // set pixels off
    for ( int j = 0 ; j < NUMPIXELS[i]; ++j) {
      strips[i]->setPixelColor(j, 0, 0, 0);
    }
    strips[i]->show();
  }

 
  Serial.begin(9600);

}

void loop() {
  #if DEBUG
    Serial.print("Analog reading = ");
  #endif
  for (i = 0; i < SENSORS; ++i) {
    current = analogRead(SENSOR[i]);
    #if DEBUG
      Serial.print(current);
      Serial.print(" - ");
    #endif
    switch(keyBuffer[2+i]) {
      case 0:
        if (current >= LIMITS[i]) {
          keyBuffer[2+i] = bindings[i];
          // Turn on lights for that switch
          for ( int j = 0 ; j < NUMPIXELS[i]; ++j) {
            strips[i]->setPixelColor(j, (*colors[i])[j]);
          }
          strips[i]->show();
        }
        break;
      default:
        if (current < LIMITS[i]) {
          keyBuffer[2+i] = 0;
          // Turn lights off for that switch
          for ( int j = 0 ; j < NUMPIXELS[i]; ++j) {
            strips[i]->setPixelColor(j, 0, 0, 0);
          }
          strips[i]->show();
        }
        break;
    }
  }
  #if DEBUG
    Serial.println("");
  #else
    Serial.write(keyBuffer, 8);
  #endif
}
