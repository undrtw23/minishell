/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:50:59 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 15:52:55 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FD_H
# define FT_PRINTF_FD_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdint.h>
# include <stdarg.h>

int		ft_printf_fd(int fd, const char *format, ...);
int		ft_printchar_fd(int fd, int c);
int		ft_printstr_fd(int fd, char *s);
int		ft_printhex_fd(int fd, unsigned int n, char minmaj);
int		ft_printnbr_fd(int fd, int n);
int		ft_printunsi_fd(int fd, unsigned int u);
int		ft_printptr_fd(int fd, unsigned long ptr);

#endif
