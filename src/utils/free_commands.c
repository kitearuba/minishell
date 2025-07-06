//
// Created by christian on 5/07/25.
//

#include "../../include/minishell.h"

void    free_commands(t_command *cmd)
{
    t_command       *next_cmd;
    t_redirection   *redirection;
    t_redirection   *next_redirection;
    int             i;

    while (cmd)
    {
        next_cmd = cmd->next;
        if (cmd->argv)
        {
            i = 0;
            while (cmd->argv[i])
            {
                free(cmd->argv[i]);
                i++;
            }
            free(cmd->argv);
        }
        redirection = cmd->redirection;
        while (redirection)
        {
            next_redirection = redirection->next;
            free(redirection->filename);
            free(redirection);
            redirection = next_redirection;
        }
        free(cmd);
        cmd = next_cmd;
    }
}