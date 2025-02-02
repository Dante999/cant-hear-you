#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "sound_card.h"
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

	args_add_argument("--soundcard");
	args_add_argument("--serial");
	args_parse(argc, argv);

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

	Result ret = sound_card_open(&sc, scard_name);
	if (!ret.success) {
		fprintf(stderr, "Unable to open sound_card dir: %s\n", ret.msg);
		return 1;
	}

	while (running) {

		if (sound_card_isrunning(&sc)) {
			printf("soundcard %s: RUNNING\n", sc.card_path);
		}
		else {
			printf("soundcard %s: CLOSED\n", sc.card_path);
		}

		sleep(1);
	}

	sound_card_close(&sc);
	printf("closing sound card: %s\n", sc.card_path);

	return 0;
}
