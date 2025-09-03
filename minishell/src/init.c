/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:08:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:33:32 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* If minishell is launched without environement variables, set basic one
for minimal execution of the minishell.*/
static int	allocate_env_vars_if_envp_empty(t_cmd_set *p)
{
	size_t	i;
	char	*default_env[6];

	default_env[0] = "PATH=/usr/bin:/bin";
	default_env[1] = "HOME=/";
	default_env[2] = "PWD=/";
	default_env[3] = "OLDPWD=/";
	default_env[4] = "SHLVL=1";
	default_env[5] = "_=./minishell";
	i = 0;
	while (i < 6)
	{
		p->envp[i] = ft_strdup(default_env[i]);
		if (p->envp[i] == NULL)
			return (0);
		i++;
	}
	return (1);
}

/* Global initialisation of minishell.
Initalise a t_cmd_set and the global exit status variable.
If env exist and not empty, cuplicate it in envp, if not
use helper function for basic env variable.*/
void	init_p_struct(t_cmd_set *p)
{
	p->tokens = NULL;
	p->token_count = 0;
	p->nodes = NULL;
	p->envp = NULL;
	p->input_text = NULL;
	g_exit_status = 0;
	p->status_code = 0;
	p->pid_of_lst_cmd = 0;
	p->pipe_flag = 0;
	p->pid_index = 0;
	p->tmp_dir = NULL;
	p->filename = NULL;
	p->abort = 0;
	p->shlvl = 0;
}

static void	init_reset(t_cmd_set *p, char **envp)
{
	init_p_struct(p);
	init_pid_arr(p);
	init_heredoc_arr(p);
	if (envp && *envp)
		p->envp = ft_dup_array(envp);
	if (!p->envp || !(envp && *envp))
	{
		p->envp = (char **)ft_calloc(6, sizeof(char *));
		if (p->envp != NULL)
		{
			if (!allocate_env_vars_if_envp_empty(p))
			{
				ft_putendl_fd("Error: Failed to duplicate env variables", 2);
				free_exit(p, 1, NULL);
			}
		}
		else
		{
			ft_putendl_fd("Error: Failed to allocate memory for p->envp", 2);
			free_exit(p, 1, NULL);
		}
	}
}

/* Main minishell initializer :
Security if init_rest failed get a minimal path
get a correct cwd : $PWD.
set $SHLV : reset it to 1 if not a valid value
otherwise increment it by 1.*/
void	init(t_cmd_set *p, char **envp, char **argv, int argc)
{
	char	*path;
	char	*cwd;
	char	*shlvl;
	char	*num;

	if (argc > 1 || argv[1])
		ft_putendl_fd("Arguments have been ignored", 2);
	init_reset(p, envp);
	path = ft_getenv("PATH", p->envp);
	if (path == NULL)
		p->envp = ft_setenv("PATH", "/usr/bin:/bin", p->envp);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup("/");
	p->envp = ft_setenv("PWD", cwd, p->envp);
	shlvl = ft_getenv("SHLVL", p->envp);
	if (!shlvl || ft_atoi(shlvl) <= 0 || ft_atoi(shlvl) == 2147483647)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(shlvl) + 1);
	if (num == NULL)
		num = ft_strdup("");
	p->envp = ft_setenv("SHLVL", num, p->envp);
	free_all(num, cwd, shlvl, path);
}
