/*
 * inter.c
 * This file is part of ultrasonic range finder interfacing
 *
 * Copyright (C) 2014 - Rohit
 *
 * ultrasonic range finder interfacing is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ultrasonic range finder interfacing is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ultrasonic range finder interfacing. If not, see <http://www.gnu.org/licenses/>.
 */


/*

Share code free knowledge


*/
/*
compilation instructions as it involves floating point math this will bloat up the code so to optimise it use -lm while compiling in avr-gcc
so be careful to link math.h by -lm if using gcc

*/
#define F_CPU 12000000UL
#include<avr/io.h>
#include<string.h>
#include<stdlib.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "lcd.c"
volatile int up=1,run=1;
volatile uint64_t ovt=0;
volatile double res=0;
char buffer[50];
volatile flag=0;

void main(){

lcd_init(LCD_DISP_ON);
DDRA=0xff; //trig
DDRB=0x00; //echo
//External interrupts are set to be called on raising edge
MCUCSR|=(1<<ISC2);
GICR|=(1<<INT2);
GIFR|=(1<<INTF2);
sei();
while(1){
{
if(run){
//Here if code is not running or waiting for echo 
//set the trig high for the given time 15us
DDRA=0xff;
PORTA&=~(1<<0);
_delay_us(1);
PORTA|=(1<<0);
_delay_us(20);
PORTA&=~(1<<0);
DDRA=0x00;
run=0;
}
}
/*
Check for max time then reset

*/
if(((ovt*65536)+TCNT1)*(0.083333)>29410 ){
up=1;
cli();
//End timer process
TCCR1B&=~(1<<CS10);
//pre=TCNT1;
TIMSK&=~(1<<TOIE1);

flag=1;
GICR&=~(1<<INT2);
MCUCSR|=(1<<ISC2);
GIFR|=(1<<INTF2);
GICR|=(1<<INT2);
TCCR1B|=(1<<CS10);
TCNT1=0;
ovt=0;
TIMSK|=(1<<TOIE1);
//SREG=sreg;
sei();
run=1;
}
if(flag)
/*
if data is changed then update lcd

*/
{lcd_clrscr();
lcd_puts(buffer);
flag=0;
_delay_ms(60);
}
}
}
ISR(INT2_vect){
/*
Echo connected here 


*/
uint8_t sreg;
float pre;
if(up==1){
/*raising edge
 if the signal is raising

*/
up=0;
//Start timer here
sreg=SREG;


cli();   //stop interrupt
/*

Reset the external interrupt to call interrupt on falling edge

*/
TCNT1=0;
GICR&=~(1<<INT2);
MCUCSR&=~(1<<ISC2);
GICR|=(1<<INT2);
TCCR1B|=(1<<CS10);

TIMSK|=(1<<TOIE1);
SREG=sreg;
sei();
//enable global interrupt
}
else{

//This is falling edge
pre=TCNT1;
//pre is current TCNT1 value
up=1;

cli();

//End timer process
TCCR1B&=~(1<<CS10);

TIMSK&=~(1<<TOIE1);
pre=(((ovt*65536)+pre)*14.1667*.0001)+1;
/*


pre is the formula i derived to find the time delay or pulse width
Formula derived

one timer register is updated at one clock cycle
here clock is 12MHZ
therefore 1 register takes 1/(12*10^-6) seconds to update

here ovt is overflow variable that is called each time 16 bit timer is overflowed
hence time delay=ovt*65536+pre(see previous comment)

therefore total time delay in us is 
t=(ovt*65536+pre)*(1/12)
t=(ovt*65536+pre)*0.08333 microseconds  -------------------------------------------(1)
this will give me the register count of timer(This is an ingenious hack to keep track of time ,as 16 bit timer overflows in 65536/12 microseconds i.e approx 5 ms)

here best thing is t can vary from min of 20 us to many minutes

this algo can be used to find time difference between 2 events

assuming the speed of sound in air is average to be 340m/s
then 
the ultrasonic wave took 't'seconds to go to and fro then
it is actually 't'seconds away
hence to find distance
distance=(t*340/2)
distance=(t*170) meter 
in cm 
distance is 
dis=t*170*100; -----------------------------------------------------------------------------------------(2)

taking (1) and (2)


distance is

dis=(t*170*100)
but t is (ovt*65536+pre)*0.08333*10^-6 seconds

therefore 
dis=((ovt*65536+pre)*0.08333*170*100*10^-6)
dis =((ovt*65536+pre)*14.1667*100*10^-6)
dis =((ovt*65536+pre)*14.1667*0.0001)

now you can see i have added a +1 to dis and might be wondering why
its because there is some unknown bug
which is causing the answer to be short of 1 cm although the differntial accuracy is quite good(1 mm range)
if you can fix it it would be great
*/


dtostrf( pre, 3, 2, buffer);// copys pre value as string declared in stdlib.h

flag=1;
GICR&=~(1<<INT2);
MCUCSR|=(1<<ISC2);

GICR|=(1<<INT2);
TCCR1B|=(1<<CS10);
TCNT1=0;
ovt=0;
TIMSK|=(1<<TOIE1);
//same stuff here external interrrupt is set to be called on raising edge
sei();
run=1;
}
}

ISR(TIMER1_OVF_vect){
//This is called at overflow
ovt++;
}



