#ifndef ERROR_H__
#define ERROR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_COUNT 6

/**
 * All error types
 */
typedef enum {
	ok,
	undefined,
	invalid_args,
	buffer_overflow,
	unexpected_eof,
	unistd_err
} sm_error;

/**
 * Saves an detailed error description for the last error
 * @param dsc the description
 */
void err_set_last(char *dsc);

/**
 * Get the detailed error description of the last error (set by err_set_last)
 * this will overwrite the old error description
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
 * Writes all errors as a list to a buffer
 * @param buff the buffer to write to
 * @param size write at most <size> bytes
 */
void err_list(char *buff, size_t size);

#endif
