/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:11:02 by lferro            #+#    #+#             */
/*   Updated: 2024/01/22 18:26:44 by lferro           ###   ########.fr       */
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

# define PL		printf("line %d, file %s\n", __LINE__, __FILE__);

/*************************************************************
 *                        STRUCTURES                         *
 *************************************************************/

typedef struct s_cmd
{
	char	*path;
	char	**args;
}			t_cmd;

typedef struct s_cmds
{
	t_cmd	in;
	t_cmd	out;
}			t_cmds;

typedef struct s_err
{
	int		in;
	int		out;
}			t_err;

typedef struct s_errs
{
	t_err	file;
	t_err	cmd;
}			t_errs;

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
char	*get_file_string(char *filepath);

int		has_create_permission(void);
int		has_write_permission(char *filepath);
int		has_read_permission(char *filepath);

#endif
