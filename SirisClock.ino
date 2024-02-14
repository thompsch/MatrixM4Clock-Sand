#include "pixel_dust.h"
#include "simple_text.h"
#include "wifi_time.h"
#include <Time.h>

void setup()
{
  // pixel_dust_setup();
  write_text_to_matrix(time.now());
  time = get_ntp_time();
  write_text_to_matrix(time);
}

void loop()
{
  // pixel_dust_loop();
  simple_text_loop();
  delay(1000);
}
