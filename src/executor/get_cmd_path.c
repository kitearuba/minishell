/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 07:10:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*construct_cmd_path(char *dir, char *cmd)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

static char	*search_cmd_in_path(char **dirs, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (dirs[i])
	{
		path = construct_cmd_path(dirs[i], cmd);
		if (path && access(path, X_OK) == 0)
		{
			free_2d_array(dirs);
			return (path);
		}
		free(path);
		i++;
	}
	free_2d_array(dirs);
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*path_value;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_value = ft_getenv(envp, "PATH");
	if (!path_value)
		return (NULL);
	dirs = ft_split(path_value, ':');
	if (!dirs)
		return (NULL);
	return (search_cmd_in_path(dirs, cmd));
}
