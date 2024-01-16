/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/16 14:00:46 by lferro           ###   ########.fr       */
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


int	main(int argc, char const *argv[], char *const *envp)
{
	int		exec;
	// char	*file1;
	t_cmd	cmd1;
	pid_t	pid1;
	int		file1_fd;
	int	dup2err;
	int		fd[2];

	if (pipe(fd) < 0)
		perror("pipe error\n");

	parse_cmd(&cmd1, argv[2], envp);

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork error");
		return (1);
	}
	else if (pid1 == 0) // child process
	{
		file1_fd = open(argv[1], O_RDONLY);
		dup2err = dup2(file1_fd, STDIN_FILENO);
		if (dup2err < 0)
		{
			perror("dup2 error");
			close(file1_fd);
		}
		close(file1_fd);

		exec = execve(cmd1.path, cmd1.args, envp);
	}
	else // parent process
	{

		wait(0);
	}

	// printf("fork pid: %d\n", pid);

	free(cmd1.path);
	return (0);
}
