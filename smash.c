#include <stdio.h>
#include <stdlib.h>
#include "smash.h"
#include "utils.h"
#include "strings.h"
#include "error.h"
#include "parser.h"

sm_error read_command(char *buff, size_t *nread);
sm_error build_promt();
sm_error before_promt_hook();
sm_error promt();

sm_error read_command(char *buff, size_t *nread)
{
	size_t max = *nread;
	*nread = 0;
	while(*nread < max) {
		if((buff[*nread] = getc(stdin)) == EOF)
			return unexpected_eof;
		if(buff[(*nread)++] == '\n')
			return ok;
	}

	return buffer_overflow;
}

inline sm_error before_promt_hook()
{
	utils_update_cwd();
	return ok;
}

sm_error promt()
{
	sm_error err;
	size_t	 nread = 128;
	char	 buff[128];

	before_promt_hook();
	if(sm.flags.promt_refresh_needed)
		if(err = build_promt()) return err;

	printf(sm.promt);
	fflush(stdout);

	if(err = read_command(buff, &nread)) return err;
	buff[nread++] = '\0';

	if((err = parse_command(buff))) {
		switch(err_sev(err)) {
		case warn:	/* Fall through */
		case notice:
			err_desc(err, buff, sizeof(buff));
			fprintf(stderr, "%s: %s\n", buff, err_get_last());
			return ok;
		case severe: return err;
		}
	}

	return ok;
}

sm_error build_promt()
{
	sm_error err;
	off_t	 off = 0;
	off_t	 off_b = 0;
	size_t	 nwrite, buff_max = sizeof(sm.promt);
	int	 escape = 0;
	char	 *buff = sm.promt;

	for(char c = str_promt[off++];
	    c != '\0' && off_b < buff_max;
	    c = str_promt[off++]) {
		if(escape) {
			nwrite = buff_max - off_b;
			if(nwrite < 0) return buffer_overflow;

			switch(c) {
			case 'h':
				err = utils_hostname((char*) buff+off_b, &nwrite);
				break;
			case 'u':
				err = utils_username((char*) buff+off_b, &nwrite);
				break;
			case 'd':
				err = utils_cwd((char*) buff+off_b, &nwrite);
				break;
			default: nwrite = 0;
			}

			if(err) return err;
			off_b += nwrite;
			escape = 0;
		}else if(c == '\\') {
			escape = 1;
		}else {
			buff[off_b++] = c;
		}
	}

	buff[off_b] = '\0';
	sm.flags.promt_refresh_needed = 0;

	return ok;
}

int main(int argc, char **argv)
{
	sm_error err;
	char	 buff[1024];

	sm = (sm_values){0};

	while(!(err = promt()));

	err_desc(err, buff, 1024);
	fwrite(buff, 1, strlen(buff), stderr);

	return 0;
}
