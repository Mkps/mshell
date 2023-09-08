/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:23:19 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/08 11:42:31 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	argc_error(int error_code)
{
	ft_putendl_fd("pipex: error: incorrect number of arguments.", 2);
	if (error_code == 0)
		ft_putendl_fd("Standard use is ./pipex infile cmd cmd outfile", 2);
	if (error_code == 1)
	{
		ft_putstr_fd("Use with here_doc is ", 2);
		ft_putendl_fd("./pipex here_doc LIMITER cmd cmd1 file", 2);
	}
	exit(1);
}

//Use perror to display the errno and exit with the specified code.
void	error_exit(int exit_code)
{
	perror("minishell: an error occured.");
	exit(exit_code);
}


int	open_fd(int mode, char *filename)
{
	int	fd;

	if (mode == 0)
		fd = open(filename, O_RDONLY, 0664);
	if (mode == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (mode == 2)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (fd);
	}
	return (fd);
}
