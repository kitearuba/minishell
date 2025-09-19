/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 13:23:33 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ============================== INCLUDES =============================== */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include "../libft/include/libft.h"
# include "builtin.h"
# include "struct.h"

/* ============================== MACROS ================================= */

# define BUILTIN_CMDS ":echo:cd:pwd:export:unset:env:exit:"

/* ========================== GLOBAL VARIABLE ============================ */

extern volatile sig_atomic_t	g_heredoc_interrupted;
extern volatile sig_atomic_t	g_sigint_flag;

/* ======================== FUNCTION PROTOTYPES ========================== */

/* --- Initiation --- */
int			init_minishell(t_bash *bash, char **envp);
int			exit_failure(t_bash *bash);
void		process_input(char *line, t_bash *bash);
void		minishell_loop(t_bash *bash);

/* --- Tokenizer --- */
t_token		*tokenize_input(const char *input);
t_token		*new_token(t_token_type type, const char *start, size_t len,
				int quoted, int space_before);
size_t		handle_pipe(const char *input, t_token **tokens, size_t i,
				int space_before);
size_t		handle_word(const char *input, t_token **tokens, size_t i,
				int space_before);
size_t		handle_env_var(const char *input, t_token **tokens, size_t i,
				int space_before);
int			handle_quotes(const char *input, t_token **tokens, size_t *i,
				int space_before);
void		add_token(t_token **head, t_token *new);
char		*extract_quoted_token(const char *line, size_t *index);

/* --- Expander --- */
void		expand_env_vars(t_token **tokens, t_bash *bash);
char		*get_env_value(const char *key, char **env);
char		*append_and_free(char *s1, char *s2);
void		expand_wildcards(t_token **tokens);
void		replace_with_matches(t_token **tokens,
				t_token *prev, t_token **curr);
t_token		*wildcard_match(const char *pattern, int space_before);
void		handle_match_loop(DIR *dir, const char *pattern,
				t_token **head, t_token **tail, int space_before);

/* --- Parser --- */
t_command	*parse_tokens(t_token *tokens);
t_command	*new_command(void);
t_command	*last_command(t_command *head);
int			last_token_is_pipe(t_token *tok);
int			check_leading_pipe(t_token *tokens, t_command *head,
				t_command *current);
int			check_trailing_pipe(t_token *tokens, t_command *head,
				t_command *current);
int			check_consecutive_pipes(t_token *tok, t_command **current);
int			check_initial_errors(t_token *tok);
t_command	*handle_parse_error(t_command *head, t_command *current,
				t_list *args);
int			check_commandless_redirection(t_command *head);
int			handle_parse_redirection(t_token *tok, t_command **current);
void		add_redirection(t_command *cmd, int type, char *filename,
				int quoted);
void		add_token_argument(t_token *tok, t_list **args);
int			handle_redirection(t_token *tok, t_command **current,
				t_list **args);
int			handle_token_type(t_token *tok, t_command **current,
				t_list **args);

/* --- Execution --- */
int			execute_command(t_command *cmds, t_bash *bash);
int			is_builtin(const char *cmd);
int			run_builtin(char **argv, t_bash *bash);
void		execve_cmd(char **argv, char **env, t_bash *bash);
int			apply_redirections(t_redirection *redir, t_bash *bash);
int			handle_heredoc(t_redirection *redir, t_bash *bash);
char		*hd_expand_key(const char *s, size_t *i, t_bash *bash);
char		*hd_expand_line(const char *s, t_bash *bash);
int			exec_external(char **args, t_bash *bash);

/* --- Command Path --- */
char		*get_cmd_path(char *cmd, char **envp);

/* --- Environment --- */
char		*ft_getenv(char **envp, char *key);
int			env_set(t_bash *bash, const char *key, const char *val);

/* --- Utilities --- */
void		free_2d_array(char **arr);
void		free_tokens(t_token *tokens);
void		free_commands(t_command *cmd);
void		free_all_and_exit(t_bash *bash, int exit_code);

/* --- Pipes --- */
int			execute_pipeline(t_command *cmds, t_bash *bash);
void		create_pipe(int *pipefd, t_bash *bash);
int			fork_and_exec(t_command *cmd, int input_fd,
				int output_fd, t_bash *bash);

/* --- Signal --- */
void		setup_signal_handlers(void);
void		setup_child_signals(void);

/* --- Wildcard Expansion --- */
char		**expand_token_if_wildcard(char *token);

#endif /* MINISHELL_H */
