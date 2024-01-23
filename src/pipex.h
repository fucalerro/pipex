/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:11:02 by lferro            #+#    #+#             */
/*   Updated: 2024/01/23 15:22:47 by lferro           ###   ########.fr       */
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

# define F_NOT_EXIST	-1
# define READ_DENIED	-2
# define WRITE_DENIED	-3
# define CREATE_DENIED	-4
# define CMD_NOT_FOUND	-1

# define PL		printf("\nline %d, file %s\n\n", __LINE__, __FILE__);

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
int		get_cmd_path(char **all_paths, char const ***cmd_args, char **cmd_path);
int		parse_cmd(t_cmd *cmd, const char *args, char *const *envp);
void	out_cmd_process(t_infos *info, char *const *envp);
void	in_cmd_process(t_infos *info, char *const *envp);
int		cmds_process(t_infos *info, char *const *envp);
void	parent_process(t_infos *info, pid_t pid);
t_err	open_files(char const *argv[], int *in_fd, int *out_fd);
void	access_infile(const char *filename, t_err *err);
void	cmds_parsing(t_infos *info, char const *argv[], char *const *envp);
void	close_fds(t_fds *fd);

#endif
