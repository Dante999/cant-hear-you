#include "sound_card.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "libcutils/util_strings.h"
#include "libcutils/util_makros.h"

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

void sound_card_show() {
	static const char *names_to_ignore[] = {
		".",
		"..",
		"oss",
		"seq"
		};


	DIR *sc_basedir = opendir("/proc/asound");

	if (sc_basedir == NULL) {
		fprintf(stderr,
			"ERROR: unable to show folder in /proc/asound: %s\n",
			strerror(errno));
		return;
	}

	struct dirent *ent;
	// TODO: show symlink locations (e.g. AudioPCI -> card0)
	printf("Showing available cards in /proc/asound/...\n");
	while ((ent = readdir(sc_basedir)) != NULL) {

		if (utils_stranyof(ent->d_name,
		                   names_to_ignore,
		                   ARRAY_SIZE(names_to_ignore))) {
			continue;
		}

		if (ent->d_type == DT_DIR) {
			printf("\t* %s\n", ent->d_name);
		}
		else if (ent->d_type == DT_LNK) {
			char symlink[1024];
			char symlink_target[1024];

			snprintf(symlink, sizeof(symlink), "/proc/asound/%s", ent->d_name);
			ssize_t ret = readlink(symlink, symlink_target, sizeof(symlink_target));
			int errval = errno;
			if (ret == -1) {
				fprintf(stderr, "ERROR: unable to read symlink %s: %s\n",
						symlink, strerror(errval));
			}
			printf("\t* %s -> %s\n", ent->d_name, symlink_target);
		}
	}

	closedir(sc_basedir);
}
