/*
 ===============================================================================
 Name        : callback.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2014/1/7
 License	 : MIT
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/7	v1.0.1	Enable pin 8~13									Jason
  ===============================================================================
 */


#ifndef CALLBACK_H_
#define CALLBACK_H_

extern int samplingInterval;

void callback_init(void);
void systemResetCallback(void);
void checkAnalogInputs(void);
void checkDigitalInputs(void);

#endif /* CALLBACK_H_ */
