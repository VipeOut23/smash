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

sm_error utils_cwd(char *buff, size_t *nwrite)
{
	if(sm.cache_last_cwd)
		*nwrite = strlcpy(buff, sm.cache_last_cwd, *nwrite);
	return ok;
}

sm_error utils_update_cwd()
{
	char buff[256];

	if(getcwd(buff, sizeof(buff)) < 0) return unistd_err;
	if(!sm.cache_last_cwd || strcmp(buff, sm.cache_last_cwd)) {
		free(sm.cache_last_cwd);
		sm.cache_last_cwd = strdup(buff);
		sm.flags.promt_refresh_needed = 1;
	}

	return ok;
}

sm_error utils_exec(char **argv, pid_t *pid, int *status)
{
	//TODO: Finish functionality
	pid_t child_pid;

	if(!(child_pid = fork())) {
		execv(argv[0], argv);
		exit(0);
	}
	else if(child_pid < 0)
		return unistd_fork_err;

	waitpid(child_pid, status, 0);

	*pid = child_pid;

	return ok;
}
