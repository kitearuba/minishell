/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                         :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 19:30:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    HEREDOC,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    ENV_VAR
}	t_token_type;

typedef enum e_builtin_type
{
    NO_BUILTIN = 0,
    BUILTIN_CD,
    BUILTIN_PWD,
    BUILTIN_ECHO,
    BUILTIN_ENV,
    BUILTIN_EXIT,
    BUILTIN_EXPORT,
    BUILTIN_UNSET
}	t_builtin_type;

typedef struct s_bash
{
    char	**env;
    int		exit_status;
}	t_bash;

typedef struct s_token
{
    t_token_type		type;
    char				*value;
    struct s_token		*next;
}	t_token;

typedef struct s_redirection
{
    int						type;
    char					*filename;
    struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
    char				**argv;
    t_redirection		*redirection;
    struct s_command	*next;
}	t_command;

#endif /* STRUCT_H */
