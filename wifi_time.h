#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

String get_ntp_time();
void printWifiStatus();
unsigned long sendNTPpacket(IPAddress& address);