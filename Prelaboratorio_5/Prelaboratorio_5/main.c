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
#include "PWM0A/PWM0.h"
uint8_t dutyCycle= 101;


//Función Prototypes
void setup();
void INIT_TMR1();
void INIT_ADC();

//void initPWM0A(uint8_t invertido, uint16_t prescaler);
//void updateDutyCycle(uint8_t duty);		//Asignarle valor a 0CR0A


void setup(){
	cli();	//Desabilitar interrupciones
	INIT_TMR1();	//Llamar a la configuración del timer
	INIT_ADC();
	initPWM1A(non_invert, 8);
	CLKPR = (1 << CLKPCE); //Habilita cambios de prescaler
	CLKPR = (1 << CLKPS2);	// 1MHz
	UCSR0B=0;	//Apagar los bit 0 y 1 del puerto D
	sei();		//Habilita cambios de interrupción
	//initPWM0A(invert, 8);
}

int main(void)
{
    setup();
	
    while (1) 
    {
		updateDutyCycle(dutyCycle);
		dutyCycle++;
		_delay_ms(20);
    }
}

void INIT_TMR1(){
	TCCR1A =0;	//Configurarlo en modo normal
	TCCR1B |= (1<< CS01)  ;	//configurar un prescaler de 8
	TCNT1 =50;	//Desborde cada 1 ms
	TIMSK1 = (1 << TOIE0);	//Habilitar interrupciones
}

void INIT_ADC(){
	ADMUX = 0;	//Apagar todo
	
	//Voltaje de referencia 5V
	ADMUX |=(1<<REFS0);
	ADMUX &=~(1<<REFS1);
	
	//ADMUX |= (1<<ADLAR); //orientación (izquierda)
	ADMUX |= (1<<MUX2) | (1<<MUX1);//Selección de canal Bit 6 del puerto C
	
	ADCSRA = 0;	//Apagar todo
	ADCSRA |=(1<<ADPS1)	| (1 << ADPS0); //Configuración del presacaler 8
	ADCSRA |= (1 << ADIE);	//Habilitación de interrupciones
	ADCSRA |= (1 << ADEN);	//Habilitar ADS
}

/*************VECTOR DE INTERRUPCIÓN************/

ISR(ADC_vect)
{
	uint8_t temporal = ADCH;
	// Mapear ADC (0-255) a dutyCycle (5% - 10%)
	dutyCycle = ((temporal * (10 - 5)) / 255) + 5;
	updateDutyCycle(dutyCycle); // Actualizar PWM

	ADCSRA |= (1 << ADSC);
}
