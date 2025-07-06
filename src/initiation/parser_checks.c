//
// Created by christian on 6/07/25.
//

#include "../../include/minishell.h"

/**
 * Check if the first token is a pipe (leading pipe).
 */
int	check_leading_pipe(t_token *tokens, t_command *head, t_command *current)
{
    if (tokens && tokens->type == PIPE)
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        if (head)
            free_commands(head);
        if (current)
            free_commands(current);
        return (1);
    }
    return (0);
}

/**
 * Check if the last token is a pipe (dangling pipe).
 */
int	check_trailing_pipe(t_token *tokens, t_command *head, t_command *current)
{
    if (last_token_is_pipe(tokens))
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        if (head)
            free_commands(head);
        if (current)
            free_commands(current);
        return (1);
    }
    return (0);
}

/**
 * Check if there's a command with only redirections and no argv.
 */
int	check_commandless_redirection(t_command *head, t_command *current)
{
    if (current && !current->argv)
    {
        ft_printf_fd(2, "Syntax error: unexpected redirection with no command\n");
        if (head)
            free_commands(head);
        if (current)
            free_commands(current);
        return (1);
    }
    return (0);
}

/**
 * Handle general parse loop errors.
 * Frees resources and returns NULL.
 */
t_command *handle_parse_error(t_command *head, t_command *current)
{
    if (head)
        free_commands(head);
    if (current)
        free_commands(current);
    return (NULL);
}
