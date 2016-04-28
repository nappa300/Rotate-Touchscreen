Rotate is a script written in C that handles rotating the screen of a touchscreen on Ubuntu based systems running X11. Target use is for 2 in 1 ultrabook touchscreen laptops that when folded require having their screens rotated.

In particular this script handles rotating the image displayed on the screen as well as rotating the touchscreen input.
If you were to only rotate the display, for example with `xrandr -o left`, you will find that the touchscreen input is still reacting as if the screen had not been rotated.

##Usage 
`binary <{normal, inverted, left, right}>`

###Example
Compile <br/>
 `gcc -o rotate rotate.c`
Use <br/>
./rotate inverted


How to make this a terminal command:
http://askubuntu.com/a/118320/357970