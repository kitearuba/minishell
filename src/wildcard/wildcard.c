#include "../../include/minishell.h"

// for more storage capacity
t_wildcard *resize_matches(t_wildcard *wc)
{
    char **new_matches;
    int i;
    
    if (!wc)
        return (NULL);
    wc->capacity *= 2;
    new_matches = malloc(sizeof(char *) * wc->capacity);
    if (!new_matches)
        return (NULL);
    i = 0;
    while (i < wc->count)
    {
        new_matches[i] = wc->matches[i];
        i++;
    }
    free(wc->matches);
    wc->matches = new_matches;
    return (wc);
}

t_wildcard *add_match(t_wildcard *wc, char *filename)
{
    char *match;
    
    if (!wc || !filename)
        return (NULL);
    if (wc->count >= wc->capacity)
        if (!resize_matches(wc))
            return (NULL);
    match = ft_strdup(filename);
    if (!match)
        return (NULL);
    wc->matches[wc->count] = match;
    wc->count++;
    return (wc);
}

t_wildcard *init_wildcard(void)
{
    t_wildcard *wc;
    
    wc = malloc(sizeof(t_wildcard));
    if (!wc)
        return (NULL);
    wc->matches = malloc(sizeof(char *) * 16);
    if (!wc->matches)
    {
        free(wc);
        return (NULL);
    }
    wc->count = 0;
    wc->capacity = 16;
    return (wc);
}

t_wildcard *expand_wildcard(char *pattern)
{
    DIR *dir;
    struct dirent *entry;
    t_wildcard *wc;
    
    wc = init_wildcard();
    if (!wc)
        return (NULL);
    dir = opendir(".");
    if (!dir)
        return (wc);
    entry = readdir(dir);
    while (entry != NULL)
    {
        if (entry->d_name[0] == '.' && pattern[0] != '.')
            continue;
        if (match_pattern(pattern, entry->d_name))
            if (!add_match(wc, entry->d_name))
                break;
    }
    closedir(dir);
    sort_matches(wc);
    return (wc);
}

char **replace_with_matches(char **args, int index, t_wildcard *wc)
{
    char **new_args;
    int i;
    int j;
    int total_args;
    
    if (!args || !wc)
        return (args);
    total_args = 0;
    while (args[total_args])
        total_args++;
    new_args = malloc(sizeof(char *) * (total_args + wc->count));
    if (!new_args)
        return (args);
    i = 0;
    j = 0;
    while (i < total_args)
    {
        if (i == index)
        {
            while (j < wc->count)
                new_args[i + j] = strdup(wc->matches[j++]);
            i += wc->count;
        }
        else
            new_args[i++] = strdup(args[i]);
    }
    new_args[i] = NULL;
    return (new_args);
}