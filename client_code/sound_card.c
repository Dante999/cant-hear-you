#include "sound_card.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define ENABLE_SC_LOG 0

#if ENABLE_SC_LOG
  #define SC_LOG( fmt, ... ) printf(fmt, ##__VA_ARGS__ )
#else
  #define SC_LOG( ... )
#endif



bool sound_card_open(Sound_Card *sc, const char *filepath)
{
	sprintf(sc->card_path, "/proc/asound/%s", filepath);

	sc->m_dir = opendir(sc->card_path);

	return (sc->m_dir != NULL) ? true : false;
}

void sound_card_close(Sound_Card *sc)
{
	if (sc->m_dir != NULL) {
		closedir(sc->m_dir);
	}
}

bool sound_card_isrunning(const Sound_Card *sc) {

	SC_LOG("checking soundcard %s\n", sc->card_path);
	struct dirent *ent;

	rewinddir(sc->m_dir);

	while ((ent = readdir(sc->m_dir)) != NULL) {

		if (strncmp(ent->d_name, "pcm", strlen("pcm")) != 0 || 
			ent->d_type != DT_DIR) {
			continue; 
		}

		char filepath[2096];
		
		snprintf(filepath, sizeof(filepath), "%s/%s/sub0/status", sc->card_path, ent->d_name);
		SC_LOG("\t%s ...", filepath);

		FILE *fd = fopen(filepath, "r");
		int err = errno;
		if (fd == NULL) {
			SC_LOG("UNABLE_TO_OPEN: %s\n", strerror(err));
			continue;
		}

		char linebuffer[255];

		while (fgets(linebuffer, sizeof(linebuffer), fd)){
			if (strcmp(linebuffer, "state: RUNNING\n") == 0) {
				SC_LOG("RUNNING\n");
				fclose(fd);
				return true;
			}
		}
		
		SC_LOG("CLOSED\n");
		fclose(fd);
		
	}

	return false;

}

