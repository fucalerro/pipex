/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:11:02 by lferro            #+#    #+#             */
/*   Updated: 2024/02/11 14:02:03 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/*************************************************************
 *                        INCLUDES                           *
 *************************************************************/

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# include <errno.h>

/*************************************************************
 *                        DEFINES                            *
 *************************************************************/

# define TRUE			0
# define FALSE			1

# ifndef O_DIRECTORY
#  define O_DIRECTORY	__O_DIRECTORY
# endif

# define F_NOT_EXIST	-1
# define READ_DENIED	-2
# define WRITE_DENIED	-3
# define CREATE_DENIED	-4
# define CMD_NOT_FOUND	-1

// # define PL		printf("\nline %d, file %s\n\n", __LINE__, __FILE__);

/*************************************************************
 *                        STRUCTURES                         *
 *************************************************************/

typedef struct s_cmd
{
	char	*path;
	char	**args;
}			t_cmd;

typedef struct s_err
{
	int		in;
	int		out;
}			t_err;

typedef struct s_fds
{
	int		pipe[2];
	int		in;
	int		out;
}			t_fds;

typedef struct s_infos
{
	t_cmd	cmd_in;
	t_cmd	cmd_out;

	t_err	err_file;
	t_err	err_cmd;

	t_fds	fd;
}			t_infos;

/*************************************************************
 *                        FUNCTIONS                          *
 *************************************************************/

char	**get_paths(char *const **envp);
int		get_cmd_path(char **all_paths, char const ***cmd_args,
			char **cmd_path, int err_file);
int		parse_cmd(t_cmd *cmd, const char *args,
			char *const *envp, int err_file);
void	out_cmd_process(t_infos *info, char *const *envp);
void	in_cmd_process(t_infos *info, char *const *envp);
int		cmds_process(t_infos *info, char *const *envp);
void	parent_process(t_infos *info, pid_t pid);
void	open_files(char const *argv[], t_infos *info);
void	access_infile(const char *filename, t_infos *info);
void	access_outfile(const char *filename, t_infos *info);
void	cmds_parsing(t_infos *info, char const *argv[], char *const *envp);
void	close_fds(t_infos *info);
void	init_err(t_infos *info);
void	print_errors(char *before_msg, char *errmsg, char *after_msg);
// void	ft_perror(const char *s, ...);

#endif
