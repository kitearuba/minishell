#include "../../include/minishell.h"

static int	is_wildcard(const char *str)
{
    while (*str)
    {
        if (*str == '*')
            return (1);
        str++;
    }
    return (0);
}

static int	match_pattern(const char *pattern, const char *filename)
{
    if (*pattern == '\0' && *filename == '\0')
        return (1);
    if (*pattern == '*')
        return (match_pattern(pattern + 1, filename) ||
                (*filename && match_pattern(pattern, filename + 1)));
    if (*pattern == *filename)
        return (match_pattern(pattern + 1, filename + 1));
    return (0);
}

static char **list_to_array(t_list *lst)
{
    int		len = ft_lstsize(lst);
    char	**array = malloc(sizeof(char *) * (len + 1));
    int		i = 0;

    if (!array)
        return (NULL);
    while (lst)
    {
        array[i++] = lst->content;
        lst = lst->next;
    }
    array[i] = NULL;
    return (array);
}

 static char	**get_matches(const char *pattern)
{
    DIR				*dir;
    struct dirent	*entry;
    t_list			*matches = NULL;
    char			**result;

    dir = opendir(".");
    if (!dir)
        return (NULL);
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] == '.')
            continue;
        if (match_pattern(pattern, entry->d_name))
            ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
    }
    closedir(dir);
    result = list_to_array(matches);
    ft_lstclear(&matches, free);
    return (result);
}

char	**expand_token_if_wildcard(char *token)
{
    if (!is_wildcard(token))
        return (ft_split(token, ' '));
    return (get_matches(token));
}


/*
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
*/