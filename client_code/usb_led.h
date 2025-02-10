#ifndef USB_LED_H
#define USB_LED_H

#include <stdbool.h>

#include "libcutils/result.h"

Result usb_led_set(bool on);

#endif // USB_LED_H
