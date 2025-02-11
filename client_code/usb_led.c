/* Name: set-led.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/*
   General Description:
   This is the host-side driver for the custom-class example device. It searches
   the USB for the LEDControl device and sends the requests understood by this
   device.
   This program must be linked with libusb on Unix and libusb-win32 on Windows.
   See http://libusb.sourceforge.net/ or http://libusb-win32.sourceforge.net/
   respectively.
   */
#include "usb_led.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb.h>        /* this is libusb */
#include "opendevice.h" /* common code moved to separate module */

#include "../firmware/requests.h"   /* custom request numbers */
#include "../firmware/usbconfig.h"  /* device's VID/PID and names */


static Result usb_led_open(libusb_device_handle **handle)
{
	const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID};
	const unsigned char rawPid[2] = {USB_CFG_DEVICE_ID};
	char vendor[]  = {USB_CFG_VENDOR_NAME, 0};
	char product[] = {USB_CFG_DEVICE_NAME, 0};

	/* compute VID/PID from usbconfig.h so that there is a central source of information */
	const int vid = rawVid[1] * 256 + rawVid[0];
	const int pid = rawPid[1] * 256 + rawPid[0];

	int r = libusb_init(NULL);
	if (r != 0) {
		return make_result(false, "Warning: cannot initialize libusb: %s\n", libusb_strerror(r));
	}

	if (usbOpenDevice(handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0) {
		return make_result(false, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
	}

	/* The following function is in opendevice.c: */
	/* Since we use only control endpoint 0, we don't need to choose a
	 * configuration and interface. Reading device descriptor and setting a
	 * configuration and interface is done through endpoint 0 after all.
	 * However, newer versions of Linux require that we claim an interface
	 * even for endpoint 0. Enable the following code if your operating system
	 * needs it: */
#if 0
	int retries = 1, usbConfiguration = 1, usbInterface = 0;
	r = libusb_set_configuration(handle, usbConfiguration);
	if (r != 0 && showWarnings) {
		fprintf(stderr, "Warning: could not set configuration: %s\n", libusb_strerror(r));
	}
	/* now try to claim the interface and detach the kernel HID driver on
	 * Linux and other operating systems which support the call. */
	while((len = libusb_claim_interface(handle, usbInterface)) != 0 && retries-- > 0) {
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
		r = libusb_detach_kernel_driver(handle, 0);
		if (r != 0 && showWarnings) {
			fprintf(stderr, "Warning: could not detach kernel driver: %s\n", libusb_strerror(r));
		}
#endif
	}
#endif

	return make_result_success();
}

Result usb_led_set(bool on)
{
	libusb_device_handle *handle = NULL;

	Result result = usb_led_open(&handle);
	if (!result.success) {
		return result;
	}



	char buffer[4];
	int led_status = (on ? 1 : 0);

	int cnt = libusb_control_transfer(
		handle,
		LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT, CUSTOM_RQ_SET_STATUS,
		led_status,
		0,
		(unsigned char *)buffer,
		0,
		5000);

		if (cnt < 0) {
			result = make_result(false, "USB error: %s\n", libusb_strerror(cnt));
		}

	libusb_close(handle);
	return make_result_success();
}

