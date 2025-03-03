/* Name: main.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/*
   This example should run on most AVRs with only little changes. No special
   hardware resources except INT0 are used. You may have to change usbconfig.h for
   different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
   at least be connected to INT0 as well.
   We assume that an LED is connected to port B bit 0. If you connect it to a
   different port or bit, change the macros below:
 */


#define LED_INIT()   (DDRB  |= _BV(PB0))
#define LED_STATUS() (PORTB & _BV(PB0))
#define LED_ON()     (PORTB &= ~_BV(PB0))
#define LED_OFF()    (PORTB |= _BV(PB0))

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"
#include "usbdrv/oddebug.h"        /* This is also an example for using debug macros */
#include "requests.h"       /* The custom request numbers we use */

#define abs(x) ((x) > 0 ? (x) : (-x))

// Called by V-USB after device reset
void hadUsbReset() {
	int frameLength, targetLength = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
	int bestDeviation = 9999;
	uchar trialCal, bestCal, step, region;

	// do a binary search in regions 0-127 and 128-255 to get optimum OSCCAL
	for(region = 0; region <= 1; region++) {
		frameLength = 0;
		trialCal = (region == 0) ? 0 : 128;

		for(step = 64; step > 0; step >>= 1) {
			if(frameLength < targetLength) // true for initial iteration
				trialCal += step; // frequency too low
			else
				trialCal -= step; // frequency too high

			OSCCAL = trialCal;
			frameLength = usbMeasureFrameLength();

			if(abs(frameLength-targetLength) < bestDeviation) {
				bestCal = trialCal; // new optimum found
				bestDeviation = abs(frameLength -targetLength);
			}
		}
	}

	OSCCAL = bestCal;
}



/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	static uchar    dataBuffer[4];  /* buffer must stay valid when usbFunctionSetup returns */

	if(rq->bRequest == CUSTOM_RQ_ECHO){ /* echo -- used for reliability tests */
		dataBuffer[0] = rq->wValue.bytes[0];
		dataBuffer[1] = rq->wValue.bytes[1];
		dataBuffer[2] = rq->wIndex.bytes[0];
		dataBuffer[3] = rq->wIndex.bytes[1];
		usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
		return 4;
	}
	else if(rq->bRequest == CUSTOM_RQ_SET_STATUS){
		if(rq->wValue.bytes[0] & 1){    /* set LED */
			LED_ON();
		}
		else{                          /* clear LED */
			LED_OFF();
		}
	}
	else if(rq->bRequest == CUSTOM_RQ_GET_STATUS){
		dataBuffer[0] = (LED_STATUS() != 0);
		usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
		return 1;                       /* tell the driver to send 1 byte */
	}
	return 0;   /* default for not implemented requests: return no data back to host */
}

int main(void)
{
	LED_INIT();
	LED_ON();
	wdt_enable(WDTO_1S);
	usbInit();

	usbDeviceDisconnect();

	for( uint8_t i=0; i < 250; ++i) {
		wdt_reset();
		_delay_ms(2);
	}

	usbDeviceConnect();

	sei();

	LED_OFF();
	while(1) {
		wdt_reset();
		usbPoll();
	}

	return 0;
}

