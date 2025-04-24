/*UNIVERSIDAD DEL VALLE DE GUATEMALA
 * Laboratorio_5.c
 *
 * Created: 22/4/2025 12:00:51
 * Author : yelen
 */ 



#define	F_CPU	16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM1/PWM1.h"
#include "PWM2/PWM1B.h"
uint8_t dutyCycle1= 128;	//Valor inicial en el centro (1.5 ms)
uint8_t dutyCycle2= 128;	//Valor inicial en el centro (1.5 ms)
uint8_t POT=0;	//Variable a utilizar para el multiplexeo de la lectura del ADC
uint8_t ADC1=0;
uint8_t ADC2=0;


//Función Prototypes
void setup();
void INIT_TMR1();
void INIT_TMR0();
void INIT_ADC();

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

void setup(){
	cli();	//Desabilitar interrupciones
	//INIT_TMR1();	//Llamar a la configuración del timer
	INIT_ADC();
	initPWM1A();
	initPWM1B();
	INIT_TMR0();
	DDRB|=(1 << PORTB0);
	CLKPR = (1 << CLKPCE); //Habilita cambios de prescaler
	CLKPR = (1 << CLKPS2);	// 1MHz
	UCSR0B=0;	//Apagar los bit 0 y 1 del puerto D
	sei();		//Habilita cambios de interrupción
	
	ADCSRA |= (1 << ADSC); // Iniciar primera conversión

}


void INIT_ADC(){
	ADMUX = 0;	//Apagar todo
	
	//Voltaje de referencia 5V
	ADMUX |=(1<<REFS0);
	
	ADMUX |= (1<<ADLAR); //orientación (izquierda)
	//ADMUX |= (1<<MUX2) | (1<<MUX1);//Selección de canal Bit 6 del puerto C
	
	ADCSRA = 0;	//Apagar todo
	ADCSRA |=(1<<ADPS1)	| (1 << ADPS0); //Configuración del presacaler 8
	ADCSRA |= (1 << ADIE);	//Habilitación de interrupciones
	ADCSRA |= (1 << ADEN);	//Habilitar ADS
}

void INIT_TMR0(){
	//Configurarlo en modo normal 
	TCCR0A = 0x00; 
	TCCR0B = (1 << CS01) | (1 << CS00);   //Prescaler 64
	TCNT0 = 0xB1;	//Desborde cada 0.5ms
	TIMSK0 = (1 << TOIE0);
}

/*************VECTOR DE INTERRUPCIÓN************/

ISR(ADC_vect)
{
	POT++;
	
	switch (POT){
		case 1:
			ADMUX &= ~(1<<MUX0);  // Limpiar bit MUX0 primero
			ADMUX |= (1<<MUX2) | (1<<MUX1);//Selección de canal Bit 6 del puerto C
			ADC1=ADCH;
			dutyCycle1 = (ADC1 * (188.0 / 255.0)) + 69.0;
			updateDutyCycle1(dutyCycle1); // Actualizar PWM
			break;
		
		case 2:
			ADMUX |= (1<<MUX2)|(1<<MUX1)|(1<<MUX0); //Selección de canal Bit 7 del puerto C
			ADC2=ADCH;
			dutyCycle2 = (ADC2 * (188.0 / 255.0)) + 69.0;
			updateDutyCycle1B(dutyCycle2); // Actualizar PWM
			break;
		
		case 3:
			POT=0;
			break;
		
		default:
			break;
	}
	 
	ADCSRA |= (1 << ADSC);	//Iniciar nueva conversión
}



