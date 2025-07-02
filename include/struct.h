/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                         :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:35:24 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/* ************************************************************************** */
/*                                 ENUMS                                      */
/* ************************************************************************** */

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

/* ************************************************************************** */
/*                               STRUCTURES                                   */
/* ************************************************************************** */

typedef struct s_token
{
    t_token_type		type;
    char				*value;
    struct s_token		*next;
}	t_token;

typedef struct s_redirection
{
    int					type;
    char				*filename;
    struct s_redir		*next;
}   t_redirection;

typedef struct s_command
{
    char                **argv;
    t_redirection       *redirection;
    struct s_command    *next;
}   t_command;

#endif /* STRUCTS_H */
