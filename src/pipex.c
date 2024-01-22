/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/22 14:43:44 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

void	print_errors(int errnum)
{
	perror("pipex");
}


int	check_errors(int argc, char const *argv[])
{
	int	error;

	error = 0;
	if (argc != 5)
	{
		perror("4 arguments needed");
		// error = ARG_NUM;
	}




	return (error);
}

int	check_file_permissions(char *filepath)
{

}

t_err	open_files(char const *argv[], int *in_fd, int *out_fd)
{
	t_err	err;

	err.in = 0;
	if (access(argv[1], F_OK) == -1)
	{
		printf("%s: %s\n", strerror(errno), argv[1]);
		err.in = F_NOT_EXIST;
	}
	else if (access(argv[1], R_OK) == -1)
	{
		printf("%s: %s\n", strerror(errno), argv[1]);
		err.in = READ_DENIED;
	}
	*in_fd = open(argv[1], O_RDONLY);
	*out_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (access(argv[3], F_OK) == -1)
	{
		if (access(".", W_OK) == -1)
			printf("%s: %s\n", strerror(errno), argv[3]);
		err.out = CREATE_DENIED;
	}
	else if (access(argv[3], W_OK) == -1)
	{
		printf("%s: %s\n", strerror(errno), argv[3]);
		err.out = WRITE_DENIED;
	}
	return (err);
}


// check if the outfile is a directory. In that case, exit the whole program
int	is_outfile_directory(char const *outfile)
{
	int	fd;

	fd = open(outfile, O_DIRECTORY);
	if (fd >= 0)
	{
		printf("is a directory: %s\n", outfile);
		close(fd);
		exit(0);
	}
	close(fd);

}


int	main(int argc, char const *argv[], char *const *envp)
{
	t_cmd	cmd_in;
	t_cmd	cmd_out;
	pid_t	in_pid;
	pid_t	out_pid;
	int		in_fd;
	int		out_fd;
	int		fd[2];
	t_err	f_err;
	t_err	cmd_err;

	is_outfile_directory(argv[3]);
	pipe(fd);

	// init file and cmd errors
	f_err.in = 0;
	f_err.out = 0;
	cmd_err.in = 0;
	cmd_err.out = 0;

	// parse commands if no file errors
	f_err = open_files(argv, &in_fd, &out_fd);
	if (f_err.in >= 0)
	{
		if (parse_cmd(&cmd_in, argv[2], envp) == FALSE)
		{
			cmd_err.in = CMD_NOT_FOUND;
			printf("command not found: %s", argv[2]);
		}
	}
	if (f_err.out >= 0)
	{
		if (parse_cmd(&cmd_out, argv[4], envp) == FALSE)
		{
			cmd_err.out = CMD_NOT_FOUND;
			printf("command not found: %s", argv[2]);
		}
	}

	in_pid = fork();
	if (in_pid < 0)
	{
		perror("fork error");
		return (1);
	}
	else if (in_pid == 0) // child process
	{
		// if no file error and no cmd error, exec cmd1 with file1
		if (f_err.in >= 0 && cmd_err.in >= 0)
		{
			close(fd[0]);
			dup2(in_fd, STDIN_FILENO);
			close(in_fd);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			execve(cmd_in.path, cmd_in.args, envp);
		}
	}
	else
	{
		out_pid = fork();

		if (out_pid < 0)
		{
			perror("fork2 error");
			return (1);
		}
		else if (out_pid == 0) // 2nd child process
		{
			if (f_err.out >= 0 && cmd_err.out >= 0)
			{
				wait(0);
				close(fd[1]);
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]);
				dup2(out_fd, STDOUT_FILENO);
				close(out_fd);
				execve(cmd_out.path, cmd_out.args, envp);
			}
		}
		else //  parent process
		{
			free(cmd_in.path);
			free(cmd_out.path);
			free(cmd_in.args);
			free(cmd_out.args);
			return (0);
		}
	}
}
