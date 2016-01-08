# LED_Pulsing

-- Description\n
Most LED fading functions rely on changing RGB values, but during the fade, the actual color varies since you're directly changing the RGB values. Furthermore, using loops is quite expensive, especially if you want to execute an LED fade "simultaneously" with some other process.

This code uses some Math Magic to fade in and out using a single loop instead of two and avoids changing the RGB values to change light intensity/brightness. Refer to the setBrightness() method to learn more about how brightness is implemented. 
https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp
