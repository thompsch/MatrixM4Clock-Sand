#include "pixel_dust.h"
#include "matrix.h"
#include "wifi_time.h"

double dimmer = 1;
double dimmerArray[3]= {1,0.2,0};
int dimmerIndex = 0;

int BUTTON_UP = 2;
int BUTTON_DOWN = 3;

unsigned long lastDownDebounceTime = 0;
int lastDownButtonState = HIGH;
int downButtonState = HIGH;

unsigned long lastUpDebounceTime = 0;
int lastUpButtonState = HIGH;
int upButtonState = HIGH;

void setup()
{
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pixel_dust_setup();
  write_text_to_matrix("SIRIS");
  time_setup();
}

void loop()
{
  pixel_dust_loop();

  String time = time_loop();

  write_text_to_matrix(time);

  // Debouncers
  int sensorDownValue = digitalRead(BUTTON_DOWN);
  if (sensorDownValue != lastDownButtonState) {
    if ((millis() - lastDownDebounceTime) > 300) {
      lastDownDebounceTime = millis();
      if (sensorDownValue != downButtonState) {
        sensorDownValue = downButtonState;
        if (dimmerIndex == 2) dimmerIndex = 0;
        else dimmerIndex++; //should loop through 3-value array]
        dimmer = dimmerArray[dimmerIndex];
      }
    }
  }
}
