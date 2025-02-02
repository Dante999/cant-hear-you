#include "sound_card.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>


Result sound_card_open(Sound_Card *sc, const char *filepath)
{
	sprintf(sc->card_path, "/proc/asound/%s", filepath);

	sc->m_dir = opendir(sc->card_path);

	if (sc->m_dir == NULL) {
		return make_result_error(errno);
	}
	else {
		return make_result_success();
	}
}

void sound_card_close(Sound_Card *sc)
{
	if (sc->m_dir != NULL) {
		closedir(sc->m_dir);
	}
}

bool sound_card_isrunning(const Sound_Card *sc) {

	struct dirent *ent;

	rewinddir(sc->m_dir);

	while ((ent = readdir(sc->m_dir)) != NULL) {

		if (strncmp(ent->d_name, "pcm", strlen("pcm")) != 0 ||
			ent->d_type != DT_DIR) {
			continue;
		}

		char filepath[4096];

		snprintf(filepath, sizeof(filepath), "%s/%s/sub0/status", sc->card_path, ent->d_name);

		FILE *fd = fopen(filepath, "r");
		if (fd == NULL) {
			continue;
		}

		char linebuffer[255];

		while (fgets(linebuffer, sizeof(linebuffer), fd)){
			if (strcmp(linebuffer, "state: RUNNING\n") == 0) {
				fclose(fd);
				return true;
			}
		}

		fclose(fd);
	}

	return false;
}

