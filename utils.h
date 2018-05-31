#ifndef UTILS_H__
#define UTILS_H__

#include <string.h>
#include <bsd/string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "smash.h"
#include "error.h"

/**
 * A wrapper for printf to format strings inplace
 * @param buff the buffer to use
 * @param size the buffers size
 * @param fmt the format
 * @param ... the arguments
 * @return a pointer to the buffer
 */
char *iprintf(char *buff, size_t size, char *fmt, ...);

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

/**
 * Writes the current working directory to a buffer
 * @param buff the buffer to write to
 * @param nwrite the buffer's size (will be set to the actual bytes written)
 * @return error
 */
sm_error utils_cwd(char *buff, size_t *nwrite);

/**
 * Updates the last cached cwd
 * @return error
 */
sm_error utils_update_cwd();

/**
 * executes a program
 * @param argv the arguments to run the program (MUST be terminated with a nullpointer)
 * @param pid if it points to a non-zero value the program will be called asynchron and
 * pid will point to the child's pid; otherwise (zero-value) the program will run 
 * in sync (blocking)
 * @param status the return code of the program will be in there
 */
sm_error utils_exec(char **argv, pid_t *pid, int *status);

/**
 * Tries to find an executeable inside any of the paths in enviroment
 * variable $PATH
 * @param name the executeables name
 * @param path the path will be written in there
 * @param size size of the path buffer
 * @return errors
 */
sm_error utils_xfind(char *name, char *path, size_t size);

#endif
