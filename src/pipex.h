/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:11:02 by lferro            #+#    #+#             */
/*   Updated: 2024/01/15 15:36:25 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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

# define TRUE	0
# define FALSE	1

# define PL		printf("line %d, file %s\n", __LINE__, __FILE__);


typedef struct s_cmd
{
	char	*path;
	char	**args;
	char	**envp;
}			t_cmd;

typedef struct s_file
{
	char	*path;
	int		fd;
}			t_file;



#endif
