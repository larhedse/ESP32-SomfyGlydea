# ESP32-SomfyGlydea
## An ESP32 implementation to control Somfy Glydea curtain motors over WiFi

This is a very small and simple implementation of ESP32-C6 in combo with Somfy Glydea 35 motors. This project needs to modify the Glydea motor control electronics to support enough current (electricty) for the ESP32-C6 development card I choosed to build this upon.

So one needs:
Somfy Glydea RTS 35 motor(s) for curtain control (Open, Close and My Position / Stop).
Some sort of development board like the [SparkFun Qwiix ESP32-C6](https://www.sparkfun.com/products/22925)

I choosed the ESP32-C6 since it does support ZigBee (Matter), however this implementation does currently NOT support ZigBee. And when / if it ever will, then I fear that one has to choose either WiFi or ZigBee - so both active at the same time (think ZigBee with OTA WiFi update - one might be able to do firmware update over ZigBee, but I simply do no know how (yet)).

I also choosed it for it share small size - it is so small that it can easily hang from the RJ12 (6-contact which comes from well I think telecom usage?) connection, which will be used anyway.

One will also be able to remove the RTS control board, so that it does not interfeer with the WiFi signal (do note that the RTS or RF433 signal is not on the same channel as WiFi so this might be minor). When removing the RTS control board from the motor one will lose the ability to control the motor from the Somfy remote control (that only works on RF 433MHz), which also takes away the ability to set up the end position and stuff like that (at least until I coded the same functions into the WiFi ESP32-C6 software, which can take ages before I do).

Let's start with the ESP32-C6 board, and how to solder the "telephone wires" (all 6 of them, color coded). For this one needs a fully 6-line cable with most likly preterminated RJ12 on both ends. Cut one of the ends about 10cm or so from the contact, and prepp the end for soldering into the ESP32-C6 board. The  solder the cables like this picture:
![Somfy3](https://github.com/larhedse/ESP32-SomfyGlydea/assets/8342357/a0b09a15-5d07-485f-a272-a5f6c92346fa)

When this is done, one needs to do two minor adjustments on one of the control boards that runs the motor. So pic the Somfy Glydea motor apart and remove the very small resistor (R504) one can see on this picture:
![Somfy1](https://github.com/larhedse/ESP32-SomfyGlydea/assets/8342357/7be3517a-dd2d-42b0-b3a8-c42d6ec8654d)

And the solder a small jumper wire between two pins on the reverse side to give some more current to the ESP-C6:
![Somfy2](https://github.com/larhedse/ESP32-SomfyGlydea/assets/8342357/a8e3dc0b-1ccd-4e85-a8b0-c218eaa539f3)

All this can be done with just pushing the small board out of the housing for the electric motor. This small board sits on the end with the RJ12 connector - but you will need to open both ends - and be aware of the spring that is loaded on the engine side. It likes to pop out...

When the above work is done, then just open up Arduino and download the code I written. Feel free to do any sort of modification, there is no right or wrong here - just keep exploring!
