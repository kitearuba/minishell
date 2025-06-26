/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdaniely <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by vdaniely          #+#    #+#             */
/*   Updated: 2025/05/20 17:28:03 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

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

static int	init_minishell(t_bash *bash, char **envp)
{
    bash->env = copy_envp(envp);
    if (!bash->env)
        return (1);
    bash->exit_status = 0;
    return (0);
}

static int	exit_failure(t_bash *bash)
{
	write(2, "minishell: exit\n", 15);
    free_2d_array(bash->env);
	return (1);
}

int	main(int ac, char *argv[], char *envp[])
{
	char	*line;
    //char	**args;
	t_bash	bash;
    //int     i;
    t_token *tokens;

    //i =0;
    (void)ac;
    (void)argv;
    if (init_minishell(&bash, envp))
        return (exit_failure(&bash));
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
            tokens = tokenize_input(line);
            print_tokens(tokens);
            free_tokens(tokens);
            add_history(line);/*
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
    clear_history();
    free (bash.env);
    return (0);
}
