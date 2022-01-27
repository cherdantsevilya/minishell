/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abridger <abridger@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 17:15:16 by abridger          #+#    #+#             */
/*   Updated: 2022/01/27 22:41:24 by abridger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_check(const char *line, t_env *tmp)
{
	int		len_key;
	char	*key;
	char	*sep;
	int		check;

	len_key = ft_len_key((char *)line);
	check = 0;
	key = ft_substr(line, 0, len_key);
	if (ft_strlen((char *)line) - len_key >= 1)
		sep = ft_substr(line, len_key, 1);
	else
		sep = NULL;
	while (tmp && check == 0)
	{
		if (!ft_strcmp(key, tmp->key) && !ft_strcmp(sep, tmp->sep))
			check = 1;
		else if (!ft_strcmp(key, tmp->key) && ft_strcmp(sep, tmp->sep))
			check = 2;
		tmp = tmp->next;
	}
	ft_str_clear(&key);
	ft_str_clear(&sep);
	return (check);
}

void	ft_lst_change_value(t_env *lst, const char *line)
{
	int		len_key;
	int		len_value;
	char	*key;
	int		done;

	done = 0;
	len_key = ft_len_key((char *)line);
	len_value = ft_len_value((char *)line);
	key = ft_substr(line, 0, len_key);
	while (lst && !done)
	{
		if (!ft_strcmp(key, lst->key))
		{
			ft_str_clear(&lst->value);
			if (len_value > 0)
				lst->value = ft_substr(line, len_key + 1, len_value);
			else
				lst->value = NULL;
			done = 1;
		}
		lst = lst->next;
	}
	ft_str_clear(&key);
}

void	ft_add_variable(t_info *curr, t_shell *data)
{
	int		i;
	int		check;
	t_env	*new;
	t_env	*env;
	t_env	*tmp;

	i = 1;
	new = NULL;
	while (curr->argv[i])
	{
		tmp = data->env;
		env = data->env;
		check = ft_env_check(curr->argv[i], tmp);
		if (check == 0)
		{
			new = ft_lstnew(curr->argv[i]);
			ft_lstadd_back(&env, new);
		}
		else if (check == 1)
			ft_lst_change_value(env, curr->argv[i]);
		i++;
	}
}

void	ft_print_export(t_shell *data, t_info *curr, int height)
{
	char	**array;
	int		i;
	int		size;

	array = NULL;
	i = 0;
	size = ft_lstsize_all(data);
	if (data->env != NULL && height == 0 && curr->is_pipe == 0)
	{
		array = create_array_all(data, NULL); // добавить сортировку
		while (i < size)
		{
			write(1, "declare -x ", ft_strlen("declare -x "));
			write(1, array[i], ft_strlen(array[i]));
			write(1, "\n", 1);
			i++;
		}
		ft_array_clear(array);
	}
}

int	ft_exec_export(t_shell *data, t_info *curr)
{
	int		height;

	height = ft_height_array(curr->argv);
	if (curr->nb_cmd == 3 && curr->is_pipe == 0)
	{
		if (height > 0)
		{
			if (ft_check_key(curr->argv[1]) == 1)
				return (ft_err_export(data, curr->argv[1]));
			ft_add_variable(curr, data);
		}
		ft_print_export(data, curr, height);
	}
	return (0);
}
