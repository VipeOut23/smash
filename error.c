#include "error.h"

static char *err_desc_ptr;

void err_set_last(char *dsc)
{
	free(err_desc_ptr);
	err_desc_ptr = strdup(dsc);
}

char* err_get_last()
{
	return err_desc_ptr;
}

size_t err_desc(sm_error error, char *buff, size_t size)
{
	if(size < 0) return 0;

	char   *desc;
	size_t nwrite;

	switch(error) {
	case ok:		desc = "Ok"; break;
	case undefined:		desc = "Undefined error"; break;
	case invalid_args:	desc = "Invalid arguments"; break;
	case buffer_overflow:	desc = "Buffer overflow"; break;
	case unexpected_eof:	desc = "Unexpected EOF"; break;
	case unistd_err:	desc = "Unistd library error"; break;
	case unistd_fork_err:	desc = "Fork error"; break;
	case parser_err:	desc = "Parser encounterd error"; break;
	}

	nwrite = strlen(desc);
	nwrite = (nwrite <= size)?nwrite:size;
	strncpy(buff, desc, nwrite);

	return nwrite;
}

void err_list(char *buff, size_t size)
{
	off_t off = 0;

	for(int i = 0; i < ERR_COUNT && off < size; ++i) {
		off += snprintf((char*) buff+off, size-off, "No. %03d: ", i);
		off += err_desc((sm_error) i, (char*) buff+off, size-off);
		if(off<size) buff[off++] = '\n';
	}
}
