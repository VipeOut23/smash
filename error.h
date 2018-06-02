#ifndef ERROR_H__
#define ERROR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_COUNT 9

/**
 * All error types
 */
typedef enum {
	ok,
	undefined,
	invalid_args,
	buffer_overflow,
	unexpected_eof,
	unistd_err,
	unistd_fork_err,
	parser_err,
	xfile_not_found
} sm_error;

/**
 * All error severities
 */
typedef enum {
	notice,
	warn,
	severe
} sm_error_severity;

/**
 * Saves an detailed error description for the last error
 * this will overwrite the old error description
 * @param dsc the description
 */
void err_set_last(char *dsc);

/**
 * Get the detailed error description of the last error (set by err_set_last)
 * @return a pointer to the description
 */
char* err_get_last();

/**
 * Writes an error description to a buffer
 * @param error the error
 * @param buff the buffer to write to
 * @param size write at most <size> bytes
 * @return how many bytes were written
 */
size_t err_desc(sm_error error, char *buff, size_t size);

/**
 * Get the severity of an error
 * @param error the error
 * @return the severity
 */
sm_error_severity err_sev(sm_error error);

/**
 * Writes all errors as a list to a buffer
 * @param buff the buffer to write to
 * @param size write at most <size> bytes
 */
void err_list(char *buff, size_t size);

#endif
