/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_noval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 15:21:36 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 15:58:01 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  name_exists(char **v, const char *name)
{
    size_t  nlen;
    int     i;

    if (!v || !name)
        return (0);
    nlen = ft_strlen(name);
    i = 0;
    while (v[i])
    {
        if (ft_strncmp(v[i], name, nlen) == 0 && v[i][nlen] == '\0')
            return (1);
        i++;
    }
    return (0);
}

static int  push_name(char ***v, const char *name)
{
    int     i;
    char    **dst;

    i = 0;
    while ((*v) && (*v)[i])
        i++;
    dst = (char **)malloc(sizeof(char *) * (i + 2));
    if (!dst)
        return (1);
    i = 0;
    while ((*v) && (*v)[i])
    {
        dst[i] = (*v)[i];
        i++;
    }
    dst[i] = ft_strdup(name);
    dst[i + 1] = NULL;
    free((*v));
    *v = dst;
    return (0);
}

int export_declare_name_only(t_bash *bash, const char *name)
{
    char    *val;

    if (!bash || !name)
        return (1);
    val = ft_getenv(bash->env, (char *)name);
    if (val)
        return (0);
    if (name_exists(bash->export_noval, name))
        return (0);
    if (push_name(&bash->export_noval, name))
        return (1);
    return (0);
}

void    export_list_remove(t_bash *bash, const char *name)
{
    int     i;
    int     j;

    if (!bash || !bash->export_noval || !name)
        return ;
    i = 0;
    while (bash->export_noval[i]
        && ft_strcmp(bash->export_noval[i], name) != 0)
        i++;
    if (!bash->export_noval[i])
        return ;
    free(bash->export_noval[i]);
    j = i;
    while (bash->export_noval[j + 1])
    {
        bash->export_noval[j] = bash->export_noval[j + 1];
        j++;
    }
    bash->export_noval[j] = NULL;
}
