#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_ARGS 100
#define MAX_KEY_LEN        255
#define MAX_VALUE_LEN      255


typedef struct {
	char key[MAX_KEY_LEN];
	char value[MAX_VALUE_LEN];
	char default_value[MAX_VALUE_LEN];
} Arg;


typedef struct {
	size_t count;
	Arg entries[MAX_ARGS];
} Args;

static Args g_args;


void args_add_key(const char *key)
{
	if (g_args.count < MAX_ARGS) {
		Arg *arg = &g_args.entries[g_args.count++];
		strncpy(arg->key, key, MAX_KEY_LEN);
		arg->value[0] = '\0';
	}
}

void args_print()
{
	printf("--- ARGS ---\n");

	for (size_t i = 0; i < g_args.count; i++) {
		Arg *arg = &g_args.entries[i];

		printf("\t%s=%s\n", arg->key, arg->value);
	}

	printf("--------------\n");
}

void args_parse(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "No arguments were given!\n");
		return;
	}


	for (int i=1; i < argc; ++i) {
		const char *val = argv[i];

		char *delimiter = strchr(val, '=');

		if (delimiter == NULL) {
			fprintf(stderr, "argument contains no equal sign: %s\n", val);
			return;
		}

		for (size_t j=0; j < g_args.count; ++j) {
			Arg *stored_arg = &g_args.entries[j];

			if (strncmp(stored_arg->key, val, strlen(stored_arg->key)) != 0) {
				continue;
			}

			if (strlen(delimiter) < 1) {
				fprintf(stderr, "argument contains no value: %s\n", val);
				return;
			}

			strncpy(stored_arg->value, (delimiter+1), MAX_KEY_LEN);

		}
	}

}

const char *args_gets(const char *key)
{
	for (size_t i = 0; i < g_args.count; ++i) {

		if (strncmp(g_args.entries[i].key, key, MAX_KEY_LEN) == 0) {
			return g_args.entries[i].value;
		}
	}

	return NULL;
}

int config_geti(const char *key)
{
	for (size_t i = 0; i < g_args.count; ++i) {

		if (strncmp(g_args.entries[i].key, key, MAX_KEY_LEN) == 0) {
			return atoi(g_args.entries[i].value);
		}
	}

	return 0;
}

