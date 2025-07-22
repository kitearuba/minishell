//
// Created by christian on 21/07/25.
//
#include "../../include/minishell.h"

static int	match_pattern(const char *pattern, const char *str)
{
    if (!*pattern && !*str)
        return (1);
    if (*pattern == '*')
        return (match_pattern(pattern + 1, str) || (*str && match_pattern(pattern, str + 1)));
    if (*pattern == *str)
        return (match_pattern(pattern + 1, str + 1));
    return (0);
}

static t_token	*wildcard_match(const char *pattern)
{
    DIR				*dir;
    struct dirent	*entry;
    t_token			*head;
    t_token			*tail;
    t_token			*tmp;

    dir = opendir(".");
    if (!dir)
        return (NULL);
    head = NULL;
    tail = NULL;
    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] == '.')
            continue;
        if (match_pattern(pattern, entry->d_name))
        {
            tmp = new_token(WORD, entry->d_name,
                    ft_strlen(entry->d_name), 0);
            if (!tmp)
                continue;
            if (!head)
                head = tmp;
            else
                tail->next = tmp;
            tail = tmp;
        }
    }
    closedir(dir);
    return (head);
}

void	expand_wildcards(t_token **tokens)
{
    t_token	*prev;
    t_token	*curr;
    t_token	*matches;
    t_token	*tail;

    prev = NULL;
    curr = *tokens;
    while (curr)
    {
        if (curr->type == WORD
            && curr->quoted == 0
            && ft_strchr(curr->value, '*'))
        {
            matches = wildcard_match(curr->value);
            if (matches)
            {
                tail = matches;
                while (tail->next)
                    tail = tail->next;
                tail->next = curr->next;
                if (prev)
                    prev->next = matches;
                else
                    *tokens = matches;
                free(curr->value);
                free(curr);
                curr = tail->next;
                continue;
            }
        }
        prev = curr;
        curr = curr->next;
    }
}