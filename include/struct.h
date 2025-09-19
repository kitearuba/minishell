/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 12:22:12 by chrrodri         ###   ########.fr       */
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

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
	int					quoted;
	int					space_before;
}	t_token;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	int						quoted;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char				**argv;
	t_redirection		*redirection;
	struct s_command	*next;
}	t_command;

typedef struct s_bash
{
	char		**env;
	int			exit_status;
	t_token		*tokens;
	t_command	*commands;
}	t_bash;

#endif /* STRUCT_H */
