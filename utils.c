#include "utils.h"

char *iprintf(char *buff, size_t size, char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vsnprintf(buff, size, fmt, args);
	va_end(args);

	return buff;
}

sm_error utils_hostname(char* buff, size_t *nwrite)
{
	if(sm.cache_hostname) {
		*nwrite = strlcpy(buff, sm.cache_hostname, *nwrite);
	}else {
		char str[128];
		if(gethostname(str, 128)) return unistd_err;
		sm.cache_hostname = strdup(str);
		if(sm.cache_hostname) return utils_hostname(buff, nwrite);
		return undefined;
	}

	return ok;
}

sm_error utils_username(char* buff, size_t *nwrite)
{
	if(sm.cache_username) {
		*nwrite = strlcpy(buff, sm.cache_username, *nwrite);
	}else {
		char *str;
		if(!(str = getenv("USER"))) return unistd_err;
		sm.cache_username = strdup(str);
		if(sm.cache_username) return utils_username(buff, nwrite);
		return undefined;
	}

	return ok;
}

sm_error utils_pwd(char *buff, size_t *nwrite)
{
	if(sm.cache_last_pwd)
		*nwrite = strlcpy(buff, sm.cache_last_pwd, *nwrite);
	return ok;
}

sm_error utils_update_pwd()
{
	char *var;

	if(!(var = getenv("PWD"))) return unistd_err;
	if(!sm.cache_last_pwd || strcmp(var, sm.cache_last_pwd)) {
		free(sm.cache_last_pwd);
		sm.cache_last_pwd = strdup(var);
		sm.flags.promt_refresh_needed = 1;
	}

	return ok;
}
