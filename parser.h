#ifndef PARSER_H__
#define PARSER_H__

#include <string.h>
#include <bsd/string.h>
#include "smash.h"
#include "utils.h"
#include "error.h"

typedef struct token token;
struct token{
	char *val;
	token *next;	
};

typedef struct {
	char *name;
	sm_error (*eval)(token*);
} command;

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

/**
 * Evaluate the tokenchain
 * @param tokenchain the tokenchain to evaluate
 * @return error
 */
sm_error eval(token *tokenchain);

/*
 *******************************************************************************
 * All evaluateable comments
 *******************************************************************************
 */
sm_error eval_run(token *tokenchain);
sm_error eval_exit(token *tokenchain);
sm_error eval_cd(token *tokenchain);

/* An index for the commands (searching will be from top to bottom) */
static command commands[] = {
	{"run", &eval_run},
	{"cd", &eval_cd},
	{"exit", &eval_exit}
};

#endif
