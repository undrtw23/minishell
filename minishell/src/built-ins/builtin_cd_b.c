/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:36:07 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:38:16 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Checks errors for the cd argument when not a directory.
If the path does not exist, prints "No such file or directory".
If the path exists but is not a directory, prints "Not a directory".
If the path exists and is a directory but cannot be entered,
prints "Permission denied".*/
static void	cd_path_checks(char *path, t_cmd_set *p)
{
	struct stat	st;

	if (access(path, F_OK) == -1)
		put_err_cd("NoFile_NoDir", path, 1, p);
	else if (stat(path, &st) != -1 && !S_ISDIR(st.st_mode))
		put_err_cd("Not_Directory", path, 1, p);
	else if (access(path, X_OK) == -1)
		put_err_cd("Perm_Denied", path, 1, p);
}

/* Core logic for the "cd" builtin, handles all argument cases.
If HOME is unset and no argument or '~', prints error.
If no argument or '~', changes directory to HOME.
If argument is '-', changes to OLDPWD.
If argument exists but is not a directory, calls cd_path_checks.
If argument is a directory, changes to it.*/
static void	ft_chdir(char ***av, char *home, char *oldpwd, t_cmd_set *p)
{
	DIR	*dir;

	dir = NULL;
	if (av[0][1] && av[0][1][0] == '~' && ft_strlen(av[0][1]) == 1 && home
		&& *home)
	{
		free(av[0][1]);
		av[0][1] = ft_strdup(home);
		dir = opendir(av[0][1]);
	}
	else if (av[0][1])
		dir = opendir(av[0][1]);
	if ((!home || !home[0]) && ((!av[0][1] || av[0][1][0] == '~') && !dir))
		put_err_cd("HOME_Not_Set", NULL, 1, p);
	else if ((!oldpwd || !*oldpwd) && av[0][1] && av[0][1][0] == '-')
		put_err_cd("OLDPWD_Not_Set", NULL, 1, p);
	else if (!av[0][1] || av[0][1][0] == '~')
	{
		p->status_code = (chdir(home) == -1);
		if (chdir(home) == -1)
			put_err_cd("NoFile_NoDir", home, 1, p);
	}
	else if (av[0][1] && av[0][1][0] == '-' && oldpwd && *oldpwd)
	{
		p->status_code = (chdir(oldpwd) == -1);
		if (chdir(oldpwd) == -1)
			put_err_cd("NoFile_NoDir", oldpwd, 1, p);
	}
	else if (!dir)
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
static void	update_pwd_vars(t_cmd_set *p, char *prevpwd)
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
	if (cmd_args[1] && !cmd_args[1][1] && cmd_args[1][0] == '-'
		&& p->status_code == 0)
	{
		if (!*oldpwd)
			put_err("mini: cd: OLDPWD not set", NULL, 1, p);
		else
			ft_putendl_fd(oldpwd, 1);
	}
	update_pwd_vars(p, prevpwd);
	free_all(home, oldpwd, prevpwd, NULL);
	return (p->status_code);
}
