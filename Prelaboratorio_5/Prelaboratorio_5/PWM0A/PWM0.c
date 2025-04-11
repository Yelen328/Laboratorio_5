/*
 * PWM0.c
 *
 * Created: 11/4/2025 03:43:59
 *  Author: yelen
 */ 
#include <avr/io.h>
#include "PWM0.h"

void initPWM1A(uint8_t invertido, uint16_t prescaler){
	
	DDRD |= (1 << DDD6);	//Setear bit 6 del puerto D como salida
	TCCR1A &=~((1 << COM1A1)|(1 << COM1A0));
	
	if (invertido == invert){
		TCCR1A |= (1 << COM1A1)| (1 << COM1A0);	//invertido
	}
	else{
		TCCR1A |= (1 << COM1A1);	//no invertido
	}
	
	TCCR1A |= (1 << WGM11) | (1 << WGM10);	//Modo fast PMW y top->0XFF
	
	TCCR1B &=~((1 << CS12)|(1 << CS11)|(1 << CS10));
	switch (prescaler){
		case 1:
		TCCR1B |= (1<<CS10);
		break;
		
		case 8:
		TCCR1B |= (1<<CS11);
		break;
		
		case 64:
		TCCR1B |= ((1<<CS11) | (1<<CS10));
		break;
		
		case 265:
		TCCR1B |= (1<<CS12);
		break;
		
		case 1024:
		TCCR1B |= ((1<<CS12) | (1<<CS10));
		break;
		
		default:
		TCCR0B |= (1<<CS10);
		break;
	}
	TCCR1B |= (1 << CS11); //Configurado con un prescalres de 256
	
}

void  updateDutyCycle(uint8_t duty){
	OCR1A = duty;
}