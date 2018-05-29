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

sm_error parse_command(char *cmd);
sm_error tokenize(char *cmd, token *t);
sm_error cleanup_tokenchain(token *t);

#endif
