#include "minishell.h"

void token_handler(t_shell *msh, int *i)
{
	char **tmp;
	int n;

	if (msh->info->token == TOKEN_REDIRECT_INPUT)
		redirect_input(msh, i);
	else if (msh->info->token == TOKEN_REDIRECT_OUTPUT1
		|| msh->info->token == TOKEN_REDIRECT_OUTPUT2)
		redirect_output(msh, i);
	else if (msh->info->token == TOKEN_HEREDOC)
		redirect_heredoc(msh, i);
	else
	{
		n = 0;
		tmp = (char **)malloc(sizeof(char *) * (msh->info->argc + 1));
		msh->info->argc++;
		while (n < msh->info->argc - 1)
		{
			tmp[n] = msh->info->argv[n];
			n++;
		}
		if (msh->info->argv != NULL)
			free(msh->info->argv);
		tmp[n] = ft_substr(msh->str, 0, *i);
		tmp[n + 1] = NULL;
		msh->info->argv = tmp;
	}
}

int token_redirects(t_shell *msh, int *i, int token)
{
	char *tmp;

	if (*i != 0)
		token_handler(msh, i);
	tmp = NULL;
	msh->info->token = token;
	if (token == TOKEN_HEREDOC || token == TOKEN_REDIRECT_OUTPUT2)
		(*i)++;
	while (msh->str[*i] != 0 &&(msh->str[*i + 1] == ' '
		|| msh->str[*i + 1] == '\t'))
		(*i)++;
	if (msh->str[*i + 1] == 0 || msh->str[*i + 1] == '|'
		|| msh->str[*i + 1] == '<' || msh->str[*i + 1] == '>')
		return (syntax_error(msh, msh->str + *i + 1, 1));
	if (msh->str[*i] != 0 && msh->str[*i + 1] != 0)
		tmp = ft_strdup(msh->str + *i + 1);
	(*i) = -1;
	free(msh->str);
	msh->str = tmp;
	return (0);
}

int token_pipe(t_shell *msh, int *i)
{
	char *tmp;

	if (*i != 0)
		token_handler(msh, i);
	tmp = NULL;
	msh->info->token = TOKEN_PIPE;
	add_info(msh);
	while (msh->str[*i] != 0 &&(msh->str[*i + 1] == ' '
		|| msh->str[*i + 1] == '\t'))
		(*i)++;
	if (msh->str[*i + 1] == 0 || msh->str[*i + 1] == '|'
		|| msh->str[*i + 1] == '<' || msh->str[*i + 1] == '>')
		return (syntax_error(msh, msh->str + *i + 1, 1));
	if (msh->str[*i] != 0 && msh->str[*i + 1] != 0)
		tmp = ft_strdup(msh->str + *i + 1);
	(*i) = -1;
	free(msh->str);
	msh->str = tmp;
	msh->have_a_pipe = 1;
	return (0);
}

int token_space(t_shell *msh, int *i)
{
	char *tmp;

	token_handler(msh, i);
	tmp = NULL;
	while (msh->str[*i] != 0 &&(msh->str[*i + 1] == ' '
		|| msh->str[*i + 1] == '\t'))
		(*i)++;
	if (msh->str[*i] != 0 && msh->str[*i + 1] != 0)
		tmp = ft_strdup(msh->str + *i + 1);
	(*i) = -1;
	free(msh->str);
	msh->str = tmp;
	return (0);
}
