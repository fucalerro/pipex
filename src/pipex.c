/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/25 18:18:59 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief init all errors status to 0
 *
 * @param err_file
 * @param err_cmd
 */
void	init_err(t_infos *info)
{
	info->err_file.in = 0;
	info->err_file.out = 0;
	info->err_cmd.in = 0;
	info->err_cmd.out = 0;
}

/**
 * @brief close all fds
 *
 * @param fd struct containing all fds
 */
void	close_fds(t_infos *info)
{
	close(info->fd.pipe[1]);
	close(info->fd.pipe[0]);
	if (info->err_file.in == 0)
		close(info->fd.in);
	close(info->fd.out);
}

int	main(int argc, char const *argv[], char *const *envp)
{
	t_infos	info;

	if (argc != 5)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	cmds_parsing(&info, argv, envp);
	pipe(info.fd.pipe);
	return (cmds_process(&info, envp));
}
