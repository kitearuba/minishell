/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 16:04:28 by chrrodri         ###   ########.fr       */
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

/* ============================== CORE =================================== */

/* init & main loop */
int						init_minishell(t_bash *bash, char **envp);
int						exit_failure(t_bash *bash);
void					process_input(char *line, t_bash *bash);
void					minishell_loop(t_bash *bash);

/* ============================== LEXER ================================== */

/* token creation & stream */
t_token					*tokenize_input(const char *input);
t_token					*new_token(t_token_type type, const char *start,
							size_t len, int quoted);
void					add_token(t_token **head, t_token *new);

/* token kind handlers */
size_t					handle_pipe(const char *input, t_token **tokens,
							size_t i, int space_before);
size_t					handle_word(const char *input, t_token **tokens,
							size_t i, int space_before);
size_t					handle_env_var(const char *input, t_token **tokens,
							size_t i, int space_before);
int						handle_quotes(const char *input, t_token **tokens,
							size_t *i, int space_before);
char					*extract_quoted_token(const char *line, size_t *index);

/* redirects */
size_t					append_redirect_token(const char *in, t_token **toks,
							size_t i, int space);

/* ============================== EXPANDER =============================== */

char					*expand_token_value(t_token *token, t_bash *bash);
void					expand_env_var(t_token **head, t_token **prev,
							t_token **cur, t_bash *b);
void					expand_env_vars(t_token **tokens, t_bash *bash);
void					expand_one(t_token **head, t_token **prev,
							t_token **cur, t_bash *b);
char					*get_env_value(const char *key, char **env);
char					*append_and_free(char *s1, char *s2);

/* wildcard expansion */
void					expand_wildcards(t_token **tokens);
t_token					*wildcard_match(const char *pattern, int space_before);
int						match_pattern(const char *p, const char *s);

/* ============================== PARSER ================================= */

t_command				*parse_tokens(t_token *tokens);
t_command				*new_command(void);
t_command				*last_command(t_command *head);
int						last_token_is_pipe(t_token *tok);

/* parser checks */
int						check_initial_errors(t_token *tok);
int						check_leading_pipe(t_token *tokens, t_command *head,
							t_command *current);
int						check_trailing_pipe(t_token *tokens, t_command *head,
							t_command *current);
int						check_consecutive_pipes(t_token *tok,
							t_command **current);
int						check_commandless_redirection(t_command *head);

/* parser building helpers */
t_command				*handle_parse_error(t_command *head, t_command *current,
							t_list *args);
int						handle_parse_redirection(t_token *tok,
							t_command **current);
void					add_redirection(t_command *cmd, int type,
							char *filename, int quoted);
void					add_token_argument(t_token *tok, t_list **args);
int						handle_redirection(t_token *tok, t_command **current,
							t_list **args);
int						handle_token_type(t_token *tok, t_command **current,
							t_list **args);

/* argv helpers */
char					**list_to_argv(t_list *args);
void					finalize_cmd(t_command **head, t_command **current,
							t_list **args);

/* ============================== EXECUTION ============================== */

int						execute_command(t_command *cmds, t_bash *bash);
int						is_builtin(const char *cmd);
int						run_builtin(char **argv, t_bash *bash);

/* external commands */
void					execve_cmd(char **argv, char **env, t_bash *bash);
int						exec_external(char **args, t_bash *bash);

/* redirections & heredoc */
int						apply_redirections(t_redirection *redir, t_bash *bash);
int						handle_heredoc(t_redirection *redir, t_bash *bash);
char					*hd_expand_key(const char *s, size_t *i, t_bash *bash);
char					*hd_expand_line(const char *s, t_bash *bash);

/* parent/child signal modes around exec */
void					parent_ignore_signals(void);
void					parent_restore_prompt_signals(void);

/* ============================== PIPELINES ============================== */

int						execute_pipeline(t_command *cmds, t_bash *bash);
void					create_pipe(int *pipefd, t_bash *bash);

/* ============================== COMMAND PATH =========================== */

char					*get_cmd_path(const char *cmd, char **envp);

/* ============================== ENVIRONMENT ============================ */

char					*ft_getenv(char **envp, char *key);
int						env_set(t_bash *bash, const char *key,
							const char *val);

/* ============================== SIGNALS ================================ */

void					setup_signal_handlers(void);
void					setup_child_signals(void);
volatile sig_atomic_t	*get_sigint_flag(void);

/* ============================== UTILITIES ============================== */

void					free_2d_array(char **arr);
void					free_tokens(t_token *tokens);
void					free_commands(t_command *cmd);
void					free_all_and_exit(t_bash *bash, int exit_code);
int						is_only_spaces(const char *s);

#endif /* MINISHELL_H */
