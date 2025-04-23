/*UNIVERSIDAD DEL VALLE DE GUATEMALA
 * Prelaboratorio_5.c
 *
 * Created: 10/4/2025 22:32:40
 * Author : yelena Cotzojay
 */ 
#define	F_CPU	16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM1A/PWM1.h"
uint8_t dutyCycle= 128;	//Valor inicial en el centro (1.5 ms)


//Función Prototypes
void setup();
void INIT_TMR1();
void INIT_ADC();


void setup(){
	cli();	//Desabilitar interrupciones
	//INIT_TMR1();	//Llamar a la configuración del timer
	INIT_ADC();
	initPWM1A();
	DDRB|=(1 << PORTB0);
	CLKPR = (1 << CLKPCE); //Habilita cambios de prescaler
	CLKPR = (1 << CLKPS2);	// 1MHz
	UCSR0B=0;	//Apagar los bit 0 y 1 del puerto D
	sei();		//Habilita cambios de interrupción
	
	ADCSRA |= (1 << ADSC); // Iniciar primera conversión

}

int main(void)
{
    setup();
	
    while (1) 
    {
		//updateDutyCycle(dutyCycle);
		//dutyCycle++;
		//_delay_ms(20);
		//PORTB  &= ~(1 << PORTB0);
		//updateDutyCycle(155); // Actualizar PWM
		
    }
}


void INIT_ADC(){
	ADMUX = 0;	//Apagar todo
	
	//Voltaje de referencia 5V
	ADMUX |=(1<<REFS0);
	
	ADMUX |= (1<<ADLAR); //orientación (izquierda)
	ADMUX |= (1<<MUX2) | (1<<MUX1);//Selección de canal Bit 6 del puerto C
	
	ADCSRA = 0;	//Apagar todo
	ADCSRA |=(1<<ADPS1)	| (1 << ADPS0); //Configuración del presacaler 8
	ADCSRA |= (1 << ADIE);	//Habilitación de interrupciones
	ADCSRA |= (1 << ADEN);	//Habilitar ADS
}

/*************VECTOR DE INTERRUPCIÓN************/

ISR(ADC_vect)
{
	//PORTB  |= (1 << PORTB0);
	uint8_t temporal = ADCH;
	// Mapear ADC (0-1023) a dutyCycle
	//PORTB= temporal; 
	 dutyCycle = (temporal * (225.0 / 255.0)) + 60.0;
	 updateDutyCycle(dutyCycle); // Actualizar PWM

	ADCSRA |= (1 << ADSC);	//Iniciar nueva conversión
}
