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
