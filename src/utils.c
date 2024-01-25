/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:51:52 by lferro            #+#    #+#             */
/*   Updated: 2024/01/25 20:38:45 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// void	ft_perror(const char *s, ...)
// {
// 	int		og_stdout;
// 	va_list	args;	
// 	va_start(args, s);
// 	og_stdout = dup(1);
// 	dup2(2, 1);
// 	ft_printf(s, args);
// 	dup2(og_stdout, 1);
// }

void	print_errors(char *before_msg, char *errmsg, char *after_msg)
{
	int	og_stdout;

	og_stdout = dup(1);
	dup2(2, 1);
	ft_printf("%s%s%s\n", before_msg, errmsg, after_msg);
	dup2(og_stdout, 1);
}

/**
 * @brief Get the paths array of string from the global ENV variable.
 * The array contains all possible path of system commands.
 * @param environ The global ENV variable.
 * @param paths The array of string to store the paths.
 * @return char**
 */
char	**get_paths(char *const **envp)
{
	char	**paths;
	int		i;

	i = 0;
	while (ft_strncmp((*envp)[i], "PATH=", 5 != 0))
		i++;
	if ((*envp)[i] == NULL)
	{
		paths = NULL;
		perror("PATH not found");
	}
	paths = ft_split((*envp)[i] + 5, ':');
	return (paths);
}

/**
 * @brief Get the correct path for the command in paramater.
 * The final path + cmd name is saved to cmd_path.
 * @param paths The array of string containing all the paths.
 * @param argv The command to find.
 * @param cmd_path The final path of the command.
 * @return int
 */
int	get_cmd_path(char **paths, char const ***args, char **path, int err_file)
{
	int		i;
	char	*p_slash;
	int		cmd_exist;

	cmd_exist = FALSE;
	i = 0;
	while (paths[++i])
	{
		p_slash = ft_strjoin_safe(paths[i], "/");
		*path = ft_strjoin_safe(p_slash, **args);
		free(p_slash);
		cmd_exist = access(*path, F_OK);
		if (cmd_exist == TRUE)
			break ;
		else
		{
			cmd_exist = FALSE;
			free(*path);
			*path = 0;
		}
	}
	if (cmd_exist != TRUE && err_file == 0)
		print_errors("Command not found: ", (char *)(*args)[0], "");
	return (cmd_exist);
}

void	single_quote_string(char ***args)
{
	int	j;
	int	i;

	i = -1;
	j = -1;
	while ((*args)[++i])
	{
		if ((*args)[i][0] == '\'')
		{
			while ((*args)[i][++j])
				(*args)[i][j] = (*args)[i][j + 1];
			(*args)[i][j - 2] = 0;
		}
		j = -1;
	}
}

/**
 * @brief Parse the command in args and store the result in cmd struct.
 *
 * @param cmd
 * @param args
 * @param envp
 * @return int
 */
int	parse_cmd(t_cmd *cmd, const char *args, char *const *envp, int err_file)
{
	char	**paths;
	int		i;
	int		cmd_exist;

	cmd->path = 0;
	paths = get_paths(&envp);
	cmd->args = ft_split(args, ' ');
	cmd_exist = get_cmd_path(paths, (char const ***)&cmd->args,
			&cmd->path, err_file);
	i = -1;
	single_quote_string(&cmd->args);
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (cmd_exist);
}
