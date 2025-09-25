/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:11:09 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** exit_failure
** ------------
** Used on startup failure before entering the prompt loop:
** print required "minishell: exit" and free duplicated env.
** Return 1 so main can propagate failure.
*/
int	exit_failure(t_bash *bash)
{
	write(2, "minishell: exit\n", 16);
	free_2d_array(bash->env);
	return (1);
}

/*
** parse_and_execute
** -----------------
** One-shot pipeline for a single input line:
**   tokenize -> expand env -> expand wildcards -> parse -> execute
** Frees temporary token/command lists locally.
** On parse failure, emit a friendly diagnostic and leave exit_status as-is.
*/
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
		bash->exit_status = execute_command(bash->commands, bash);
		free_commands(bash->commands);
		bash->commands = NULL;
	}
	else
		ft_printf_fd(2, "Parse error: no commands generated\n");
	free_tokens(bash->tokens);
	bash->tokens = NULL;
}

/*
** process_input
** -------------
** Safety wrapper before parsing a fresh line:
**   - ensure any previous token/command lists are freed
**   - run parse+execute for the new line
*/
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

/*
** handle_line
** -----------
** Prompt-level line handler:
**   - If SIGINT was caught at the prompt, set $?=130 and clear the flag.
**     Empty lines after Ctrl-C are ignored (bash-like behavior).
**   - Ignore empty/space-only lines.
**   - Otherwise add to history and process.
*/
static void	handle_line(char *line, t_bash *bash)
{
	if (*get_sigint_flag())
	{
		bash->exit_status = 130;
		*get_sigint_flag() = 0;
		if (line[0] == '\0')
			return ;
	}
	if (*line == '\0' || is_only_spaces(line))
		return ;
	add_history(line);
	process_input(line, bash);
}

/*
** minishell_loop
** --------------
** Interactive loop:
**   - Install prompt signal handlers
**   - Keep saved copies of STDIN/STDOUT (restored each iteration)
**   - Read with readline; on EOF (^D) print "exit" and cleanly terminate
**   - Delegate the actual work to handle_line() per iteration
*/
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
		handle_line(line, bash);
		free(line);
	}
}
