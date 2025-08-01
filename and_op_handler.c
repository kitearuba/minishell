#include "../include/minishell.h"

int execute_cmds_chain(t_command *command_list)
{
    t_command *current_cmd;
    int last_exit_status;

    current_cmd = command_list;
    last_exit_status = 0;
    while (current_cmd)
    {
        last_exit_status = execute_command(current_cmd); // run builtin
        if (current_cmd->op == OP_NONE)
            break;
        else if (current_cmd->op == OP_AND) // continue ONLY if the last one was successful
        {
            if (last_exit_status != 0)
            {
                g_sig.exit_status = last_exit_status; // global variable needed
                break;
            }
        }
        else if (current_cmd->op == OP_OR)
        {
            if (last_exit_status == 0) // continue ONLY if it's not successful
            {
                g_sig.exit_status = last_exit_status;
                break;
            }
        }
        current_cmd = current_cmd->next;
    }
    free_commands(command_list);
    return (last_exit_status);
}

//I didnt had the prototype, and needed you to check the "run builtin" part and the global variable
// because i dont know if i can use yours ! 