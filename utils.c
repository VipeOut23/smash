#include "utils.h"

sm_error utils_hostname(char* buff, size_t *nwrite)
{
	if(sm.cache_hostname) {
		strcpy(buff, sm.cache_hostname);
		*nwrite = strlen(sm.cache_hostname);
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
		strcpy(buff, sm.cache_username);
		*nwrite = strlen(sm.cache_username);
	}else {
		char *str;
		if(!(str = getenv("USER"))) return unistd_err;
		sm.cache_username = strdup(str);
		if(sm.cache_username) return utils_username(buff, nwrite);
		return undefined;
	}

	return ok;
}
