/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 15:16:53 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef enum e_token_type
{
	word,
	pipe_tok,
	redirect_in,
	redirect_out,
	redirect_append,
	heredoc_tok,
	single_quote,
	double_quote,
	env_var
}	t_token_type;

typedef enum e_builtin_type
{
	builtin_none = 0,
	builtin_cd,
	builtin_pwd,
	builtin_echo,
	builtin_env,
	builtin_exit,
	builtin_export,
	builtin_unset
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
	char		**export_noval;
}	t_bash;

#endif /* STRUCT_H */
