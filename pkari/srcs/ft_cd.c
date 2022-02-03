/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abridger <abridger@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 18:12:43 by abridger          #+#    #+#             */
/*   Updated: 2022/01/27 20:52:35 by abridger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_add_oldpwd_env(t_shell *data, char *curr_pwd)
{
	t_env	*tmp;
	t_env	*new;
	char	*str;

	tmp = data->env;
	str = ft_strjoin2("OLDPWD=", curr_pwd);
	new = ft_lstnew(str);
	ft_lstadd_back(&tmp, new);
	ft_str_clear(&curr_pwd);
	ft_str_clear(&str);
}

void	ft_change_oldpwd_env(t_shell *data, char *curr_pwd, int check)
{
	t_env	*tmp;

	tmp = data->env;
	if (check == 1)
	{
		while (tmp)
		{
			if (0 == ft_strcmp2(tmp->key, "OLDPWD"))
			{
				free(tmp->value);
				tmp->value = ft_strdup(curr_pwd);
				check = 0;
			}
			tmp = tmp->next;
		}
		if (check == 1)
			ft_add_oldpwd_env(data, curr_pwd);
	}
}

void	ft_change_pwd_env(t_shell *data, char *curr_pwd, char *new_pwd)
{
	t_env	*tmp;
	int		check;

	tmp = data->env;
	check = 0;
	while (tmp && check == 0)
	{
		if (0 == ft_strcmp(tmp->key, "PWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(new_pwd);
			free(new_pwd);
			check = 1;
		}
		tmp = tmp->next;
	}
	if (check == 1)
		ft_change_oldpwd_env(data, curr_pwd, check);
}

int	ft_exec_cd(t_shell *data, t_info *curr)
{
	char	*buf1;
	char	*buf2;
	char	*curr_pwd;
	char	*new_pwd;
	int		height;

	buf1 = NULL;
	buf2 = NULL;
	height = ft_height_array(curr->argv);
	if (curr->nb_cmd == 1 && height > 0 && curr->is_pipe == 0 \
		&& data->count == 1)
	{
		curr_pwd = getcwd(NULL, sizeof(buf1));
		if (chdir(curr->argv[1]) < 0)
			return (ft_error(data, ft_two_colon("cd", curr->argv[1])));
		new_pwd = getcwd(NULL, sizeof(buf2));
		ft_change_pwd_env(data, curr_pwd, new_pwd);
	}
	return (0);
}
