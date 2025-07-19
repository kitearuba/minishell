/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 19:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

static char	**copy_envp(char **envp)
{
	char	**local_envp;
	int		i;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	local_envp = malloc(sizeof(char *) * (count + 1));
	if (!local_envp)
		return (NULL);
	i = 0;
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

static int	init_minishell(t_bash *bash, char **envp)
{
	bash->env = copy_envp(envp);
	if (!bash->env)
		return (1);
	bash->exit_status = 0;
    bash->tokens = NULL;
    bash->commands = NULL;
	return (0);
}

static int	exit_failure(t_bash *bash)
{
	write(2, "minishell: exit\n", 16);
	free_2d_array(bash->env);
	return (1);
}

static void	process_input(char *line, t_bash *bash)
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
    bash->tokens = tokenize_input(line);
    if (!bash->tokens)
        return ;
    expand_env_vars(bash->tokens, bash);
    bash->commands = parse_tokens(bash->tokens);
    if (bash->commands && is_builtin(bash->commands->argv[0]))
    {
        bash->exit_status = run_builtin(bash->commands->argv, bash);
        free_commands(bash->commands);
        bash->commands = NULL;
    }
    else if (bash->commands)
    {
        run_external_cmd(bash->commands, bash);
    }
    else
    {
        ft_printf_fd(2, "Parse error: no commands generated\n");
    }
    free_tokens(bash->tokens);
    bash->tokens = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_bash	bash;

	(void)ac;
	(void)av;
	if (init_minishell(&bash, envp))
		return (exit_failure(&bash));
	while (1)
	{
		line = readline("minishell $> ");
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		if (*line)
			process_input(line, &bash);
		free(line);
	}
	clear_history();
    free_all_and_exit(&bash, 0);
	return (0);
}
