/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:33:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 20:51:02 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* type of variable that is safe to modify inside signa_handlers
volatile tells the compiler that this can't be changed asynchonously*/
volatile sig_atomic_t	g_exit_status;

/* Instalation of signals :
signal(SIGINT, signals_parent) : Ctrl + c
Ctrl + \ is ignored in the main processus
Only work on blocking command : cat, sleep 100, grep....
No negative status. if g_exit status is set,
value cope to p->status_code and reset*/

void	set_signals(t_cmd_set *p)
{
	signal(SIGINT, signals_parent);
	signal(SIGQUIT, SIG_IGN);
	if (p->status_code < 0)
		p->status_code = 0;
	if (g_exit_status >= 0)
	{
		p->status_code = g_exit_status;
		g_exit_status = -1;
	}
}

/* Handle Ctrl + c in signal parent.
Print newline
set status to 130
tell readline that a new line is starting
Clear the input via rl_replace_line("")
redisplay re_wirte the prompt*/
void	signals_parent(int signal_code)
{
	if (signal_code == SIGINT)
	{
		write(1, "\n", 1);
		g_exit_status = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*Begining is the same that for parent process
(CRTL + C clear and exit line)|
Handle ctrl + \ in child process :
print Core Dumped and clearing/new line.*/
void	signals_child(int signal_code)
{
	if (signal_code == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_exit_status = 130;
	}
	else if (signal_code == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		rl_replace_line("", 0);
		g_exit_status = 131;
		rl_on_new_line();
	}
}

/* Force an heredoc opened to close without having the delimiter EOF
ioctl fakes a newline to readline so it returns (simulate press enter).
Else it would block.*/
void	signals_heredoc(int signal_code)
{

	if (signal_code == SIGINT)
	{
		g_exit_status = -130;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();

	}
}

int	rl_heredoc_hook(void)
{
	if (g_exit_status == -130)
	{
		rl_done = 1;
		g_exit_status = 130;
		return 1;
	}
	return 0;
}

/* Ctrl + z can suspend the program*/
void	disable_ctrl_z(void)
{
	signal(SIGTSTP, SIG_IGN);
}
