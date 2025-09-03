/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:36:07 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 19:50:41 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_chdir(char ***av, char *home, char *oldpwd, t_cmd_set *p)
{
	DIR	*dir;

	dir = NULL;
	if (av[0][1] && av[0][1][0] == '~' && ft_strlen(av[0][1]) == 1 && home
		&& *home)
	{
		free(av[0][1]);
		av[0][1] = ft_strdup(home);
	}
	if (av[0][1])
		dir = opendir(av[0][1]);
	if (handle_special_cases(av, home, oldpwd, p))
		return ;
	if (!dir)
		cd_path_checks(av[0][1], p);
	else
		chdir(av[0][1]);
	if (dir)
		closedir(dir);
}

/* Done for setting env variable PWD and OLDPWD if they dont
already exist.*/
void	check_home_pwd_oldpwd(t_cmd_set *p)
{
	char	*pwd;
	char	*oldpwd;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	pwd = ft_getenv("PWD", p->envp);
	oldpwd = ft_getenv("OLDPWD", p->envp);
	free_all(pwd, oldpwd, cwd, NULL);
}

/* Update variables of OLDPWD/PWD.*/
void	update_pwd_vars(t_cmd_set *p, char *prevpwd)
{
	char	*curpwd;

	p->envp = ft_setenv("OLDPWD", prevpwd, p->envp);
	curpwd = getcwd(NULL, 0);
	if (!curpwd)
		curpwd = ft_strdup("");
	p->envp = ft_setenv("PWD", curpwd, p->envp);
	free_all(curpwd, NULL, NULL, NULL);
}

/* Check for numbers of arguments, if arg[2] exit : fail
Set HOME/PWD/OLDPWD/prevpwd for later usage.
ft_chdir : main logic of cd.
"cd -" printage is handled here !*/
void	handle_oldpwd_output(char **cmd_args, char *oldpwd, t_cmd_set *p)
{
	if (cmd_args[1] && !cmd_args[1][1] && cmd_args[1][0] == '-'
		&& p->status_code == 0)
	{
		if (!*oldpwd)
			put_err("mini: cd: OLDPWD not set", NULL, 1, p);
		else
			ft_putendl_fd(oldpwd, 1);
	}
}

int	builtin_cd_b(t_cmd_set *p, char **cmd_args)
{
	char	*home;
	char	*oldpwd;
	char	*prevpwd;

	p->status_code = 0;
	if (cmd_args[1] && cmd_args[2])
		return (put_err(NULL, "cd: too many arguments", 1, p), 1);
	check_home_pwd_oldpwd(p);
	home = ft_getenv("HOME", p->envp);
	oldpwd = ft_getenv("OLDPWD", p->envp);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	prevpwd = getcwd(NULL, 0);
	if (!prevpwd)
		prevpwd = ft_strdup("");
	ft_chdir(&cmd_args, home, oldpwd, p);
	handle_oldpwd_output(cmd_args, oldpwd, p);
	update_pwd_vars(p, prevpwd);
	free_all(home, oldpwd, prevpwd, NULL);
	return (p->status_code);
}
