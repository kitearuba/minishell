/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                         :+:      :+:    :+:    */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:21:19 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:35:24 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

/* ************************************************************************** */
/*                             FORWARD DECLARATIONS                           */
/* ************************************************************************** */

typedef struct s_bash	t_bash;

/* ************************************************************************** */
/*                             BUILTIN FUNCTIONS                              */
/* ************************************************************************** */

int ft_echo(char **argv, t_bash *bash);
int ft_cd(char **argv, t_bash *bash);
int ft_pwd(char **argv, t_bash *bash);
int ft_export(char **argv, t_bash *bash);
int ft_unset(char **argv, t_bash *bash);
int ft_env(char **argv, t_bash *bash);
int ft_exit(char **argv, t_bash *bash);

#endif /* BUILTIN_H */