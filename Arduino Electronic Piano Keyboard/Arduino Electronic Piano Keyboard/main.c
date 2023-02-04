/*
 * Arduino Electronic Piano Keyboard.c
 *
 * Created: 30.11.2022. 17:12:40
 * Author : Aleksandar Bogdanovic
 */ 

// Fast PWM Mode

// All defines
#define F_CPU 16000000
#define __DELAY_BACKWARD_COMPATIBLE__
#define DEBOUNCE 250

// All includes
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "pitches.h"

// Variables
int tempo;
double dutyCycle;
double length;
// int keyVal;
volatile int analogResult = 0;


#define c4 (16000000 / 256) / 261.63 - 1
#define cs4 (16000000 / 256) / 277.18 - 1
#define d4 (16000000 / 256) / 293.66 - 1
#define ds4 (16000000 / 256) / 311.13 - 1
#define e4 (16000000 / 256) / 329.63 - 1
#define f4 (16000000 / 256) / 349.23 - 1
#define fs4 (16000000 / 256) / 369.99 - 1
#define g4 (16000000 / 256) / 392.00 - 1
#define gs4 (16000000 / 256) / 415.30 - 1
#define a4 (16000000 / 256) / 440 - 1
#define as4 (16000000 / 256) / 466.16 - 1
#define b4 (16000000 / 256) / 493.88 - 1

#define whole		1
#define half		0.5
#define quarter		0.25
#define eight		0.125
#define sixteenth	0.0625
#define halfdot		0.75
#define quarterdot	0.375

void soundOn(double tone, double len) {
	
	DDRD   |= (1 << PORTD5);
	TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << WGM02) | (1 << CS02);
	OCR0A = tone;
	OCR0B = tone / 2;
	
	length = (60.0/tempo)*len*1000.0;
	_delay_ms(length);
	DDRD &= (0 << PORTD5);
	_delay_ms(10);
}

void pause(double len) {
	DDRD &= (0 << PORTD5);
	length = (60.0/tempo)*len*1000.0;
	_delay_ms(length);
}
//**********************************************************

// ADC setup -----------------------------------------------
/*void setupADC() {
	
	ADMUX = (1 << REFS0)  | (1 << MUX0) | (1 << MUX2);		// ADMUX = 0b01000101
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1<<ADPS1) | (1 << ADPS2);		// ADCSRA = 0b10001111
	DIDR0 = (1 << ADC5D);		// DIDR0 = 0b00100000

}*/

/*void startConversion() {
	
	ADCSRA |= (1 << ADSC);		// ADCSRA |= 0b01000000
}
*/
void init() {
	/*DDRD = (1 << PORTD5);		// PD5 as an output
	PORTD = (1 << PORTD5);		// Pull Up Resistor
	
	// DDRC |= (1 << PORTC2);
	PORTC &= (0 << PORTC2);		// PORTC2 as input
	
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);		// TCCR0A = 0b10000011
	TIMSK0 = (1 << TOIE0);		// TIMSK0 = 0b00000001
	
	setupADC();
	
	sei();
	
	TCCR0B = (1 << CS00) | (1 << CS02);		// 0b00000101*/
	DDRC &= ~(1 << PORTC0);		// input
	PORTC &= (0 << PORTC0);
	//DDRD |= (1 << PORTD5);
	//PORTD |= (1 << PORTD5);		// output
	
	ADMUX &= ~( (1 << REFS1) | (1 << REFS0) | (1 << ADLAR) );
	ADMUX |= (1 << MUX1);
	
	ADCSRB &= ~( (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0) );
	
	DIDR0 |= (1 << ADC2D);
	
	ADCSRA |= (1 << ADEN) | (1 << ADIE);
	
	sei();
	
	ADCSRA |= (1 << ADSC);
	
	// analogResult = (ADCH << 8) | ADCL;
}
//***********************************************************
void buttonSetup() {
	DDRC  &=! (1<<PORTC0); //PC0 - input pin ,switch
	DDRD  |= (1<<PORTD7);//PD5 - output pin, led
}


void buttonPress() {
	
	buttonSetup();
  
    if(!(PINC&(1<<PORTC0)))//if input to Port D pin 1 is low ie.if(PD1==0)
    {
	    PORTD &=! (1 << PORTD7);
	    //PORTD |= (1<<PORTD7);//led on, PD7=0
	    //_delay_ms(DEBOUNCE);
    }
    else
    {
	    PORTD|=(1<<PORTD7);//led off, PD2=1
	    //_delay_ms(DEBOUNCE);
    }
}

// Main	
int main(void)
{	
	init();
    tempo = 60;
    while (1) 
    {
		//for (analogResult = 0; analogResult < 1024; analogResult++) {
		////// KEYBOARD NOTES & FREQUENCY STATEMENTS //////
		// Use the resistor ladder you created to change the voltage of each piano key button
		// 220 ohm = C5 // 560 ohm = D5 // 1K ohm = E5 // etc...
		if(analogResult == 1023)  {
			soundOn(c4, half); // C
			_delay_ms(DEBOUNCE);
		} // end if
		
		else if(analogResult >= 990 && analogResult <=1010)  {
			soundOn(d4, half); // D
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult >= 960 && analogResult <= 980)  {
			soundOn(e4, half);; // E
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult >= 900 && analogResult <= 945)  {
			soundOn(f4, half);; // F
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult >= 650 && analogResult <= 710)  {
			soundOn(g4, half);; // G
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult >= 500 && analogResult <= 550)  {
			soundOn(a4, half);; // A
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult >= 10 && analogResult <= 50)  {
			
			soundOn(b4, half);; // B
			_delay_ms(DEBOUNCE);
		} // end else if
		
		else if(analogResult <= 5)  { // if no switch is pressed, do not play tone
			pause(whole+whole);
		} // end else if
		//}
	} //end frequency tone loop
		
}


ISR(TIMER0_OVF_vect) {
	OCR0A = analogResult;
}

ISR(ADC_vect) {
	analogResult = (ADCH << 8) | ADCL;
}
