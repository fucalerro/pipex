/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:45:23 by lferro            #+#    #+#             */
/*   Updated: 2024/01/23 18:01:46 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


void	out_cmd_process(t_infos *info, char *const *envp)
{
	if (info->err_file.out >= 0 || info->err_cmd.out >= 0)
	{
		// int	status;

		// waitpid(pid, )
		close(info->fd.pipe[1]);
		dup2(info->fd.pipe[0], STDIN_FILENO);
		close(info->fd.pipe[0]);
		dup2(info->fd.out, STDOUT_FILENO);
		close(info->fd.out);
		execve(info->cmd_out.path, info->cmd_out.args, envp);
	}
	else
	{
		close_fds(&info->fd);
		PL;
		exit(1);
	}
}

void	in_cmd_process(t_infos *info, char *const *envp)
{

	printf("info->err_file.in: %d\n", info->err_file.in);
	printf("info->err_cmd.in: %d\n", info->err_cmd.in);

	if (info->err_file.in >= 0 || info->err_cmd.in >= 0)
	{
		close(info->fd.pipe[0]);
		dup2(info->fd.in, STDIN_FILENO);
		close(info->fd.in);
		dup2(info->fd.pipe[1], STDOUT_FILENO);
		close(info->fd.pipe[1]);
		execve(info->cmd_in.path, info->cmd_in.args, envp);
	}
	else
	{
		PL;
		close_fds(&info->fd);
		exit(1);
	}
}

int	cmds_process(t_infos *info, char *const *envp)
{
	pid_t	pid;

	pid = fork();
	printf("pid: %d\n", pid);
	if (pid < 0)
		perror("fork error");
	if (pid < 0)
		return (1);
	else if (pid == 0)
		in_cmd_process(info, envp);
	else
	{
		// pid = fork();
		// if (pid < 0)
		// 	perror("fork2 error");
		// if (pid < 0)
		// 	return (1);
		// else if (pid == 0)
			out_cmd_process(info, envp);
		// else
		// {
		// 	printf("pid: %d\n", pid);
			PL;

			// int	status;
			// waitpid(pid, &status, 0);

			parent_process(info, pid);
			return (0);
		// }
	}
	return (0);
}

void	parent_process(t_infos *info, pid_t pid)
{
	free(info->cmd_in.path);
	free(info->cmd_out.path);
	int	i;
	i = -1;
	if (info->err_file.in == 0)
	{
		while (info->cmd_in.args[++i])
			free(info->cmd_in.args[i]);
		free(info->cmd_in.args);
	}
	i = -1;
	while (info->cmd_out.args[++i])
		free(info->cmd_out.args[i]);
	free(info->cmd_out.args);
}
