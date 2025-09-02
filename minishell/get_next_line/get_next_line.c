/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:28:27 by gkorzec           #+#    #+#             */
/*   Updated: 2025/08/25 23:20:51 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_leftovers(char *buffer)
{
	char	*remains;
	size_t	i;

	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\0' || buffer[1] == '\0')
		return (NULL);
	remains = ft_substr_gnl(buffer, i + 1, ft_strlen_gnl(buffer) - i);
	if (remains == NULL)
		return (NULL);
	if (*remains == 0)
	{
		free(remains);
		remains = NULL;
	}
	buffer[i + 1] = '\0';
	return (remains);
}

static char	*fill_buffer(int fd, char *remains, char *buffer)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remains);
			return (NULL);
		}
		else if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (remains == NULL)
			remains = ft_strdup_gnl("");
		temp = remains;
		remains = ft_strjoin_gnl(temp, buffer);
		free(temp);
		if (ft_strchr_gnl(buffer, '\n') != NULL)
			break ;
	}
	return (remains);
}

char	*get_next_line(int fd)
{
	static char	*remains;
	char		*line;
	char		*buffer;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(remains);
		free(buffer);
		remains = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (buffer == NULL)
		return (NULL);
	line = fill_buffer(fd, remains, buffer);
	free(buffer);
	buffer = NULL;
	if (line == NULL)
		return (NULL);
	remains = get_leftovers(line);
	return (line);
}
