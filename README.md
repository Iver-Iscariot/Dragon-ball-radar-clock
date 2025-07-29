# Dragon ball radar watch
A clock based on the dragon-radar from Dragon Ball.<br>
Very basic, pocket watch-style clock, with two rings of 12 LED's (hour/minute).<br>
At the press of the top button, the hour and minute (rounded to closest five) will pulse a couple of times, like in the anime.
&nbsp;

As this will be a novelty item that I only use once in a while, I will forgo a proper RTC, and just rely on the MCU itself for timekeeping. 
To work with the one-button interface of the original, I will set it such that a long-press will set the time to 12 o'clock, so you can simply set it when the actual time is 12 am or pm.<br>


&nbsp;

<img src="https://github.com/user-attachments/assets/d41f6462-61da-4b58-95d4-ea9c64b999cb" width="200px" height="240px"/>

Progress so far. I need to turn a housing on the lathe



## VERY organized scematic:

![image](https://github.com/user-attachments/assets/83958388-5268-49c8-95af-034e015e63ec)


## TODO for next PCB itteration:
 - Mosfet poweroff for 4052 multiplexer (power-saving)
 - Perhaps figure out more efficient way of driving the matrix than 4052 (IS31FL3746A LED driver ?)
 - Consider using dedicated RTC, to allow MCU sleep (power saving)
 - Attempt to move LED rings closer to center and make it smaller (LED driver chip would reduce total component footprint)
 - Try out a different LIPO charging circuit (there HAS to be a better way)
