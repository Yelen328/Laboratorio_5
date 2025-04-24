/*
 * PWM_MAN.c
 *
 * Created: 23/4/2025 22:55:16
 *  Author: yelena Cotzojay
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM_MAN.h"

uint8_t CONT = 0;
uint8_t limite = 0;

void INIT_TMR0(){
	DDRD |= (1 << PIND6);	// Salida PD6

	//Configurar el timer 0 en modo normal
	TCCR0A = 0x00;
	TCCR0B = (1 << CS01) | (1 << CS00);   //Prescaler 64
	TCNT0 = 255;	
	TIMSK0 = (1 << TOIE0);	//Habilitar interrupciones del timer 0
}

void  DutyCycleLED(uint8_t duty){
	limite = duty;
}
		
	
ISR(TIMER0_OVF_vect){
	if (CONT >= limite)
	{
		PORTD &= ~(1 << PORTD6);  

	}
	
	else{
		PORTD|= (1<<PORTD6);
	}
	
	CONT ++;
	TCNT0 = 255;
}
