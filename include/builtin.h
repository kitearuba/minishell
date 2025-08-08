/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                         :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/01 16:30:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

typedef struct s_bash	t_bash;

int		ft_echo(char **argv, t_bash *bash);
int		ft_cd(char **argv, t_bash *bash);
int		ft_pwd(char **argv, t_bash *bash);
int		ft_export(char **argv, t_bash *bash);
int		append_env_var(char ***env, const char *new_var);
int		ft_unset(char **argv, t_bash *bash);
int		ft_env(char **argv, t_bash *bash);
void	ft_exit(char **argv, t_bash *bash);
int		is_valid_identifier(const char *str);
char	**copy_env(char **env);

#endif /* BUILTIN_H */
