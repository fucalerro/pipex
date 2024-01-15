/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/15 17:30:55 by lferro           ###   ########.fr       */
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
int	get_cmd_path(char **all_paths, char const **cmd_args, char **cmd_path)
{
	int		i;
	char	*p_slash;
	int		cmd_exist;

	i = -1;
	while (all_paths[++i])
	{
		p_slash = ft_strjoin(all_paths[i], "/");
		*cmd_path = ft_strjoin(p_slash, *cmd_args);
		free(p_slash);
		cmd_exist = access(*cmd_path, F_OK);
		if (cmd_exist == TRUE)
			break ;
		else
		{
			cmd_exist = -1;
			free(*cmd_path);
		}
	}
	if (cmd_exist)
		perror("command not found!!");
	return (cmd_exist);
}

/**
 * @brief Create a string with the content of the file in filepath.
 *
 * @param filepath The path of the file to read.
 * @return char*
 */
char	*get_file_string(char *filepath)
{
	int		fd;
	char	*line;
	char	*new_buffer;
	char	*buffer;

	fd = open(filepath, O_RDWR);
	buffer = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		new_buffer = ft_strjoin_safe(buffer, line);
		free(buffer);
		buffer = 0;
		buffer = new_buffer;
		free(line);
	}
	close(fd);
	return (new_buffer);
}

int	parse_cmd(t_cmd *cmd, const char *args, char *const *envp)
{
	char	**paths;
	int		i;

	cmd->path = 0;
	paths = get_paths(&envp);

	// printf("paths: %s\n", paths[0]);

	cmd->args = ft_split(args, ' ');
	// printf("cmdargs: %s\n", cmd->args[0]);
	get_cmd_path(paths, (char const **)cmd->args, &cmd->path);
	i = -1;
	while (paths[++i])
	{
		printf("paths: %s\n", paths[i]);
		free(paths[i]);
	}
	free(paths);
	return (0);
}

int	main(int argc, char const *argv[], char *const *envp)
{
	int		exec;
	char	*file1;
	t_cmd	cmd1;

	file1 = get_file_string((char *)argv[1]);
	parse_cmd(&cmd1, argv[2], envp);
	exec = execve(cmd1.path, cmd1.args, envp);

	free(cmd1.path);

	return (0);
}
