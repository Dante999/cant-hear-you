#ifndef SOUND_CARD_H
#define SOUND_CARD_H

#include <stdbool.h>
#include <linux/limits.h>
#include <dirent.h>

#include "libcutils/result.h"

typedef struct {
	char card_path[2096];
	DIR *m_dir;
} Sound_Card;

Result sound_card_open(Sound_Card *sc, const char *filepath);
bool sound_card_isrunning(const Sound_Card *sc);
void sound_card_close(Sound_Card *sc);

#endif // SOUND_CARD_H
