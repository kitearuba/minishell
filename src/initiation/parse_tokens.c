#include "../../include/minishell.h"

t_command   *parse_tokens(t_token *tokens)
{
    t_command   *head;
    t_command   *curr;
    t_command   *new_cmd;

    head = NULL;
    curr = NULL;
    while (tokens)
    {
        new_cmd = malloc(sizeof(t_command));
        if (!new_cmd)
            return(NULL);
    }
}