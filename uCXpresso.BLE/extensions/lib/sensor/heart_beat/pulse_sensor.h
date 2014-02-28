/*
 ===============================================================================
 Name        : pulse_sensor.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2014/2/17
 Description : a Heart Rate pulse sensor class
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/2/17	v1.0.0	Initialize										Jason
 ===============================================================================
 */

#ifndef PULSE_SENSOR_H_
#define PULSE_SENSOR_H_

#include "class/thread.h"
#include "class/adc.h"
#include "class/pin.h"

class CPulseSensor: public CThread {
public:
	CPulseSensor(ADC_CH_T pulsePin);
	virtual ~CPulseSensor();

	virtual bool start();

public:
	int BPM;                // used to hold the pulse rate
	int Signal;        		// holds the incoming raw data
	int IBI;          		// holds the time between beats, must be seeded!
	bool Pulse;     		// true when pulse wave is high, false when it's low
	bool QS;        		// becomes true when Arduoino finds a beat.

protected:
	int rate[10];			// array to hold last ten IBI values
	uint32_t sampleCounter; // used to determine pulse timing
	uint32_t lastBeatTime;  // used to find IBI
	int P;					// used to find peak in pulse wave, seeded
	int T;					// used to find trough in pulse wave, seeded
	int thresh;				// used to find instant moment of heart beat, seeded
	int amp;				// used to hold amplitude of pulse waveform, seeded
	bool firstBeat;	// used to seed rate array so we startup with reasonable BPM
	bool secondBeat;// used to seed rate array so we startup with reasonable BPM
	ADC_CH_T m_pulsePin;

	//
	// Override the CThread::run()
	//
	virtual void run();

	//
	// Event for LED
	//
	virtual void onBlink(PIN_LEVEL_T val) {}

};

#endif /* PULSE_SENSOR_H_ */
