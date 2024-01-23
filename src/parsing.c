/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:46:16 by lferro            #+#    #+#             */
/*   Updated: 2024/01/23 17:51:15 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Parse the command and store it in a struct.
 *
 * @param info
 * @param argv
 * @param envp
 * @return int
 */
void	cmds_parsing(t_infos *info, char const *argv[], char *const *envp)
{
	info->err_file = open_files(argv, &info->fd.in, &info->fd.out);
	if (info->err_file.in >= 0)
	{
		if (parse_cmd(&info->cmd_in, argv[2], envp) == FALSE)
		{
			info->err_cmd.in = CMD_NOT_FOUND;
			printf("!!info->err_cmd.in: %d\n", info->err_cmd.in);
			printf("command not found: %s", argv[2]);
		}
	}
	if (info->err_file.out >= 0)
	{
		if (parse_cmd(&info->cmd_out, argv[3], envp) == FALSE)
		{
			info->err_cmd.out = CMD_NOT_FOUND;
			printf("command not found: %s", argv[2]);
		}
	}
}

void	access_infile(const char *filename, t_err *err)
{
	err->in = 0;
	if (access(filename, F_OK) == -1)
	{
		printf("fileok: %s: %s\n", strerror(errno), filename);
		err->in = F_NOT_EXIST;
	}
	else if (access(filename, R_OK) == -1)
	{
		printf("readok: %s: %s\n", strerror(errno), filename);
		err->in = READ_DENIED;
	}
}

/**
 * @brief check if the files exist, if they are readable and writable
 * and open them.
 * If the outfile doesn't exist, create it.
 * If the outfile is a directory, exit the program.
 *
 * @param argv
 * @param in_fd
 * @param out_fd
 * @return t_err
 */
t_err	open_files(char const *argv[], int *in_fd, int *out_fd)
{
	t_err	err;

	access_infile(argv[1], &err);
	*in_fd = open(argv[1], O_RDONLY);
	*out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (access(argv[4], F_OK) == -1)
	{
		if (access(".", W_OK) == -1)
			printf("%s: %s\n", strerror(errno), argv[4]);
		err.out = CREATE_DENIED;
	}
	else if (access(argv[4], W_OK) == -1)
	{
		printf("%s: %s\n", strerror(errno), argv[4]);
		err.out = WRITE_DENIED;
	}
	return (err);
}
