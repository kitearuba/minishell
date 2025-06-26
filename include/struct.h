#ifndef STRUCTS_H
# define STRUCTS_H

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    HEREDOC,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}	t_token_type;

typedef struct s_token
{
    t_token_type		type;
    char				*value;
    struct s_token		*next;
}	t_token;

typedef struct s_redirection
{
    t_token_type        type;
    struct s_redirection  *next;
}   t_redirection;

typedef struct s_command
{
    char                **argv;
    t_redirection       *redirs;
    struct s_command    *next;
}   t_command;

#endif
