ultrasonic-12MHZ_ATmega16
=========================
This is an code written in avr-gcc to interface ultrasonic range finder HC-SR04 to atmega16 running at 16MHz
the unique feature of code is the algorithm to measure time distance between two event whic can be measure accurately from minimum of 20 microsecs to several minutes

This will measure a distance of obstacle from 3 cm to 4 metres with the accuracy of 1 mm

Trigger pin of hcsr04 to any pin in port a 
echo pin of hcsr04 to PB2 of atmega

and thanks to 
Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
for his opensource LCD library

queries 
 rohitfoss00@gmail.com
