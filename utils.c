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
	pid_t child_pid;

	if(!(child_pid = fork())) {
		execv(argv[0], argv);
		exit(0);
	}
	else if(child_pid < 0)
		return unistd_fork_err;

	if(!*pid)
		waitpid(child_pid, status, 0);

	*pid = child_pid;

	return ok;
}

sm_error utils_xfind(char *name, char *path, size_t size)
{
	char env_path[1024];
	char *env_path_ptr = env_path;
	char bin_path[256];
	char *token;

	strncpy(env_path, getenv("PATH"), sizeof(env_path));

	while(token = strsep(&env_path_ptr, ":")) {
		strcpy(bin_path, token);
		strcat(bin_path, "/");
		strcat(bin_path, name);
		if(!access(bin_path, X_OK)) {
			strncpy(path, bin_path, size);
			return ok;
		}
	}

	err_set_last(name);

	return xfile_not_found;
}
