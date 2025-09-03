/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_b.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:32:31 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/29 01:48:15 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Used to print env variable that are not empty (meaning is there is a = sign
it prints it.)
Loop through the array until last string is null.*/
int	builtin_env_b(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
	{
		perror("envp");
		return (1);
	}
	while (envp[i] != NULL)
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
