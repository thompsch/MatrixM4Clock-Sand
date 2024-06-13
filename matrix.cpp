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
  ProtomatterStatus status = matrix.begin();
  matrix.setFont();
  matrix.drawRect(15, 10, 33, 11, matrix.color565(0, 60 * dimmer+0.2, 0)); //always show text, even if dimmer == 0
  matrix.show();
}


void write_text_to_matrix(String message)
{   
  Serial.println("Writing message: " + message);
    matrix.drawRect(15, 10, 33, 11, matrix.color565(0, 60 * dimmer+0.2, 0)); //main box
    matrix.fillRect(16, 11, 31, 9, matrix.color565(0, 0, 0)); 
    matrix.setCursor(17, 12);
    matrix.setTextColor(matrix.color565(0, 60 * dimmer+0.2, 0));
    matrix.print(message);
    matrix.show(); // Copy data to matrix buffers
}
