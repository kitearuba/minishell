/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/01 16:40:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/18 02:15:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Prompt loop rules (bash-like):
** - Ctrl-C at prompt: print newline, clear line, redraw prompt; set $?=130;
**   do not execute or add history for that empty line.
** - Ctrl-\ at prompt: ignored.
** - Ctrl-D: exit minishell with current exit_status (print "exit\n").
** - Empty/space-only line: do nothing.
*/

#include "../../include/minishell.h"

int	exit_failure(t_bash *bash)
{
	write(2, "minishell: exit\n", 16);
	free_2d_array(bash->env);
	return (1);
}

static int	is_only_spaces(const char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

static void	parse_and_execute(char *line, t_bash *bash)
{
	bash->tokens = tokenize_input(line);
	if (!bash->tokens)
		return ;
	expand_env_vars(&bash->tokens, bash);
	expand_wildcards(&bash->tokens);
	bash->commands = parse_tokens(bash->tokens);
	if (bash->commands)
	{
		/* execute_command() should map child status:
		** if (WIFSIGNALED) status = 128 + signal; else WEXITSTATUS */
		bash->exit_status = execute_command(bash->commands, bash);
		free_commands(bash->commands);
		bash->commands = NULL;
	}
	else
		ft_printf_fd(2, "Parse error: no commands generated\n");
	free_tokens(bash->tokens);
	bash->tokens = NULL;
}

void	process_input(char *line, t_bash *bash)
{
	if (bash->tokens)
	{
		free_tokens(bash->tokens);
		bash->tokens = NULL;
	}
	if (bash->commands)
	{
		free_commands(bash->commands);
		bash->commands = NULL;
	}
	parse_and_execute(line, bash);
}

void	minishell_loop(t_bash *bash)
{
	char	*line;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	setup_signal_handlers();
	while (1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		line = readline("minishell $> ");
		if (!line)
		{
			ft_printf("exit\n");
			free_all_and_exit(bash, bash->exit_status);
		}
	    if (g_sigint_flag)
		{
			bash->exit_status = 130;
			g_sigint_flag = 0;
	        if (line[0] == '\0')
	        {
	            free(line);
	            continue ;
	        }
		}
		if (*line == '\0' || is_only_spaces(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_input(line, bash);
		free(line);
	}
}