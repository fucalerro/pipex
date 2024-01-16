/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/16 18:27:43 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	check_errors(int argc, char const *argv[])
{
	int	error;

	error = 0;
	if (argc != 5)
	{
		perror("4 arguments needed");
		error = ARG_NUM;
	}




	return (error);
}

int	main(int argc, char const *argv[], char *const *envp)
{
	t_cmd	cmd1;
	t_cmd	cmd2;
	pid_t	pid;
	int		file1_fd;
	int		fd[2];
	int		file2_fd;

	if (check_errors(argc, argv) != 0)
	{
		print_errors();
		freestuff();
		exit(0);
	}

	pipe(fd);

	parse_cmd(&cmd1, argv[2], envp);
	parse_cmd(&cmd2, argv[4], envp);

	file1_fd = open(argv[1], O_RDONLY);
	file2_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return (1);
	}
	else if (pid == 0) // child process
	{
		close(fd[0]);
		dup2(file1_fd, STDIN_FILENO);
		close(file1_fd);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(cmd1.path, cmd1.args, envp);
	}
	else // parent process
	{
		wait(0);
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(file2_fd, STDOUT_FILENO);
		close(file2_fd);
		execve(cmd2.path, cmd2.args, envp);
	}

	free(cmd1.path);
	free(cmd2.path);
	free(cmd1.args);
	free(cmd2.args);
	exit(1);
	return (0);
}
