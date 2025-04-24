/*UNIVERSIDAD DEL VALLE DE GUATEMALA
 * Postlaboratorio_5.c
 *
 * Created: 22/4/2025 10:18:20
 * Author : yelena Cotzojay*/



#define	F_CPU	16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM1A/PWM1.h"
#include "PWM1B/PWM1B.h"
#include "PWM_MAN/PWM_MAN.h"
uint8_t dutyCycle1= 128;	//Valor inicial en el centro (1.5 ms)
uint8_t dutyCycle2= 128;	//Valor inicial en el centro (1.5 ms)
uint8_t POT=0;	//Variable a utilizar para el multiplexeo de la lectura del ADC
uint8_t ADC1=0;
uint8_t ADC2=0;
uint8_t ADC3=0;


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
	}
}

void setup(){
	cli();	//Desabilitar interrupciones
	//INIT_TMR1();	//Llamar a la configuración del timer
	INIT_ADC();
	initPWM1A();
	initPWM1B();
	//init del timer 0 para el 3er pwm
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
	ADCSRA = 0;	//Apagar todo
	ADCSRA |=(1<<ADPS1)	| (1 << ADPS0); //Configuración del presacaler 8
	ADCSRA |= (1 << ADIE);	//Habilitación de interrupciones
	ADCSRA |= (1 << ADEN);	//Habilitar ADS
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
			
			ADMUX &= ~(1<<MUX1);  // Limpiar bit MUX1 primero
			ADMUX |= (1<<MUX2)|(1<<MUX0); //Selección de canal Bit 5 del puerto C
			ADC3=ADCH;
			DutyCycleLED(ADC3); // Actualizar PWM
			break;
			
		case 4:
			POT=0;
			break;
		
		default:
			break;
	}
	 
	ADCSRA |= (1 << ADSC);	//Iniciar nueva conversión
}

