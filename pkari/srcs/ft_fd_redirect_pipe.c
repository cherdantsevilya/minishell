/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fd_redirect_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abridger <abridger@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 20:56:15 by abridger          #+#    #+#             */
/*   Updated: 2022/01/25 18:56:53 by abridger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_init_saved_fd(t_shell *data)
{
	data->save_in = dup(0);
	data->save_out = dup(1);
}

void	ft_close_saved_fd(t_shell *data)
{
	dup2(data->save_in, 0);
	dup2(data->save_out, 1);
	close(data->save_in);
	close(data->save_out);
}

void	ft_redirect_dup(t_info *curr)
{
	if (curr->fd_input_file != -2)
	{
		dup2(curr->fd_input_file, 0);
		close(curr->fd_input_file);
	}
	if (curr->fd_output_file != -2)
	{
		dup2(curr->fd_output_file, 1);
		close(curr->fd_output_file);
	}
}

void	ft_pipe_init(t_shell *data, t_info *curr)
{
	if (curr->token == TOKEN_PIPE && data->count == 1)
		pipe(data->fd_pipe);
	if (data->count >= 2)
	{
		dup2(data->fd_pipe[0], 0);
		close(data->fd_pipe[0]);
	}
	if (curr->token == TOKEN_PIPE)
	{
		dup2(data->fd_pipe[1], 1);
		close(data->fd_pipe[1]);
	}
}
