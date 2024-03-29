#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

bool time_setup();
time_t getNtpTime();
String time_loop();
void sendNTPpacket(IPAddress& address);
void printWifiStatus();
String printDigits(int digits);
String digitalClockDisplay();

/*void time_setup();
void time_loop();
void digitalClockDisplay();
void printDigits(int digits);
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);*/