/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 16:04:18 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

typedef struct s_bash	t_bash;

int		ft_echo(char **argv, t_bash *bash);
int		ft_pwd(char **argv, t_bash *bash);
int		ft_export(char **argv, t_bash *bash);
void    print_export_sorted(t_bash *bash);
int		append_env_var(char ***env, const char *new_var);
int		ft_unset(char **argv, t_bash *bash);
int		ft_env(char **argv, t_bash *bash);
int		ft_exit(char **argv, t_bash *bash);
int		is_valid_identifier(const char *str);
char	**copy_env(char **env);

int ft_error_handling(char *eq, char *name, t_bash *bash, char *arg);
void	print_export_sorted(t_bash *bash);
int	ft_cd(char **argv, t_bash *bash);
void	cd_error(const char *dest);
void	update_pwd_vars(t_bash *bash, const char *oldpwd);
int	get_home_or_oldpwd(t_bash *bash,
    char **out, int *is_dash, int is_oldpwd);
int	resolve_dest(t_bash *bash, char **argv, char **out, int *is_dash);



#endif /* BUILTIN_H */
