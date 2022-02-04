/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abridger <abridger@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 20:46:58 by abridger          #+#    #+#             */
/*   Updated: 2022/01/27 20:40:05 by abridger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_err_unset(t_shell *data, char *str)
{
	int		i;
	char	*part_str;
	char	*err;

	i = 0;
	part_str = NULL;
	err = NULL;
	while (str[i] != '\0')
	{
		if (str[i] == 61)
		{
			data->exit_status = 127;
			part_str = ft_strjoin2("unset: `", str);
			err = ft_strjoin2(part_str, "': not a valid identifier");
			ft_str_clear(&part_str);
			write(STDERR_FILENO, err, ft_strlen(err));
			write(STDERR_FILENO, "\n", 1);
			ft_str_clear(&err);
			return (127);
		}
		i++;
	}
	return (0);
}

void	ft_del_lst(char *str, t_shell *data)
{
	t_env	*tmp;
	t_env	*envrmnt;
//	t_env	*previous;

	tmp = NULL;
//	previous = NULL;
	envrmnt = data->env;
	while (envrmnt->next)
	{
		if (!ft_strcmp2(envrmnt->key, str))
		{
			envrmnt = envrmnt->next;
			tmp = envrmnt->prev;
//			previous = envrmnt->prev;
			envrmnt->prev = tmp->prev;
			envrmnt->prev->next = envrmnt;
			ft_str_clear(&tmp->key);
			ft_str_clear(&tmp->sep);
			ft_str_clear(&tmp->value);
			free(tmp);
		}
		else
			envrmnt = envrmnt->next;
	}
}

int	ft_exec_unset(t_shell *data, t_info *curr)
{
	int		i;
	int		height;

	i = 1;
	height = ft_height_array(curr->argv);
	if (curr->nb_cmd == 4 && height > 0 && curr->token != 1)
	{
		while (curr->argv[i])
		{
			if (!ft_err_unset(data, curr->argv[i]))
				ft_del_lst(curr->argv[i], data);
			i++;
		}
	}
	return (0);
}
