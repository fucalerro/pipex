/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/23 18:30:16 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Check if the outfile is a directory. In that case,
 * exit the whole program
 *
 * @param outfile the path to the outfile
 */
void	is_outfile_valid(char const *outfile, t_err *err_file)
{
	int	fd;

	fd = open(outfile, O_DIRECTORY);
	if (fd >= 0)
	{
		printf("is a directory: %s\n", outfile);
		close(fd);
		exit(0);
	}
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (access_outfile(outfile, err_file) == -1);
	{
		printf("permission denied: %s\n", outfile);
		close(fd);
		exit(0);
	}
	close(fd);
}

/**
 * @brief init all errors status to 0
 *
 * @param err_file
 * @param err_cmd
 */
void	init_err(t_err *err_file, t_err *err_cmd)
{
	err_file->in = 0;
	err_file->out = 0;
	err_cmd->in = 0;
	err_cmd->out = 0;
}

/**
 * @brief close all fds
 *
 * @param fd struct containing all fds
 */
void	close_fds(t_fds *fd)
{
	close(fd->pipe[1]);
	close(fd->pipe[0]);
	close(fd->in);
	close(fd->out);
}

int	main(int argc, char const *argv[], char *const *envp)
{
	t_infos	info;

	if (argc != 5)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	is_outfile_valid(argv[4], &info.err_file);
	init_err(&info.err_cmd, &info.err_file);
	cmds_parsing(&info, argv, envp);
	pipe(info.fd.pipe);
	return (cmds_process(&info, envp));
}

