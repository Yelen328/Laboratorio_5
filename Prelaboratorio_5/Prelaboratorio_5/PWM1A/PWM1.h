/*
 * PWM0.h
 *
 * Created: 11/4/2025 03:43:13
 *  Author: yelen
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#define  invert  1
#define non_invert  0
void initPWM1A();
void updateDutyCycle(uint8_t duty);		//Asignarle valor a 0CR1A


#endif /* PWM1_H_ */