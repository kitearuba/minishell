/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/21 19:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**list_to_argv(t_list *args)
{
	char	**argv;
	int		count = 0;
	int		i = 0;
	t_list	*temp = args;

	while (temp)
	{
		count++;
		temp = temp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	temp = args;
	while (temp)
	{
		argv[i++] = temp->content;
		temp = temp->next;
	}
	argv[i] = NULL;
	return (argv);
}

static void	finalize_cmd(t_command **head, t_command **current, t_list **args)
{
    t_list	*tmp;
    t_list	*next;

    if (!*current)
        return ;
    (*current)->argv = list_to_argv(*args);
    tmp = *args;
    while (tmp)
    {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    *args = NULL;
    if (!*head)
        *head = *current;
    else
        last_command(*head)->next = *current;
    *current = NULL;
}

static int	parse_loop(t_command **head, t_command **current,
		t_list **args, t_token *tok)
{
	while (tok)
	{
		if (!*current)
		{
			if (check_initial_errors(tok))
			{
				ft_lstclear(args, free);
				return (1);
			}
			*current = new_command();
		}
		if (check_consecutive_pipes(tok, current))
		{
			ft_lstclear(args, free);
			return (1);
		}
		if (tok->type == WORD || tok->type == SINGLE_QUOTE
			|| tok->type == DOUBLE_QUOTE || tok->type == ENV_VAR)
			ft_lstadd_back(args, ft_lstnew(ft_strdup(tok->value)));
		else if (tok->type >= REDIRECT_IN && tok->type <= HEREDOC)
		{
			if (handle_parse_redirection(tok, current))
			{
				ft_lstclear(args, free);
				return (1);
			}
			tok = tok->next;
		}
		else if (tok->type == PIPE)
			finalize_cmd(head, current, args);
		tok = tok->next;
	}
	return (0);
}

static t_command	*check_parse_errors(t_command *head,
	t_command *current, t_list *args, t_token *tokens)
{
	if (current)
		finalize_cmd(&head, &current, &args);
	if (last_token_is_pipe(tokens))
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		free_commands(head);
		return (NULL);
	}
	if (check_commandless_redirection(head))
		return (NULL);
	if (!head)
	{
		ft_printf_fd(2, "Syntax error: no command given\n");
		return (NULL);
	}
	return (head);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head = NULL;
	t_command	*current = NULL;
	t_list		*args = NULL;

	if (tokens && tokens->type == PIPE)
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		return (NULL);
	}
	if (parse_loop(&head, &current, &args, tokens))
		return (handle_parse_error(head, current, args));
	return (check_parse_errors(head, current, args, tokens));
}
