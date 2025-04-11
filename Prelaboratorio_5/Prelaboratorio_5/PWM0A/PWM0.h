/*
 * PWM0.h
 *
 * Created: 11/4/2025 03:43:13
 *  Author: yelen
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#define  invert  1
#define non_invert  0
void initPWM1A(uint8_t invertido, uint16_t prescaler);
void updateDutyCycle(uint8_t duty);		//Asignarle valor a 0CR0A


#endif /* PWM0_H_ */