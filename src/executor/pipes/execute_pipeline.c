//
// Created by christian on 19/07/25.
//

#include "../../../include/minishell.h"

void execute_single_command(t_command *cmd, t_bash *bash)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        exit(1);
    if (is_builtin(cmd->argv[0]))
    {
        exit(run_builtin(cmd, bash));
    }
    else
    {
        run_external_cmd(cmd, bash);
    }
}

/**
 * Executes a pipeline of commands.
 * Each t_command in the linked list is a stage in the pipeline.
 */
void execute_pipeline(t_command *cmds, t_bash *bash)
{
    int		pipefd[2];
    int		input_fd = STDIN_FILENO;
    pid_t	pid;
    int		status;
    t_command *cmd = cmds;

    while (cmd)
    {
        if (cmd->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            if (input_fd != STDIN_FILENO)
                dup2(input_fd, STDIN_FILENO);
            if (cmd->next)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            execute_single_command(cmd, bash); // handles execve() or builtin
            exit(EXIT_FAILURE); // if exec fails
        }
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        if (cmd->next)
        {
            close(pipefd[1]);
            input_fd = pipefd[0];
        }
        cmd = cmd->next;
    }
    while (wait(&status) != -1)
        continue;
}
