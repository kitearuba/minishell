/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:45:41 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                              ft_getenv                                   */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Searches the provided environment array (envp) for a variable matching  */
/*    the given key, and returns a pointer to its value.                      */
/*                                                                            */
/*  Parameters:                                                               */
/*  - envp: environment variable array.                                       */
/*  - key: name of the environment variable to search for.                    */
/*                                                                            */
/*  Return:                                                                   */
/*  - Pointer to the value string of the environment variable, or NULL if     */
/*    the key is not found.                                                   */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Compares keys using ft_strncmp up to the length of key, checking for    */
/*    an exact match followed by '='.                                         */
/*                                                                            */
/* ************************************************************************** */
char	*ft_getenv(char **envp, char *key)
{
    size_t	len;
    int		i;

    if (!key || !envp)
        return (NULL);
    len = ft_strlen(key);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}
