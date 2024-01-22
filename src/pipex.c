/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:10:54 by lferro            #+#    #+#             */
/*   Updated: 2024/01/22 19:21:59 by lferro           ###   ########.fr       */
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
	*out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (access(argv[4], F_OK) == -1)
	{
		if (access(".", W_OK) == -1)
			printf("%s: %s\n", strerror(errno), argv[4]);
		err.out = CREATE_DENIED;
	}
	else if (access(argv[4], W_OK) == -1)
	{
		printf("%s: %s\n", strerror(errno), argv[4]);
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

void	init_err(t_err *err_file, t_err *err_cmd)
{
	err_file->in = 0;
	err_file->out = 0;
	err_cmd->in = 0;
	err_cmd->out = 0;
}



int	main(int argc, char const *argv[], char *const *envp)
{
	t_infos	info;
	pid_t	pid;

	if (argc != 5)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	is_outfile_directory(argv[4]);
	init_err(&info.err_cmd, &info.err_file);

	// cmds_parsing(argv, cmd, err, envp);

	info.err_file = open_files(argv, &info.fd.in, &info.fd.out);

	info.err_file = open_files(argv, &info.fd.in, &info.fd.out);
	if (info.err_file.in >= 0)
	{
		if (parse_cmd(&info.cmd_in, argv[2], envp) == FALSE)
		{
			info.err_cmd.in = CMD_NOT_FOUND;
			printf("command not found: %s", argv[2]);
		}
	}
	if (info.err_file.out >= 0)
	{
		if (parse_cmd(&info.cmd_out, argv[3], envp) == FALSE)
		{
			info.err_cmd.out = CMD_NOT_FOUND;
			printf("command not found: %s", argv[2]);
		}
	}

	pipe(info.fd.pipe);
	pid = fork();
	if (pid < 0)
	{
		perror("fork error");
		return (1);
	}
	else if (pid == 0) // child process
	{
		// if no file error and no cmd error, exec cmd1 with file1
		if (info.err_file.in >= 0 && info.err_cmd.in >= 0)
		{
			close(info.fd.pipe[0]);
			dup2(info.fd.in, STDIN_FILENO);
			close(info.fd.in);
			dup2(info.fd.pipe[1], STDOUT_FILENO);
			close(info.fd.pipe[1]);
			execve(info.cmd_in.path, info.cmd_in.args, envp);
		}
		else
		{
			close(info.fd.pipe[1]);
			close(info.fd.pipe[0]);
			close(info.fd.in);
			close(info.fd.out);
		}
	}
	else
	{
		pid = fork();

		if (pid < 0)
		{
			perror("fork2 error");
			return (1);
		}
		else if (pid == 0) // 2nd child process
		{
			if (info.err_file.out >= 0 && info.err_cmd.out >= 0)
			{
				wait(0);
				close(info.fd.pipe[1]);
				dup2(info.fd.pipe[0], STDIN_FILENO);
				close(info.fd.pipe[0]);
				dup2(info.fd.out, STDOUT_FILENO);
				close(info.fd.out);
				execve(info.cmd_out.path, info.cmd_out.args, envp);
			}
			else
			{
				close(info.fd.pipe[1]);
				close(info.fd.pipe[0]);
				close(info.fd.in);
				close(info.fd.out);
			}
		}
		else // parent process
		{
			wait(0);
			free(info.cmd_in.path);
			free(info.cmd_out.path);
			int	i;
			i = -1;
			while (info.cmd_in.args[++i])
				free(info.cmd_in.args[i]);
			i = -1;
			while (info.cmd_out.args[++i])
				free(info.cmd_out.args[i]);
			free(info.cmd_in.args);
			free(info.cmd_out.args);
			return (0);
		}
	}
}
