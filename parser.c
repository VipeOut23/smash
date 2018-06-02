#include "parser.h"

sm_error parse_command(char *cmd)
{
	sm_error err;
	char	 err_dsc[256];
	token	 *tokenchain;

	tokenchain = malloc(sizeof(token));

	if((err = tokenize(cmd, tokenchain)))
		goto end;

	switch( (err = eval(tokenchain)) ) {
	case parser_err:
		err_desc(err, err_dsc, sizeof(err_dsc));
		printf("%s: %s\n", err_dsc, err_get_last());
		err = ok;
		break;
	default:;
	}
	
 end:
	cleanup_tokenchain(tokenchain);
	return err;
}

sm_error tokenize(char *cmd, token *t)
{
	off_t	cmd_off = 0;
	off_t	buff_off = 0;
	int	in_quote = 0;
	char	buff[1024];

	for(char c = cmd[0]; c != '\0'; c = cmd[++cmd_off]) {
		if(c == '\"') {
			in_quote = !in_quote;
			goto end;
		}
		if(in_quote) goto print;
		if(c == '\n') goto end;
		if(c == ' ') {
			if(!buff_off) goto end;
			buff[buff_off] = '\0';
			t->val = strdup(buff);	
			t->next = malloc(sizeof(token));
			t = t->next;
			buff_off = 0;
			goto end;
		}
	print:
		if(buff_off < sizeof(buff)-1)
			buff[buff_off++] = c;
		else {
			err_set_last("Tokenizer hit single value limit");
			return buffer_overflow;
		}
	end:;
	}

	buff[buff_off] = '\0';
	t->val = strdup(buff);	
	t->next = NULL;

	return ok;
}

void cleanup_tokenchain(token *t)
{
	token *tmp;
	while(t) {
		free(t->val);
		tmp = t;
		t = t->next;
		free(tmp);
	}
}

sm_error eval(token *tokenchain)
{
	command	*c;
	char	buff[256];

	for(int i = 0; i < sizeof(commands)/sizeof(commands[0]); ++i) {
		c = &commands[i];
		if(!strcmp(c->name, tokenchain->val))
			return c->eval(tokenchain->next);
	}

	err_set_last(iprintf(buff, sizeof(buff),
			     "Invalid command: %s",
			     tokenchain->val));
	return parser_err;
}

sm_error eval_run(token *tokenchain)
{
	sm_error err;
	char	 buff[256];
	char	 *argv[64];
	int	 status;
	int	 index = 0;
	pid_t	 pid = 0;

	
	/* Check if we need to update the path with a var from $PATH */
	if(tokenchain->val[0] != '/' && tokenchain->val[0] != '.') {
		err = utils_xfind(tokenchain->val, buff, sizeof(buff));
		if(err) return err;
		free(tokenchain->val);
		tokenchain->val = strdup(buff);
	}

	for(token *t = tokenchain; t; t = t->next)
		argv[index++] = t->val;

	argv[index] = NULL;

	if(err = utils_exec(argv, &pid, &status))
		return err;

	printf("[%d] Exit: %d\n", pid, status);

	return err;
}

sm_error eval_exit(token *tokenchain)
{
	//TODO: Replace with some kind of cleanup routine
	exit(0);
}
