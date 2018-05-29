#include "parser.h"

sm_error parse_command(char *cmd)
{
	sm_error err;
	char	 err_dsc[256];
	token	 *tokenchain;

	tokenchain = malloc(sizeof(token));

	if((err = tokenize(cmd, tokenchain))) {
		cleanup_tokenchain(tokenchain);
		err_desc(err, err_dsc, sizeof(err_dsc));
		printf("%s: - %s\n", err_dsc, err_get_last());
		return err;
	}         	

	for(token *t = tokenchain; t; t = t->next) {
		printf("Token: %s\n", t->val);
	}

	cleanup_tokenchain(tokenchain);

	return ok;
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
		if(in_quote)
			goto print;
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

sm_error cleanup_tokenchain(token *t)
{
	token *tmp;
	while(t) {
		free(t->val);
		tmp = t;
		t = t->next;
		free(tmp);
	}
	return ok;
}
