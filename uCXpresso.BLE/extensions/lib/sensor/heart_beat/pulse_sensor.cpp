/*
>> Pulse Sensor Amped 1.2 <<
This code is for Pulse Sensor Amped by Joel Murphy and Yury Gitman
    www.pulsesensor.com
    >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
The following variables are automatically updated:
Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
IBI  :      int that holds the time interval between beats. 2mS resolution.
BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.

This code is designed with output serial data to Processing sketch "PulseSensorAmped_Processing-xx"
The Processing sketch is a simple data visualizer.
All the work to find the heartbeat and determine the heartrate happens in the code below.
Pin 13 LED will blink with heartbeat.
If you want to use pin 13 for something else, adjust the interrupt handler
It will also fade an LED on pin fadePin with every beat. Put an LED and series resistor from fadePin to GND.
Check here for detailed code walkthrough:
http://pulsesensor.myshopify.com/pages/pulse-sensor-amped-arduino-v1dot1

Code Version 1.2 by Joel Murphy & Yury Gitman  Spring 2013
This update fixes the firstBeat and secondBeat flag usage so that realistic BPM is reported.

*/

/* 2014/2/17
 * Modify by Embeda
 * Support uCXpresso Framework
 *
 */

#include "pulse_sensor.h"
#include "class/timer.h"
#include "class/adc.h"

CPulseSensor::CPulseSensor(ADC_CH_T pulsePin) {
	sampleCounter = 0;
	lastBeatTime = 0;
	P = 512;
	T = 512;
	thresh = 512;
	amp = 100;
	firstBeat = true;
	secondBeat = false;
	m_pulsePin = pulsePin;

	IBI = 600;
	Pulse = false;
	QS = false;
}

CPulseSensor::~CPulseSensor() {
}

bool CPulseSensor::start() {
	return CThread::start("pulse", 80, PRI_HARDWARE);
}

void CPulseSensor::run() {
	TIMER_T t = CTimer::getUnused();
	if (t >= 0) {
		//
		// setup pulse (adc) pin
		//
		CAdc adPulse(m_pulsePin);
		adPulse.enable();

		//
		// setup timer to makes sure that we take a reading every 2 miliseconds
		//
		CTimer timer(t);
		timer.second(0.002);
		timer.enable();

		//
		// timer interrupt run-loop
		//
		int	N;
		while (isAlive()) {
			//
			// wait for Timer Interrupt
			//
			timer.wait();

			//
			// Read the pulse signal
			//
			Signal = adPulse.read();

			//
			// Analysis pulse signal
			//
			sampleCounter += 2; // keep track of the time in mS with this variable
			N = sampleCounter - lastBeatTime; // monitor the time since the last beat to avoid noise

			//  find the peak and trough of the pulse wave
			if (Signal < thresh && N > (IBI / 5) * 3) { // avoid dichrotic noise by waiting 3/5 of last IBI
				if (Signal < T) {                        // T is the trough
					T = Signal;     // keep track of lowest point in pulse wave
				}
			}

			if (Signal > thresh && Signal > P) { // thresh condition helps avoid noise
				P = Signal;                             // P is the peak
			}                       // keep track of highest point in pulse wave

			//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
			// signal surges up in value every time there is a pulse
			if (N > 250) {                         // avoid high frequency noise
				if ((Signal > thresh) && (Pulse == false)
						&& (N > (IBI / 5) * 3)) {
					Pulse = true; // set the Pulse flag when we think there is a pulse

					//digitalWrite(blinkPin, HIGH);          // turn on pin 13 LED
					onBlink(HIGH);

					IBI = sampleCounter - lastBeatTime; // measure time between beats in mS
					lastBeatTime = sampleCounter; // keep track of time for next pulse

					if (secondBeat) { // if this is the second beat, if secondBeat == TRUE
						secondBeat = false;             // clear secondBeat flag
						for (int i = 0; i <= 9; i++) { // seed the running total to get a realisitic BPM at startup
							rate[i] = IBI;
						}
					}

					if (firstBeat) { // if it's the first time we found a beat, if firstBeat == TRUE
						firstBeat = false;               // clear firstBeat flag
						secondBeat = true;           // set the second beat flag
						continue;         // IBI value is unreliable so discard it
					}

					// keep a running total of the last 10 IBI values
					word runningTotal = 0; // clear the runningTotal variable

					for (int i = 0; i <= 8; i++) { // shift data in the rate array
						rate[i] = rate[i + 1]; // and drop the oldest IBI value
						runningTotal += rate[i]; // add up the 9 oldest IBI values
					}

					rate[9] = IBI;       // add the latest IBI to the rate array
					runningTotal += rate[9]; // add the latest IBI to runningTotal
					runningTotal /= 10;       // average the last 10 IBI values
					BPM = 60000 / runningTotal; // how many beats can fit into a minute? that's BPM!
					QS = true;                      // set Quantified Self flag
					// QS FLAG IS NOT CLEARED INSIDE THIS ISR
				}
			}

			if (Signal < thresh && Pulse == true) { // when the values are going down, the beat is over

				//digitalWrite(blinkPin, LOW);            // turn off pin 13 LED
				onBlink(LOW);

				Pulse = false;     // reset the Pulse flag so we can do it again
				amp = P - T;                  // get amplitude of the pulse wave
				thresh = amp / 2 + T;      // set thresh at 50% of the amplitude
				P = thresh;                         // reset these for next time
				T = thresh;
			}

			if (N > 2500) {               // if 2.5 seconds go by without a beat
				thresh = 512;                          // set thresh default
				P = 512;                               // set P default
				T = 512;                               // set T default
				lastBeatTime = sampleCounter; // bring the lastBeatTime up to date
				firstBeat = true;                    // set these to avoid noise
				secondBeat = false;            // when we get the heartbeat back
			}

		}
	}
}

