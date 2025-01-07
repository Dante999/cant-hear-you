#include "result.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

Result create_result_error(int errno_val)
{
	Result result;

	result.success = false;
	strncpy(result.msg, strerror(errno), sizeof(result.msg));

	return result;
}

Result create_result_fmt(bool success, const char *fmt, ...)
{
	Result result;
	result.success = success;

	va_list arg_list;

	va_start(arg_list, fmt);
	vsnprintf(result.msg, sizeof(result.msg), fmt, arg_list);
	va_end(arg_list);

	return result;
}

Result create_result_success()
{
	Result result = {true, ""};
	return result;
}

