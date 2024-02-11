/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:46:16 by lferro            #+#    #+#             */
/*   Updated: 2024/02/11 11:16:06 by lferro           ###   ########.fr       */
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
	open_files(argv, info);
	if (parse_cmd(&info->cmd_in, argv[2], envp, info->err_file.in) == FALSE)
		info->err_cmd.in = CMD_NOT_FOUND;
	if (parse_cmd(&info->cmd_out, argv[3], envp, info->err_file.out) == FALSE)
		info->err_cmd.out = CMD_NOT_FOUND;
}

void	access_infile(const char *filename, t_infos *info)
{
	info->err_file.in = 0;
	if (access(filename, F_OK) == -1)
	{
		print_errors(strerror(errno), ": ", (char *)filename);
		info->err_file.in = F_NOT_EXIST;
	}
	else if (access(filename, R_OK) == -1)
	{
		print_errors(strerror(errno), ": ", (char *)filename);
		info->err_file.in = READ_DENIED;
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
void	open_files(char const *argv[], t_infos *info)
{
	init_err(info);
	access_infile(argv[1], info);
	access_outfile(argv[4], info);
	if (info->err_file.in == 0)
		info->fd.in = open(argv[1], O_RDONLY);
	if (info->err_file.out == 0)
		info->fd.out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

void	access_outfile(const char *filename, t_infos *info)
{
	if (access(filename, F_OK) == -1)
	{
		if (access(".", W_OK) == -1)
		{
			print_errors(strerror(errno), ": ", (char *)filename);
			info->err_file.out = CREATE_DENIED;
		}
	}
	else if (access(filename, W_OK) == -1)
	{
		print_errors(strerror(errno), ": ", (char *)filename);
		info->err_file.out = WRITE_DENIED;
	}
}
