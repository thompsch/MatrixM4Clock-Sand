#include "pixel_dust.h"
#include "matrix.h"
#include "wifi_time.h"

double dimmer = 1;
double dimmerArray[4]= {1,0.2,0.1,0};
int dimmerIndex = 0;

int BUTTON_UP = 2;
int BUTTON_DOWN = 3;

unsigned long lastDownDebounceTime = 0;
int lastDownButtonState = HIGH;
int downButtonState = HIGH;

unsigned long lastUpDebounceTime = 0;
int lastUpButtonState = HIGH;
int upButtonState = HIGH;

//bool timeLoopStarted = false;

void setup()
{
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pixel_dust_setup();
  write_text_to_matrix("SIRIS");
  time_setup();
  Serial.println(getLatestTime());
  write_text_to_matrix(getLatestTime());
}

void loop()
{
  pixel_dust_loop();
  write_text_to_matrix(getLatestTime());
  // Debouncers
  int sensorDownValue = digitalRead(BUTTON_DOWN);
  if (sensorDownValue != lastDownButtonState) {
    if ((millis() - lastDownDebounceTime) > 300) {
      lastDownDebounceTime = millis();
      if (sensorDownValue != downButtonState) {
        sensorDownValue = downButtonState;
        if (dimmerIndex == 3) dimmerIndex = 0;
        else dimmerIndex++; //should loop through 4-value array
        dimmer = dimmerArray[dimmerIndex];
      }
    }
  }
}
