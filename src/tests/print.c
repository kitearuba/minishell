//
// Created by christian on 6/07/25.
//

#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                               print_tokens                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Prints the token list to stdout in a formatted way for debugging,       */
/*    showing each token's type and value.                                    */
/*                                                                            */
/*  Parameters:                                                               */
/*  - list: pointer to the first token in the linked list.                    */
/*                                                                            */
/* ************************************************************************** */
void print_tokens(t_token *list)
{
    while (list)
    {
        printf("[%d: \"%s\"] -> ", list->type, list->value);
        list = list->next;
    }
    printf("NULL\n");
}

void	print_commands(t_command *cmd)
{
    int	i;
    t_redirection	*redir;

    while (cmd)
    {
        i = 0;
        printf("Command:\n");
        if (cmd->argv)
        {
            while (cmd->argv[i])
            {
                printf("  argv[%d]: '%s'\n", i, cmd->argv[i]);
                i++;
            }
        }
        else
            printf("  argv: NULL\n");
        redir = cmd->redirection;
        while (redir)
        {
            printf("  redirection: type=%d, filename='%s'\n",
                redir->type, redir->filename);
            redir = redir->next;
        }
        printf("---\n");
        cmd = cmd->next;
    }
}
