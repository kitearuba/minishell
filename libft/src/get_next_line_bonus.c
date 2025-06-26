/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:06:06 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/25 19:01:59 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static char	*free_storage(char **storage)
{
	if (*storage)
	{
		free(*storage);
		*storage = NULL;
	}
	return (NULL);
}

static char	*extract_line(char **storage)
{
	char	*newline_pos;
	char	*line;
	char	*temp_storage;
	size_t	len;

	if (!*storage)
		return (NULL);
	newline_pos = ft_strchr(*storage, '\n');
	if (newline_pos)
	{
		len = newline_pos - *storage + 1;
		line = (char *)malloc((len + 1) * sizeof(char));
		if (!line)
			return (free_storage(storage));
		ft_strappend(line, *storage, NULL, len + 1);
		temp_storage = ft_strdup(newline_pos + 1);
		free(*storage);
		*storage = temp_storage;
		if (*storage && **storage == '\0')
			free_storage(storage);
		return (line);
	}
	line = ft_strdup(*storage);
	free_storage(storage);
	return (line);
}

static ssize_t	read_fd(int fd, char **storage)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (*storage)
			temp = ft_strjoin(*storage, buffer);
		else
			temp = ft_strdup(buffer);
		free(*storage);
		*storage = temp;
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (bytes_read);
}

char	*get_next_line_bonus(int fd)
{
	static char	*storage[1024];
	ssize_t		bytes_read;

	if (fd < 0 || fd > 1023 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes_read = read_fd(fd, &storage[fd]);
	if (bytes_read < 0)
		return (free_storage(&storage[fd]));
	if (bytes_read == 0 && !storage[fd])
		return (NULL);
	return (extract_line(&storage[fd]));
}
