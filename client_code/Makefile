LIBCUTILS_SRC_DIR=../thirdparty/libcutils/src
LIBCUTILS_INC_DIR=../thirdparty/libcutils/include

all:
	mkdir -p build
	${CC} -Wall -Wextra -Wpedantic -o build/canthearyou \
		-I ${LIBCUTILS_INC_DIR} \
		main.c \
		sound_card.c \
		${LIBCUTILS_SRC_DIR}/result.c \
		${LIBCUTILS_SRC_DIR}/arg_parser.c

clean:
	rm canthearyou
