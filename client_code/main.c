#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#define DEFAULT_CARD "card0"
#define MAX_BUFFER 2096


typedef struct {
	char card_path[MAX_BUFFER];
	DIR *m_dir;
} Sound_Card;

static bool sound_card_open(Sound_Card *sc, const char *filepath)
{
	sprintf(sc->card_path, "/proc/asound/%s", filepath);

	sc->m_dir = opendir(sc->card_path);

	return (sc->m_dir != NULL) ? true : false;
}

static void sound_card_close(Sound_Card *sc)
{
	if (sc->m_dir != NULL) {
		closedir(sc->m_dir);
	}
}

static bool sound_card_isrunning(const Sound_Card *sc) {

	printf("checking soundcard %s\n", sc->card_path);
	struct dirent *ent;

	rewinddir(sc->m_dir);

	while ((ent = readdir(sc->m_dir)) != NULL) {

		if (strncmp(ent->d_name, "pcm", strlen("pcm")) != 0 || 
			ent->d_type != DT_DIR) {
			continue; 
		}

		char filepath[MAX_BUFFER];
		
		snprintf(filepath, sizeof(filepath), "%s/%s/sub0/status", sc->card_path, ent->d_name);
		printf("\t%s ...", filepath);

		FILE *fd = fopen(filepath, "r");

		if (fd == NULL) {
			printf("UNABLE_TO_OPEN\n");
			continue;
		}

		char linebuffer[255];

		while (fgets(linebuffer, sizeof(linebuffer), fd)){
			if (strcmp(linebuffer, "state: RUNNING\n") == 0) {
				printf("RUNNING\n");
				fclose(fd);
				return true;
			}
		}
		
		printf("CLOSED\n");
		fclose(fd);
		
	}

	return false;

}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf(
			"No sound card given! To receive a list of available cards\n"
			"exectute `ls -la /proc/asound | grep card`\n");

		printf("usage %s <card-name>\n", argv[0]);
		return 1;
	}

	Sound_Card sc;


	if (!sound_card_open(&sc, argv[1])) {
		fprintf(stderr, "Unable to open dir!");
		return 1;
	}

	printf("cool! opened folder!\n");

	for( size_t i=0; i < 5; ++i) {
		bool isrunning = sound_card_isrunning(&sc);

	//	printf("Sound Card: %s\n", isrunning ? "STREAM" : "STOPPEND");
		sleep(1);
	}

	sound_card_close(&sc);






	return 0;
}
