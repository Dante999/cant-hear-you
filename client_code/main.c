#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sound_card.h"
#include "usb_led.h"
#include "libcutils/util_makros.h"
#include "libcutils/arg_parser.h"

static bool running =  true;

void sighandler(int signum)
{
	UNUSED(signum);

	printf("Received signal, aborting...\n");
	running = false;
}



int main(int argc, char *argv[])
{
	signal(SIGINT, sighandler);

	args_add_flag("--help");
	args_add_flag("--show_soundcards");

	args_add_argument("--soundcard");

	args_parse(argc, argv);
	Result res = args_parse(argc, argv);
	if (!res.success) {
		fprintf(stderr, "ERROR: unable to parse launch arguments: %s\n", res.msg);
		args_help(argv[0]);
		return 1;
	}

	if (args_get_flag("--help")) {
		args_help(argv[0]);
		return 0;
	}
	if (args_get_flag("--show_soundcards")) {
		sound_card_show();
		return 0;
	}

	res = args_verify();
	if (!res.success) {
		fprintf(stderr, "ERROR: verifying args: %s\n", res.msg);
		args_help(argv[0]);
		return 1;
	}

	args_print();

	const char *scard_name = args_gets("--soundcard");

	if (scard_name == NULL || strlen(scard_name) == 0) {
		printf(
			"No sound card given! To receive a list of available cards\n"
			"exectute `ls -la /proc/asound | grep card`\n");

		printf("usage %s <card-name>\n", argv[0]);
		return 1;
	}

	Sound_Card sc;

	res = sound_card_open(&sc, scard_name);
	if (!res.success) {
		fprintf(stderr, "ERROR: failed to open sound_card dir: %s\n", res.msg);
		return 1;
	}

	while (running) {

		if (sound_card_isrunning(&sc)) {
			printf("soundcard %s: RUNNING\n", sc.card_path);

			res = usb_led_set(true);
			if (!res.success) {
				fprintf(stderr, "ERROR: failed to set led: %s\n", res.msg);
			}

		}
		else {
			printf("soundcard %s: CLOSED\n", sc.card_path);
			res = usb_led_set(false);
			if (!res.success) {
				fprintf(stderr, "ERROR: failed to set led: %s\n", res.msg);
			}
		}

		sleep(1);
	}

	sound_card_close(&sc);
	printf("closing sound card: %s\n", sc.card_path);

	return 0;
}
