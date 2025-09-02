/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:31:00 by gkorzec           #+#    #+#             */
/*   Updated: 2024/07/30 12:51:08 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char	*ft_strjoin_gnl(const char *s1, const char *s2);
char	*ft_strchr_gnl(const char *s, int c);
char	*ft_strdup_gnl(const char *s);
char	*get_next_line(int fd);
size_t	ft_strlen_gnl(const char *s);

#endif
