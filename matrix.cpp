#include <Arduino.h>
#include <Adafruit_Protomatter.h>

extern double dimmer;
uint8_t rgbPins[] = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20, 21};
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;

int WIDTH = 64;
int HEIGHT = 32;
int MAX_FPS = 45;


Adafruit_Protomatter matrix(
    WIDTH,                        // Width of matrix (or matrix chain) in pixels
    6,                         // Bit depth, 1-6
    1, rgbPins,                // # of matrix chains, array of 6 RGB pins for each
    4, addrPins,               // # of address pins (height is inferred), array of pins
    clockPin, latchPin, oePin, // Other matrix control pins
    false);                    // No double-buffering here (see "doublebuffer" example)

void screen_setup(){
 // Serial.begin(9600);
  Serial.println("screen_setup");
  
  ProtomatterStatus status = matrix.begin();
  /*Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK)
  {
      // DO NOT CONTINUE if matrix setup encountered an error.
      for (;;)
          ;
  }*/

  matrix.setFont();
    // Make four color bars (red, green, blue, white) with brightness ramp:
/*
    for (int x = 0; x < matrix.width(); x++)
    {
        uint8_t level = x * 256 / matrix.width(); // 0-255 brightness
        matrix.drawPixel(x, matrix.height() - 4, matrix.color565(level, 0, 0));
        matrix.drawPixel(x, matrix.height() - 3, matrix.color565(0, level, 0));
        matrix.drawPixel(x, matrix.height() - 2, matrix.color565(0, 0, level));
        matrix.drawPixel(x, matrix.height() - 1, matrix.color565(level, level, level));
    }*/
    
    // *** TIME BOX ** //
    // start X, start y, length x, length y
    
   // matrix.drawRect(16, 6, 33, 11, matrix.color565(0, 6, 6)); //shadowbox
    //matrix.drawRect(15, 5, 33, 11, matrix.color565(0, 6, 6));
    matrix.drawRect(15, 10, 33, 11, matrix.color565(0, 60 * dimmer+0.1, 0)); //always show text, even if dimmer == 0
    //matrix.fillRect(16, 11, 31, 9, matrix.color565(0, 0, 0)); 
    //matrix.drawFastHLine(18, 16,31, matrix.color565(0, 0, 0)); //remove shadow lines
    //matrix.drawFastVLine(48, 8, 8, matrix.color565(0, 0, 0));   

//height 0-31; center = 15
//width 0-63; center = 31
// textbox  11x33
    matrix.show();
}


void write_text_to_matrix(String message)
{
   // Serial.println("printing" + message);
    matrix.drawRect(15, 10, 33, 11, matrix.color565(0, 60 * dimmer, 0)); //main box
    matrix.fillRect(16, 11, 31, 9, matrix.color565(0, 0, 0)); 
    matrix.setCursor(17, 12);
    matrix.setTextColor(matrix.color565(0, 60 * dimmer, 0));
    matrix.print(message);
    matrix.show(); // Copy data to matrix buffers
    matrix.show();
}
