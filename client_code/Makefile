LIBCUTILS_SRC_DIR=../thirdparty/libcutils/src
LIBCUTILS_INC_DIR=../thirdparty/libcutils/include

LIBVUSB_SRC_DIR=../thirdparty/v-usb/libs-host
LIBVUSB_INC_DIR=../thirdparty/v-usb/libs-host

USBFLAGS = `pkg-config --cflags libusb-1.0`
USBLIBS = `pkg-config --libs libusb-1.0`

all:
	mkdir -p build
	${CC} -Wall -Wextra -Wpedantic \
		${USBFLAGS} \
		-o build/canthearyou \
		-I ${LIBCUTILS_INC_DIR} \
		-I ${LIBVUSB_INC_DIR} \
		main.c \
		sound_card.c \
		usb_led.c \
		${LIBCUTILS_SRC_DIR}/arg_parser.c \
		${LIBCUTILS_SRC_DIR}/result.c \
		${LIBCUTILS_SRC_DIR}/util_strings.c \
		${LIBVUSB_SRC_DIR}/opendevice.c \
		${USBLIBS} 

clean:
	rm -rf build
