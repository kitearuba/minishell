
#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                           construct_cmd_path                              */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Constructs a full executable path by joining a directory and command,   */
/*    adding a '/' between them if necessary.                                 */
/*                                                                            */
/*  Parameters:                                                               */
/*  - dir: the directory path.                                                */
/*  - cmd: the command name.                                                  */
/*                                                                            */
/*  Return:                                                                   */
/*  - Newly allocated string with the combined path, or NULL on error.        */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Uses malloc, ft_strlcpy, and ft_strlcat (libft).                        */
/*                                                                            */
/* ************************************************************************** */
static char	*construct_cmd_path(char *dir, char *cmd)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

/* ************************************************************************** */
/*                                                                            */
/*                          search_cmd_in_path                               */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Iterates through directories from PATH, constructs full paths           */
/*    to the command, and checks for executability using access().            */
/*                                                                            */
/*  Parameters:                                                               */
/*  - dirs: array of directories split from the PATH variable.                */
/*  - cmd: the command name to search for.                                    */
/*                                                                            */
/*  Return:                                                                   */
/*  - Newly allocated full path to the executable if found, or NULL otherwise.*/
/*                                                                            */
/*  Notes:                                                                    */
/*  - Frees dirs with free_2d_array before returning.                         */
/*                                                                            */
/* ************************************************************************** */
static char	*search_cmd_in_path(char **dirs, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (dirs[i])
	{
		path = construct_cmd_path(dirs[i], cmd);
		if (path && access(path, X_OK) == 0)
		{
			free_2d_array(dirs);
			return (path);
		}
		free(path);
		i++;
	}
	free_2d_array(dirs);
    return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                              get_cmd_path                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Determines the full executable path for a command by checking if it is  */
/*    absolute/relative, or searching in the PATH directories.                */
/*                                                                            */
/*  Parameters:                                                               */
/*  - cmd: the command name.                                                  */
/*  - envp: environment variable array.                                       */
/*                                                                            */
/*  Return:                                                                   */
/*  - Newly allocated string with the resolved executable path, or NULL       */
/*    if the command is not found or an error occurs.                         */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Calls ft_getenv() to retrieve PATH from envp.                           */
/*  - Uses ft_split(), construct_cmd_path(), search_cmd_in_path().            */
/*  - Uses access() (allowed) to check command executability.                 */
/*                                                                            */
/* ************************************************************************** */
char	*get_cmd_path(char *cmd, char **envp)
{
	char	**dirs;
    char    *path_value;

    if (!cmd || !*cmd)
    {
        return (NULL);
    }
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
            return (NULL);
	}
    path_value = ft_getenv(envp, "PATH");
    if (!path_value)
        return (NULL);
    dirs = ft_split(path_value, ':');
    if (!dirs)
    {
        return (NULL);
    }
	return (search_cmd_in_path(dirs, cmd));
}
