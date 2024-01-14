/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/14 18:06:07 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	check_errors(int argc, char const *argv[])
{
	if (argc != 5)
		perror("4 arguments needed");
	return (0);
}



/**
 * @brief Get the paths array of string from the global ENV variable.
 *
 * @param environ The global ENV variable.
 * @param paths The array of string to store the paths.
 * @return char**
 */
void	get_paths(char **environ, char ***paths)
{
	int	i;

	i = 0;
	while (ft_strncmp(environ[i], "PATH=", 5 != 0))
		i++;
	if (environ[i] == NULL)
	{
		*paths = NULL;
		perror("PATH not found");
	}
	*paths = ft_split(environ[i] + 5, ':');
}



/**
 * @brief Get the correct path for the command in paramater.
 *
 * @param paths The array of string containing all the paths.
 * @param argv The command to find.
 * @param cmd_path The final path of the command.
 * @return int
 */
int	get_cmd_path(char **paths, char const **argv, char **cmd_path)
{
	int		i;
	char	*p_slash;
	int		cmd_exist;

	i = 0;

	while (paths[i])
	{
		p_slash = ft_strjoin(paths[i], "/");
		*cmd_path = ft_strjoin(p_slash, *argv);
		free(p_slash);
		cmd_exist = access(*cmd_path, F_OK);
		if (cmd_exist == TRUE)
		{
			printf("access: %d\n", cmd_exist);
			break ;
		}
		else
		{
			cmd_exist = -1;
			free(*cmd_path);
		}
		i++;
	}

	printf("yo: %s\n", *cmd_path);

	if (cmd_exist != 0)
			perror("command not found!!");
	return (cmd_exist);
}

int	main(int argc, char const *argv[])
{
	char	**paths;
	char	*cmdpath;
	char	**envp;
	int		i;
	char	**cmdargs;
	i = 0;
	cmdpath = 0;
	get_paths(environ, &paths);

	cmdargs = ft_split(argv[2], ' ');
	get_cmd_path(paths, (char const **)&cmdargs[0], &cmdpath);
	int exec = execve(cmdpath, cmdargs, NULL);

	free(envp);

	i = 0;
	while (paths[i++])
		free(paths[i]);
	free(paths);

	return (0);
}
