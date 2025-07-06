//
// Created by christian on 2/07/25.
//
#include "../../include/minishell.h"

t_command   *new_command(void)
{
    t_command   *cmd;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return(NULL);
    ft_memset(cmd, 0, sizeof(t_command));
    return (cmd);
}

t_command    *last_command(t_command *head)
{
    while (head && head->next)
        head = head->next;
    return (head);
}

int	last_token_is_pipe(t_token *tok)
{
    if (!tok)
        return (0);
    while (tok->next)
        tok = tok->next;
    return (tok->type == PIPE);
}

#include "../../include/minishell.h"

int	check_initial_errors(t_token *tok)
{
    if (!tok)
        return (0);
    if (tok->type == PIPE)
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        return (1);
    }
    if (tok->type >= REDIRECT_IN && tok->type <= HEREDOC)
    {
        ft_printf_fd(2,
            "Syntax error: unexpected redirection with no command\n");
        return (1);
    }
    return (0);
}

int	check_consecutive_pipes(t_token *tok, t_command **current)
{
    if (tok->type == PIPE && tok->next
        && (tok->next->type == PIPE
            || (tok->next->type >= REDIRECT_IN
                && tok->next->type <= HEREDOC)))
    {
        ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
        free_commands(*current);
        *current = NULL;
        return (1);
    }
    return (0);
}
