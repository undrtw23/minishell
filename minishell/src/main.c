/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/01 18:45:10 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Generate the current working directory via getcwd, print it in bold.
If getcwd fail, put root instead "/" so the program can continue.
Append "$ " at the end. */
static char	*generate_prompt(void)
{
	char	*cwd;
	char	*colorcwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		cwd = ft_strdup("/");
	if (cwd == NULL)
		return (NULL);
	colorcwd = ft_strjoin("\001\033[38;5;208m\002", cwd);
	if (colorcwd == NULL)
		return (free(cwd), NULL);
	prompt = ft_strjoin(colorcwd, "\001\033[0m\002$ ");
	if (prompt == NULL)
	{
		free_all(cwd, colorcwd, NULL, NULL);
		return (NULL);
	}
	free_all(cwd, colorcwd, NULL, NULL);
	return (prompt);
}

/* Normal case for Minishell :
use readline to get user input (readline handle arrow keys, history, tab...)
prompt is current working directory -> p->input_text.*/
static void	handle_interactive_input(t_cmd_set *p)
{
	char	*readline_input;
	char	*prompt;
	int		i;

	i = -1;
	readline_input = NULL;
	prompt = generate_prompt();
	if (prompt == NULL)
		free_exit(p, 1, "Prompt error");
	readline_input = readline(prompt);
	p->input_text = ft_split(readline_input, '\n');
	free(readline_input);
	free(prompt);
	if (p->input_text == NULL)
	{
		ft_putendl_fd("\033[1;31mexit\033[0m", 1);
		if (g_exit_status > 0)
			p->status_code = g_exit_status;
		free_exit(p, p->status_code, NULL);
	}
}

/* Test case for minishell, use get next line on stdin to get
a list of instruction from a file.
Example : "./minishell < test_script.sh"
Remove next line delimiter "\n" for processing. */
static void	handle_non_interactive_input(t_cmd_set *p)
{
	char	*line;
	char	*tmp;
	int		i;

	(void)tmp;
	line = NULL;
	i = -1;
	tmp = get_next_line(0);
	p->input_text = ft_split(tmp, '\n');
	//  print_tab(p->input_text);
	free(tmp);
	if (!p->input_text || !p->input_text[0])
		free_exit(p, p->status_code, NULL);
	while (p->input_text[++i] && *(p->input_text[i]))
	{
		line = p->input_text[i];
		p->input_text[i] = ft_strtrim(p->input_text[i], "\n");
		free(line);
	}
	//print_tab(p->input_text);
	if (p->input_text[0] == NULL)
		free_exit(p, g_exit_status, "exit\n");
}

/* Init & set_signal initialise everything we use.
isatty(0) = Is standard input connected to a terminal?
if yes, interactive mode, if not, non-interactive mode. */

int	main(int argc, char **argv, char **envp)
{
	t_cmd_set	p_cmd_set;

	init(&p_cmd_set, envp, argv, argc);
	while (true)
	{
		disable_ctrl_z();
		set_signals(&p_cmd_set);
		if (isatty(0))
			handle_interactive_input(&p_cmd_set);
		else
			handle_non_interactive_input(&p_cmd_set);
		//		print_tab(p_cmd_set.envp);
		if (!process_input(p_cmd_set.input_text, &p_cmd_set))
			break ;
		free_array(&p_cmd_set.input_text);
	}
	free_exit(&p_cmd_set, p_cmd_set.status_code, NULL);
}
