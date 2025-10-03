/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_view.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:59:33 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 16:14:47 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **merge_export_view(char **env, char **noval)
{
    char    **out;
    int     i;
    int     j;
    int     n_env;
    int     n_noval;

    n_env = 0;
    while (env && env[n_env])
        n_env++;
    n_noval = 0;
    while (noval && noval[n_noval])
        n_noval++;
    out = (char **)malloc(sizeof(char *) * (n_env + n_noval + 1));
    if (!out)
        return (NULL);
    i = 0;
    while (i < n_env)
    {
        out[i] = ft_strdup(env[i]);
        i++;
    }
    j = 0;
    while (j < n_noval)
    {
        out[i + j] = ft_strdup(noval[j]);
        j++;
    }
    out[i + j] = NULL;
    return (out);
}

static void sort_strings(char **v)
{
    int     i;
    int     j;
    char    *tmp;

    i = 0;
    while (v && v[i])
    {
        j = 0;
        while (v[j] && v[j + 1])
        {
            if (ft_strcmp(v[j], v[j + 1]) > 0)
            {
                tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
            }
            j++;
        }
        i++;
    }
}

/* print one export line exactly like bash: declare -x KEY="VAL" or declare -x KEY */
static void	print_export_line(const char *entry)
{
	const char	*eq;
	size_t		keylen;
	size_t		vallen;

	if (!entry)
		return ;
	eq = ft_strchr(entry, '=');
	if (!eq)
	{
		write(1, "declare -x ", 11);
		write(1, entry, ft_strlen(entry));
		write(1, "\n", 1);
		return ;
	}
	keylen = (size_t)(eq - entry);
	write(1, "declare -x ", 11);
	write(1, entry, keylen);
	write(1, "=\"", 2);
	vallen = ft_strlen(eq + 1);
	if (vallen > 0)
		write(1, eq + 1, vallen);
	write(1, "\"\n", 2);
}

void    print_export_sorted(t_bash *bash)
{
    char    **view;
    int     i;

    view = merge_export_view(bash->env, bash->export_noval);
    if (!view)
        return ;
    sort_strings(view);
    i = 0;
    while (view[i])
    {
        print_export_line(view[i]);
        i++;
    }
    free_2d_array(view);
}
