/* ----------------------------------------------------------------------
"Pixel dust" Protomatter library example. As written, this is
SPECIFICALLY FOR THE ADAFRUIT MATRIXPORTAL with 64x32 pixel matrix.
Change "HEIGHT" below for 64x64 matrix. Could also be adapted to other
Protomatter-capable boards with an attached LIS3DH accelerometer.

PLEASE SEE THE "simple" EXAMPLE FOR AN INTRODUCTORY SKETCH,
or "doublebuffer" for animation basics.
------------------------------------------------------------------------- */
#include <Arduino.h>
#include <Wire.h>                 // For I2C communication
#include <Adafruit_LIS3DH.h>      // For accelerometer
#include <Adafruit_PixelDust.h>   // For sand simulation
#include <Adafruit_Protomatter.h> // For RGB matrix

#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
#define MAX_FPS 45 // Maximum redraw rate, frames/second

extern uint8_t rgbPins[];
extern uint8_t addrPins[];
extern uint8_t clockPin;
extern uint8_t latchPin;
extern uint8_t oePin;

Adafruit_Protomatter dustmatrix(
  WIDTH, 4, 1, rgbPins, 3, addrPins,
  clockPin, latchPin, oePin, true);

Adafruit_LIS3DH accel = Adafruit_LIS3DH();

#define N_COLORS   8
#define BOX_HEIGHT 8
#define N_GRAINS (BOX_HEIGHT*N_COLORS*8)
uint16_t colors[N_COLORS];

Adafruit_PixelDust sand(WIDTH, HEIGHT, N_GRAINS, 1, 128, false);

uint32_t prevTime = 0; // Used for frames-per-second throttle

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void err(int x) {
  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
  for(i=1;;i++) {                     // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
    delay(x);
  }
}

void pixel_dust_setup(void) {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  ProtomatterStatus status = dustmatrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);

  if (!sand.begin()) {
    Serial.println("Couldn't start sand");
    err(1000); // Slow blink = malloc error
  }

  if (!accel.begin(0x19)) {
    Serial.println("Couldn't find accelerometer");
    err(250);  // Fast bink = I2C error
  }
  accel.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  //sand.randomize(); // Initialize random sand positions

  // Set up initial sand coordinates, in 8x8 blocks
  int n = 0;
  for(int i=0; i<N_COLORS; i++) {
    int xx = i * WIDTH / N_COLORS;
    int yy =  HEIGHT - BOX_HEIGHT;
    for(int y=0; y<BOX_HEIGHT; y++) {
      for(int x=0; x < WIDTH / N_COLORS; x++) {
        //Serial.printf("#%d -> (%d, %d)\n", n,  xx + x, yy + y);
        sand.setPosition(n++, xx + x, yy + y);
      }
    }
  }
  Serial.printf("%d total pixels\n", n);

  colors[0] = dustmatrix.color565(64, 64, 64);  // Dark Gray
  colors[1] = dustmatrix.color565(120, 79, 23); // Brown
  colors[2] = dustmatrix.color565(228,  3,  3); // Red
  colors[3] = dustmatrix.color565(255,140,  0); // Orange
  colors[4] = dustmatrix.color565(255,237,  0); // Yellow
  colors[5] = dustmatrix.color565(  0,128, 38); // Green
  colors[6] = dustmatrix.color565(  0, 77,255); // Blue
  colors[7] = dustmatrix.color565(117,  7,135); // Purple
}

// MAIN LOOP - RUNS ONCE PER FRAME OF ANIMATION ----------------------------

void pixel_dust_loop() {
  // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
  // calculations are non-deterministic (don't always take the same amount
  // of time, depending on their current states), this helps ensure that
  // things like gravity appear constant in the simulation.
  uint32_t t;
  while(((t = micros()) - prevTime) < (1000000L / MAX_FPS));
  prevTime = t;

  // Read accelerometer...
  sensors_event_t event;
  accel.getEvent(&event);
  //Serial.printf("(%0.1f, %0.1f, %0.1f)\n", event.acceleration.x, event.acceleration.y, event.acceleration.z);

  double xx, yy, zz;
  xx = event.acceleration.x * 1000;
  yy = event.acceleration.y * 1000;
  zz = event.acceleration.z * 1000;

  // Run one frame of the simulation
  sand.iterate(xx, yy, zz);

  //sand.iterate(-accel.y, accel.x, accel.z);

  // Update pixel data in LED driver
  dimension_t x, y;
  dustmatrix.fillScreen(0x0);
  for(int i=0; i<N_GRAINS ; i++) {
    sand.getPosition(i, &x, &y);
    int n = i / ((WIDTH / N_COLORS) * BOX_HEIGHT); // Color index
    uint16_t flakeColor = colors[n];
    dustmatrix.drawPixel(x, y, flakeColor);
    //Serial.printf("(%d, %d)\n", x, y);
  }
  dustmatrix.show(); // Copy data to dustmatrix buffers
}
