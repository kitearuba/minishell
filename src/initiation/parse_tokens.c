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

void    add_redirection(t_command *cmd, int type, char *filename)
{
    t_redirection   *redirection;
    t_redirection   *temp;

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

char    **list_to_argv (t_list *args)
{
    char    **argv:
    int     count;
    int     i;
    t_list  *temp;

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
}