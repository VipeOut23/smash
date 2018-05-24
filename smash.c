#include <stdio.h>
#include <stdlib.h>
#include "smash.h"
#include "utils.h"
#include "strings.h"
#include "error.h"

sm_error read_command(char *buff, size_t *nread)
{
	size_t max = *nread;
	*nread = 0;
	while(*nread < max) {
		if((buff[(*nread)++] = getc(stdin)) == EOF)
			return unexpected_eof;
		if(buff[*nread-1] == '\n')
			return ok;
	}

	return buffer_overflow;
}

sm_error promt()
{
	sm_error err;
	size_t	 nread = 128;
	char	 buff[128];

	printf(sm.promt);
	fflush(stdout);

	if(err = read_command(buff, &nread)) return err;
	buff[nread++] = '\0';

	fwrite(buff, 1, nread, stdout);
	fflush(stdout);

	return ok;
}

sm_error build_promt()
{
	sm_error err;
	off_t	 off = 0;
	off_t	 off_b = 0;
	size_t	 nwrite, buff_max = 256;
	int	 escape = 0;
	char	 buff[buff_max];

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

	free(sm.promt);
	sm.promt = malloc(strlen(buff)+1);
	strcpy(sm.promt, buff);

	return ok;
}

int main(int argc, char **argv)
{
	sm_error err;
	char	 buff[1024];

	err = build_promt();
	
	if(!err)
	while(!(err = promt()));

	err_desc(err, buff, 1024);
	puts(buff);

	free(sm.promt);

	return 0;
}
