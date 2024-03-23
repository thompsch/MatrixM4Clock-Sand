/* ----------------------------------------------------------------------
"Pixel dust" Protomatter library example. As written, this is
SPECIFICALLY FOR THE ADAFRUIT matrixPORTAL with 64x32 pixel matrix.
Change "HEIGHT" below for 64x64 matrix. Could also be adapted to other
Protomatter-capable boards with an attached LIS3DH accelerometer.

PLEASE SEE THE "simple" EXAMPLE FOR AN INTRODUCTORY SKETCH,
or "doublebuffer" for animation basics.
------------------------------------------------------------------------- */

#include <Wire.h>                 // For I2C communication
#include <Adafruit_LIS3DH.h>      // For accelerometer
#include <Adafruit_PixelDust.h>   // For sand simulation
#include <Adafruit_Protomatter.h> // For RGB matrix

#define HEIGHT 32  // matrix height (pixels) - SET TO 64 FOR 64x64 matrix!
#define WIDTH 64   // matrix width (pixels)
#define MAX_FPS 45 // Maximum redraw rate, frames/second

extern Adafruit_Protomatter matrix;
extern double dimmer;
bool isStopped = false;

Adafruit_LIS3DH accel = Adafruit_LIS3DH();

#define N_COLORS 4
#define BOX_HEIGHT 8
#define N_GRAINS (BOX_HEIGHT * N_COLORS * 16)
uint16_t colors[N_COLORS];

Adafruit_PixelDust sand(WIDTH, HEIGHT, N_GRAINS, 1, 128, false);

uint32_t prevTime = 0; // Used for frames-per-second throttle

void setColors() {

//try this library sometime:
//https://github.com/newdigate/rgb565_colors
    //colors[0] = matrix.color565(60 * dimmer, 60 * dimmer, 60 * dimmer);   // Dark Gray
    //colors[1] = matrix.color565(100 * dimmer, 59 * dimmer, 3 * dimmer); // Brown
    colors[0] = matrix.color565(208 * dimmer, 1 * dimmer, 208 * dimmer);   // Yellow
   // colors[3] = matrix.color565(235 * dimmer, 120 * dimmer, 0);          // Orange
    colors[1] = matrix.color565(0 * dimmer, 255 * dimmer, 100 * dimmer);    // cyan
    colors[2] = matrix.color565(0, 235 * dimmer, 1 * dimmer);           // Green
    colors[3] = matrix.color565(0, 57 * dimmer, 235 * dimmer);           // Blue
   // colors[7] = matrix.color565(97 * dimmer, 7 * dimmer, 115 * dimmer); // Purple

  /*
    colors[0] = matrix.color565(60 * dimmer, 60 * dimmer, 60 * dimmer);   // Dark Gray
    colors[1] = matrix.color565(100 * dimmer, 59 * dimmer, 3 * dimmer); // Brown
    colors[2] = matrix.color565(208 * dimmer, 1 * dimmer, 1 * dimmer);   // Red
    colors[3] = matrix.color565(235 * dimmer, 120 * dimmer, 0);          // Orange
    colors[4] = matrix.color565(235 * dimmer, 217 * dimmer, 0);          // Yellow
    colors[5] = matrix.color565(0, 108 * dimmer, 18 * dimmer);           // Green
    colors[6] = matrix.color565(0, 57 * dimmer, 235 * dimmer);           // Blue
    colors[7] = matrix.color565(97 * dimmer, 7 * dimmer, 115 * dimmer); // Purple
    */
}


void pixel_dust_setup(void)
{
  //lastDimmer = dimmer;
 //Serial.println("pixel seup" + dimmer);
    // Serial.begin(115200);
    // while (!Serial) delay(10);

    ProtomatterStatus status = matrix.begin();
    // Serial.printf("Protomatter begin() status: %d\n", status);

    if (!sand.begin())
    {
        // Serial.println("Couldn't start sand");
        // err(1000); // Slow blink = malloc error
    }

    if (!accel.begin(0x19))
    {
        // Serial.println("Couldn't find accelerometer");
        // err(250);  // Fast bink = I2C error
    }
    accel.setRange(LIS3DH_RANGE_4_G); // 2, 4, 8 or 16 G!

    //sand.randomize(); // Initialize random sand positions

    // Set up initial sand coordinates, in 8x8 blocks
    /*int n = 0;
    for (int i = 0; i < N_COLORS; i++)
    {
        int xx = i * WIDTH / N_COLORS;
        int yy = HEIGHT - BOX_HEIGHT;
        for (int y = 0; y < BOX_HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH / N_COLORS; x++)
            {
                // Serial.printf("#%d -> (%d, %d)\n", n,  xx + x, yy + y);
                sand.setPosition(n++, xx + x, yy + y);
            }
        }
    }*/
    // Serial.printf("%d total pixels\n", n);

    // matrix.color565(0, 6, 0))
    setColors();
}

// MAIN LOOP - RUNS ONCE PER FRAME OF ANIMATION ----------------------------

void pixel_dust_loop()
{
    if (dimmer == 0 && !isStopped)
    {
    isStopped = true;
    matrix.fillScreen(0x0);
    return;
    } else {
      isStopped = false;
      setColors();
    
    // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
    // calculations are non-deterministic (don't always take the same amount
    // of time, depending on their current states), this helps ensure that
    // things like gravity appear constant in the simulation.
    uint32_t t;
    while (((t = micros()) - prevTime) < (1000000L / MAX_FPS))
        ;
    prevTime = t;

    // Read accelerometer...
    sensors_event_t event;
    accel.getEvent(&event);
    // Serial.printf("(%0.1f, %0.1f, %0.1f)\n", event.acceleration.x, event.acceleration.y, event.acceleration.z);

    double xx, yy, zz;
    xx = event.acceleration.x * 1000;
    yy = event.acceleration.y * 1000;
    zz = event.acceleration.z * 1000;

    // Run one frame of the simulation
    sand.iterate(xx, yy, zz);

    // sand.iterate(-accel.y, accel.x, accel.z);

    // Update pixel data in LED driver
    dimension_t x, y;
    matrix.fillScreen(0x0);
    for (int i = 0; i < N_GRAINS; i++)
    {
        sand.getPosition(i, &x, &y);
        int n = i / ((WIDTH / N_COLORS) * BOX_HEIGHT); // Color index
        uint16_t flakeColor = colors[n];
        matrix.drawPixel(x, y, flakeColor);
        // Serial.printf("(%d, %d)\n", x, y);
    }
    // matrix.show(); // Copy data to matrix buffers
    }
}
