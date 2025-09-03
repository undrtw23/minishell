/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:48:28 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Generate the current working directory via getcwd, print it in bold.
If getcwd fail, put root instead "/" so the program can continue.
Append "$ " at the end. */
char	*generate_prompt(void)
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
void	handle_interactive_input(t_cmd_set *p)
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

char	*read_full_input(void)
{
	char	*line;
	char	*tmp;
	char	*full_input;

	full_input = NULL;
	line = get_next_line(0);
	while (line != NULL)
	{
		tmp = full_input;
		if (tmp)
			full_input = ft_strjoin(tmp, line);
		else
			full_input = ft_strdup(line);
		free(tmp);
		free(line);
		line = get_next_line(0);
	}
	return (full_input);
}

void	handle_non_interactive_input(t_cmd_set *p)
{
	char	*full_input;

	full_input = read_full_input();
	if (full_input)
	{
		p->input_text = ft_split(full_input, '\n');
		free(full_input);
	}
	else
		p->input_text = NULL;
	if (!p->input_text || !p->input_text[0])
		free_exit(p, p->status_code, NULL);
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
		if (!process_input(p_cmd_set.input_text, &p_cmd_set))
			break ;
		free_array(&p_cmd_set.input_text);
	}
	free_exit(&p_cmd_set, p_cmd_set.status_code, NULL);
}
