/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 17:28:03 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                copy_envp                                   */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Creates a deep copy of the environment variable array (envp)            */
/*    by allocating a new NULL-terminated array of strings.                   */
/*                                                                            */
/*  Parameters:                                                               */
/*  - envp: original environment variable array passed to main.               */
/*                                                                            */
/*  Return:                                                                   */
/*  - Returns a pointer to the newly allocated environment array (char **).   */
/*  - Returns NULL on memory allocation failure.                              */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Uses malloc and strdup (both allowed in 42 Minishell subject).          */
/*  - Frees already-allocated entries on failure to prevent memory leaks.     */
/*                                                                            */
/* ************************************************************************** */

static char	**copy_envp(char **envp)
{
	char	**local_envp;
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	local_envp = malloc(sizeof(char *) * (count + 1));
	if (!local_envp)
		return (NULL);
	while (i < count)
	{
		local_envp[i] = strdup(envp[i]);
		if (!local_envp[i])
		{
			while (i--)
				free(local_envp[i]);
			free(local_envp);
			return (NULL);
		}
		i++;
	}
    local_envp[i] = NULL;
	return (local_envp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                init_minishell                              */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Initializes the bash struct by copying envp and setting exit_status.    */
/*                                                                            */
/*  Parameters:                                                               */
/*  - bash: pointer to t_bash struct to initialize.                           */
/*  - envp: environment variable array.                                       */
/*                                                                            */
/*  Return:                                                                   */
/*  -- 0 on success, 1 if environment copy fails.                             */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Relies on copy_envp() for environment duplication.                      */
/*                                                                            */
/* ************************************************************************** */

static int	init_minishell(t_bash *bash, char **envp)
{
    bash->env = copy_envp(envp);
    if (!bash->env)
        return (1);
    bash->exit_status = 0;
    return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                exit_failure                                */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Prints an error message to stderr and frees the bash environment.       */
/*                                                                            */
/*  Parameters:                                                               */
/*  - bash: pointer to t_bash struct with allocated environment.              */
/*                                                                            */
/*  Return:                                                                   */
/*  - Always returns 1 to indicate failure.                                   */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Uses write() to print error to fd 2 (stderr), which is allowed.         */
/*  - Ensures no memory leaks on early exit.                                  */
/*                                                                            */
/* ************************************************************************** */

static int	exit_failure(t_bash *bash)
{
	write(2, "minishell: exit\n", 15);
    free_2d_array(bash->env);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                   main                                     */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Entry point for Minishell. Initializes environment, loads command       */
/*    history, starts an interactive loop with readline, tokenizes input,     */
/*    and handles graceful exit on EOF (Ctrl+D).                              */
/*                                                                            */
/*  Parameters:                                                               */
/*  - ac: argument count (unused).                                            */
/*  - argv: argument vector (unused).                                         */
/*  - envp: environment variable array passed to the shell.                   */
/*                                                                            */
/*  Return:                                                                   */
/*  - 0 on normal exit, 1 on initialization failure.                          */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Uses readline, add_history, read_history, and write_history, which      */
/*    are allowed by the Minishell subject.                                   */
/*  - Commented code shows planned parsing/execution implementation.          */
/*                                                                            */
/* ************************************************************************** */
int	main(int ac, char *argv[], char *envp[])
{
	char        *line;
	t_bash      bash;
    t_token     *tokens;
    t_command   *cmds;
    //char	**args;
    //int     i;
    //i =0;

    (void)ac;
    (void)argv;
    if (init_minishell(&bash, envp))
        return (exit_failure(&bash));
    read_history(".minishell_history");
    while (1)
    {
        line = readline("minishell $> ");
        if (!line)
        {
            printf("exit\n");
            break ;
        }
        if (*line)
        {
            add_history(line);
            tokens = tokenize_input(line);
            if (tokens)
            {
                expand_env_vars(tokens, &bash);
                cmds = parse_tokens(tokens);
                print_tokens(tokens);
                if (cmds)
                {
                    print_commands(cmds);
                    free_commands(cmds);
                }
                else
                    ft_printf_fd(2, "Parse error: no commands generated\n");
            }
            free_tokens(tokens);/*
            args = ft_split(line, ' ');
            while (args[i])
            {
                ft_printf("%s", args[i]);
                i++;
            }
            if (args && args[0])
                execute_command(args, &bash);
            free_2d_array(args);*/
        }
        free(line);
    }
    write_history(".minishell_history");
    clear_history();
    free (bash.env);
    return (0);
}
