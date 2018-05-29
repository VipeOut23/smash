#ifndef PARSER_H__
#define PARSER_H__

#include <string.h>
#include <bsd/string.h>
#include "smash.h"
#include "error.h"

typedef struct token token;
struct token{
	char *val;
	token *next;	
};

/**
 * Parse a commandline
 * @param cmd a null terminated string
 * @return error
 */
sm_error parse_command(char *cmd);

/**
 * Tokenize a commandline for better parsing
 * @param cmd a null terminated string
 * @param t a poniter to a token
 * @return error
 */
sm_error tokenize(char *cmd, token *t);

/**
 * Frees an entire tokenchain from memory
 * @param t the first element of the tokenchain
 */
void cleanup_tokenchain(token *t);

#endif
