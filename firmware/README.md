Canthearyou Firmware
====================
This firmware is targeted for the attiny85, more specific for the digispark
tiny85 board with V-USB compatible hardware.

The firmware is basically a combination of the v-usb example for the attiny85
from "codeandlife" (see resources) and the custom class example of the v-usb lib
itself.


Info
----

	USB_P = PB4
	USB_M = PB3
	LED   =

	LFUSE = 0xE1
	HFUSE = 0xDD

	Clock: PLL (16MHz) but F_CPU=16500000

**ATTENTION:** Since the Digispark tiny85 does not use INT0 on D+ the default
configuration of usbconfig.h will not work! You have to change the
interrupts on the bottom of the file.

Many thanks to user nick which figured this out and wrote it in the
comments:

	---------------------------------------------------------------------------
	nick:
	---------------------------------------------------------------------------

	Yes, you are right. I missed a block of code that was not were it was
	supposed to be (at the end of the file). This is was micronucleus is using
	to setup the interrupts. You’ll need to add it on the bottom of the
	usbconfig.h file of this project.


	#define USB_INTR_CFG PCMSK
	#define USB_INTR_CFG_SET (1 << USB_CFG_DPLUS_BIT)
	#define USB_INTR_CFG_CLR 0
	#define USB_INTR_ENABLE GIMSK
	#define USB_INTR_ENABLE_BIT PCIE
	#define USB_INTR_PENDING GIFR
	#define USB_INTR_PENDING_BIT PCIF
	#define USB_INTR_VECTOR PCINT0_vect

	I got it working!

	source: https://codeandlife.com/2012/02/22/v-usb-with-attiny45-attiny85-without-a-crystal/
	---------------------------------------------------------------------------


## Resources
- https://github.com/yasir-shahzad/Digispark-ATTINY85/blob/master/documents/ATtiny85-schematic.pdf
- https://www.obdev.at/products/vusb/easylogger.html
- https://www.az-delivery.de/products/digispark-board
- https://www.microchip.com/en-us/product/ATtiny85#Documentation
- https://github.com/obdev/v-usb
- https://codeandlife.com/2012/01/29/avr-attiny-usb-tutorial-part-1/
- https://codeandlife.com/2012/02/22/v-usb-with-attiny45-attiny85-without-a-crystal/
