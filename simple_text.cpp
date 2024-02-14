//TODO
/*
center text
use font
draw box?

*/
#include <Adafruit_Protomatter.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono9pt7b.h>

extern uint8_t rgbPins[];
extern uint8_t addrPins[];
extern uint8_t clockPin;
extern uint8_t latchPin;
extern uint8_t oePin;

Adafruit_Protomatter matrix(
    64,                        // Width of matrix (or matrix chain) in pixels
    4,                         // Bit depth, 1-6
    1, rgbPins,                // # of matrix chains, array of 6 RGB pins for each
    4, addrPins,               // # of address pins (height is inferred), array of pins
    clockPin, latchPin, oePin, // Other matrix control pins
    false);                    // No double-buffering here (see "doublebuffer" example)

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void write_text_to_matrix(String message)
{
    Serial.begin(9600);

    // Initialize matrix...
    ProtomatterStatus status = matrix.begin();
    Serial.print("Protomatter begin() status: " + message);
    Serial.println((int)status);
    if (status != PROTOMATTER_OK)
    {
        // DO NOT CONTINUE if matrix setup encountered an error.
        for (;;)
            ;
    }

    matrix.fillScreen(0x00);
   // matrix.setFont("IBMPlexMono-Medium-24_jep.bdf");
   // matrix.show();

    // Make four color bars (red, green, blue, white) with brightness ramp:
    for (int x = 0; x < matrix.width(); x++)
    {
        uint8_t level = x * 256 / matrix.width(); // 0-255 brightness
        matrix.drawPixel(x, matrix.height() - 4, matrix.color565(level, 0, 0));
        matrix.drawPixel(x, matrix.height() - 3, matrix.color565(0, level, 0));
        matrix.drawPixel(x, matrix.height() - 2, matrix.color565(0, 0, level));
        matrix.drawPixel(x, matrix.height() - 1,
        matrix.color565(level, level, level));
    }
    // You'll notice the ramp looks smoother as bit depth increases
    // (second argument to the matrix constructor call above setup()).

    // Simple shapes and text, showing GFX library calls:
   // matrix.drawCircle(12, 10, 9, matrix.color565(255, 0, 0));
   // matrix.drawRect(14, 6, 17, 17, matrix.color565(0, 255, 0));
    //matrix.drawTriangle(32, 9, 41, 27, 23, 27, matrix.color565(0, 0, 255));
    matrix.println(message); // Default text color is white
    if (matrix.height() > 32)
    {
        matrix.setCursor(0, 32);
        matrix.println("64 pixel"); // Default text color is white
        matrix.println("matrix");   // Default text color is white
    }

    // AFTER DRAWING, A show() CALL IS REQUIRED TO UPDATE THE MATRIX!

    matrix.show(); // Copy data to matrix buffers
}

void simple_text_loop(void)
{
    // Since there's nothing more to be drawn, this loop() function just
    // shows the approximate refresh rate of the matrix at current settings.
   // Serial.print("Refresh FPS = ~");
   // Serial.println(matrix.getFrameCount());
}

