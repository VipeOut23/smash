#ifndef SMASH_H__
#define SMASH_H__

typedef struct {
	char promt_refresh_needed;
} sm_flags;

typedef struct {
	char promt[256];
	char *cache_hostname;
	char *cache_username;
	char *cache_last_cwd;
	sm_flags flags;
} sm_values;

sm_values sm;

#endif
