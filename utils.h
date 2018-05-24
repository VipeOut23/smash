#ifndef UTILS_H__
#define UTILS_H__

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "smash.h"
#include "error.h"

/**
 * Writes the machine's hostname to a buffer
 * @param buff the buffer to write to
 * @param nwrite the buffer's size (will be set to the actual bytes written)
 * @return error
 */
sm_error utils_hostname(char* buff, size_t *nwrite);

/**
 * Writes the executing user's name to a buffer
 * @param buff the buffer to write to
 * @param nwrite the buffer's size (will be set to the actual bytes written)
 * @return error
 */
sm_error utils_username(char* buff, size_t *nwrite);

#endif
