/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:50:06 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 19:50:56 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Checks errors for the cd argument when not a directory.
If the path does not exist, prints "No such file or directory".
If the path exists but is not a directory, prints "Not a directory".
If the path exists and is a directory but cannot be entered,
prints "Permission denied".*/
void	cd_path_checks(char *path, t_cmd_set *p)
{
	struct stat	st;

	if (access(path, F_OK) == -1)
		put_err_cd("NoFile_NoDir", path, 1, p);
	else if (stat(path, &st) != -1 && !S_ISDIR(st.st_mode))
		put_err_cd("Not_Directory", path, 1, p);
	else if (access(path, X_OK) == -1)
		put_err_cd("Perm_Denied", path, 1, p);
}

int	handle_special_cases(char ***av, char *home, char *oldpwd,
		t_cmd_set *p)
{
	if ((!home || !home[0]) && ((!av[0][1] || av[0][1][0] == '~')
			&& !opendir(av[0][1])))
		return (put_err_cd("HOME_Not_Set", NULL, 1, p), 1);
	if ((!oldpwd || !*oldpwd) && av[0][1] && av[0][1][0] == '-')
		return (put_err_cd("OLDPWD_Not_Set", NULL, 1, p), 1);
	if (!av[0][1] || av[0][1][0] == '~')
	{
		p->status_code = (chdir(home) == -1);
		if (chdir(home) == -1)
			put_err_cd("NoFile_NoDir", home, 1, p);
		return (1);
	}
	if (av[0][1] && av[0][1][0] == '-' && oldpwd && *oldpwd)
	{
		p->status_code = (chdir(oldpwd) == -1);
		if (chdir(oldpwd) == -1)
			put_err_cd("NoFile_NoDir", oldpwd, 1, p);
		return (1);
	}
	return (0);
}
