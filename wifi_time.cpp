#include "wifi_time.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

#include "arduino_secrets.h"

int status = WL_IDLE_STATUS;
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;          // your network key index number (needed only for WEP)

unsigned int localPort = 2390;            // local port to listen for UDP packets
IPAddress timeServer(162, 159, 200, 123); // pool.ntp.org NTP server
const int NTP_PACKET_SIZE = 48;           // NTP timestamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];       // buffer to hold incoming and outgoing packets
const int timeZone = -8;                  // Pacific Standard Time (USA)

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

bool time_setup()
{
  // Open serial communications and wait for port to open:
  /*Serial.begin(9600);
  while (!Serial)
    ;*/
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  return true;
}

time_t getNtpTime()
{

  if (WiFi.status() == WL_NO_MODULE)
  {
    // Serial.println("Communication with WiFi module failed!");
    // return "error";
    //  don't continue
    while (true)
      ;
  }

  while (status != WL_CONNECTED)
  {
    // Serial.print("Attempting to connect to SSID: ");
    // Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    // delay(10000);
  }

  // Serial.println("Connected to WiFi");
  printWifiStatus();

  // Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  sendNTPpacket(timeServer); // send an NTP packet to a time server
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500)
  {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE)
    {
      // Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 = (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * 3600;
    }
  }
  return 0;
}

time_t prevDisplay = 0; // when the digital clock was displayed
time_t prevMinute = 0;

String time_loop()
{
  // Serial.println("let's do the time loop again");
  // Serial.println(timeStatus());
  // Serial.println(timeNotSet);

  // if (timeStatus() != timeNotSet) {
  //  if (minute() != prevMinute) { //update the display only if time has changed in the last X seconds
  return digitalClockDisplay();
  //  } else return prevDisplay;
  // }
}

String digitalClockDisplay()
{
  prevDisplay = now();
  prevMinute = minute();

  int shortHour = hour();
  if (hour() > 12)
  {
    shortHour = shortHour - 12;
  }

  // Serial.println(shortHour);
  //  digital clock display of the time
  String minutes = printDigits(minute());
  String hours = printDigits(shortHour);
  /*Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
  Serial.println(hour() + ":" + minute());
  return hour() + ":" + minute();*/
  return hours + ":" + minutes;
}

String printDigits(int digits)
{
  // Serial.println("digits" + digits);
  //  utility for digital clock display: prints leading 0
  if (digits < 10)
    return "0" + String(digits);
  else
    return String(digits);
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0;          // Stratum, or type of clock
  packetBuffer[2] = 6;          // Polling Interval
  packetBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  // Serial.print("IP Address: ");
  // Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  // Serial.print("signal strength (RSSI):");
  // Serial.print(rssi);
  // Serial.println(" dBm");
}
