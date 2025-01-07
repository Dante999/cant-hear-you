#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>
#include <string.h>

typedef struct {
	bool success;
	char msg[255];
} Result;

Result create_result_error(int errno_val);
Result create_result_fmt(bool success, const char *fmt, ...);
Result create_result_success();

#endif // RESULT_H
