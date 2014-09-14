ultrasonic-12MHZ_ATmega16
=========================
This is a code written in avr-c to interface ultrasonic range finder HC-SR04 to atmega16 running at 12MHz
the unique feature of code is the algorithm to measure time distance between two event which can be measured accurately from minimum of 20 microsecs to several minutes[asyncronously that is you needn halt your program loop]

This will measure a distance of obstacle from 3 cm to 4 metres with the accuracy of 1 mm

Trigger pin of hcsr04 to any pin in port a 
echo pin of hcsr04 to PB2 of atmega

and thanks to 
Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
for his opensource LCD library

queries 
 rohitfoss00@gmail.com
