#include "../../include/minishell.h"

void	free_2d_array(char **arr)
{
    int	i;

    if (!arr)
        return ;
    i = 0;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}