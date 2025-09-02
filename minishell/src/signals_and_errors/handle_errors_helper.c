/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:06:41 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/28 16:25:06 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Helpers for "line too long." norminette"*/
void	error_unclosed_quotes(void)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "unexpected EOF while looking for matching `'\"\n");
}

void	error_unexpected_token(char *cmd)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "syntax error near unexpected token `%s'\n", cmd);
}

void	error_token_newline(void)
{
	ft_printf_fd(2, "mini: ");
	ft_printf_fd(2, "syntax error near unexpected token `newline'\n");
}

void	error_delim_heredoc(char *lim)
{
	ft_printf_fd(2, "mini: warning: here-document delimited by end-of-file ");
	ft_printf_fd(2, "(wanted `%s\')\n", lim);
}
