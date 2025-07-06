#include "../../include/minishell.h"

static void	add_redirection(t_command *cmd, int type, char *filename)
{
	t_redirection	*redirection;
	t_redirection	*temp;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return ;
	ft_memset(redirection, 0, sizeof(t_redirection));
	redirection->type = type;
	redirection->filename = ft_strdup(filename);
	if (!cmd->redirection)
		cmd->redirection = redirection;
	else
	{
		temp = cmd->redirection;
		while (temp->next)
			temp = temp->next;
		temp->next = redirection;
	}
}

static char	**list_to_argv(t_list *args)
{
	char	**argv;
	int		count;
	int		i;
	t_list	*temp;

	count = 0;
	temp = args;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	temp = args;
	while (temp)
	{
		argv[i] = temp->content;
		temp = temp->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

static void	finalize_command(t_command **head, t_command **current, t_list **args)
{
	(*current)->argv = list_to_argv(*args);
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
                return (1);
            *current = new_command();
        }
        if (check_consecutive_pipes(tok, current))
            return (1);
        if (tok->type == WORD || tok->type == SINGLE_QUOTE
            || tok->type == DOUBLE_QUOTE || tok->type == ENV_VAR)
            ft_lstadd_back(args, ft_lstnew(ft_strdup(tok->value)));
        else if (tok->type >= REDIRECT_IN && tok->type <= HEREDOC)
        {
            if (tok->next)
            {
                add_redirection(*current, tok->type, tok->next->value);
                tok = tok->next;
            }
            else
            {
                ft_printf_fd(2,
                    "Syntax error: missing filename after redirection\n");
                free_commands(*current);
                *current = NULL;
                return (1);
            }
        }
        else if (tok->type == PIPE)
            finalize_command(head, current, args);
        tok = tok->next;
    }
	return (0);
}

t_command	*parse_tokens(t_token *tokens)
{
    t_command	*head;
    t_command	*current;
    t_list		*args;

    head = NULL;
    current = NULL;
    args = NULL;
    if (tokens && tokens->type == PIPE)
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        return (NULL);
    }
    if (parse_loop(&head, &current, &args, tokens))
        return (handle_parse_error(head, current));
    if (last_token_is_pipe(tokens))
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        free_commands(head);
        free_commands(current);
        return (NULL);
    }
    if (current)
        finalize_command(&head, &current, &args);
    if (check_commandless_redirection(head, current))
        return (NULL);
    if (!head)
    {
        ft_printf_fd(2, "Syntax error: no command given\n");
        return (NULL);
    }
    return (head);
}
