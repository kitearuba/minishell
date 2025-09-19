/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 13:50:23 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*construct_cmd_path(const char *dir, const char *cmd)
{
	char	*path;
	size_t	len;

	if (!dir || !cmd)
		return (NULL);
	len = ft_strlen(dir) + 1 + ft_strlen(cmd) + 1;
	path = (char *)malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

static char	*search_cmd_in_path(char **dirs, const char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (dirs && dirs[i])
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

char	*get_cmd_path(const char *cmd, char **envp)
{
	char	**dirs;
	char	*path_value;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (NULL);
	path_value = ft_getenv(envp, "PATH");
	if (!path_value || !*path_value)
		return (NULL);
	dirs = ft_split(path_value, ':');
	if (!dirs)
		return (NULL);
	return (search_cmd_in_path(dirs, cmd));
}
