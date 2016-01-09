
#include <Adafruit_NeoPixel.h>

#define PIN             6
#define NUMPIXELS       1
#define OSCIL_VAL       450

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//  Determines how smooth the fade in/fade out behaviour is
int delayval = 25;

//  A stable, consistent incremental value is required. 
//      millis() is too unstable, and therefore difficult to control
//      fade in/out length
long commIncVal = 0;

void setup() {
    Serial.begin(9600); 
    pixels.begin(); 
    setNeoPixelColor(pixels.Color(240, 255, 0));
}

void loop() {
	neoPulse();
}

void neoPulse() {
    //  The following initializes the oscillating value, which will be
    //      anywhere from 0 to OSCIL_VAL/2
    int i = oscilPulse(commIncVal++); 
    
    //  This loop is where the intensities of green vary. The shorter the delay
    //      the smoother the transitions. Changing the incrementation value
    //      will also change smoothness.
    //
    //  The loop only evaluates given that the interval is within OSCIL_VAL and 5.
    while(i != OSCIL_VAL && i >= 5) {
		//	In order to change pulse speed, vary number that increments commIncVal
        i = oscilPulse(commIncVal+=3); 

        //  Low to moderately bright green. 
        pixels.setBrightness(i);

        //  Whenever changes are made to the pixel color, the show method must
        //      follow in order to apply color changes.  
        pixels.show();                      

        //  The lower the delay (ms), the smoother the appearance. 
        delay(delayval);            
    }

    //  This will hold the current LED brightness/intensity at the end of every loop,
    //      signifiying the end of a light pattern as described above. 
	//  delay(10);
}

//  This function sets the color and displays the new color on the Neopixels.
//      The Color "object" as seen when calling "pixels.Color(....)" is not an object
//      but a method returning a packed 32-bit RGB color given a R, G, B values.
//      It's a bit confusing since a structure would have been more appropriate.
//  Ex. setNeoPixelColor(pixels.Color(0, 0, 255));
void setNeoPixelColor(uint32_t neoColor) {
    pixels.setPixelColor(0, pixels.Color(240, 255, 0));
    pixels.show();
}

//  This is the mathematical function that allows pulsing concurrently with any
//      existing processes that are being executed. 
uint8_t oscilPulse(long incVal) {
    //  Oscillation is divided into three parts.

    //  This is the incremental range and increasing direction from 0 to OSCIL_VAL. 
    int incFun = incVal % OSCIL_VAL;

    //  This evaluates to be either 0 or 1 given incFun is 0 when less than
    //      OSCIL_VAL/2 and 1 when higher than OSCIL_VAL/2.
    //  This allows selective subtraction when whenToSub = 0 or 1.
    int whenToSub = incFun / (OSCIL_VAL / 2);

    //  This is the range to subtract given incFun, which is (0 - (OSCIL_VAL/2)) to
    //      (OSCIL_VAL - (OSCIL_VAL/2)) -> -(OSCIL_VAL/2) to OSCIL_VAL/3.
    //  Ultimately, this is what allows an incremental decrease upon reaching
    //      OSCIL_VAL.
    int decFun = ((incVal % OSCIL_VAL) - (OSCIL_VAL / 2));

    //  The final formula is as follows, where upon subtraction is allowed, 
    //      the range is doubled to offset the range [OSCIL_VAL/2, OSCIL_VAL]
    //      to allow a decreasing behaviour.
    int result = incFun - (whenToSub * decFun * 2);

    //  The final result.
    return (uint8_t) result;
}
