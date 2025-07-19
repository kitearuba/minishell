#include "../../include/minishell.h"

void free_wildcard(t_wildcard *wc)
{
    int i;
    
    if (!wc)
        return;
    i = 0;
    while (i < wc->count)
    {
        free(wc->matches[i]);
        i++;
    }
    free(wc->matches);
    free(wc);
}

int has_wildcard(char *str)
{
    int i;
    
    if (!str)
        return (0);
    i = 0;
    while (str[i])
    {
        if (str[i] == '*' || str[i] == '?' || str[i] == '[')
            return (1);
        i++;
    }
    return (0);
}


void sort_matches(t_wildcard *wc)
{
    int i;
    int j;
    char *temp;
    
    if (!wc || wc->count < 2)
        return;
    i = 0;
    while (i < wc->count - 1)
    {
        j = 0;
        while (j < wc->count - i - 1)
        {
            if (strcmp(wc->matches[j], wc->matches[j + 1]) > 0)
            {
                temp = wc->matches[j];
                wc->matches[j] = wc->matches[j + 1];
                wc->matches[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

int match_pattern(char *pattern, char *filename)
{
    // compare if there is a match between a pattern(*, ?, []) and a string
    return (fnmatch(pattern, filename, FNM_PERIOD) == 0);
}