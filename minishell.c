/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/01 16:40:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** main
** ----
** Entry point. Initializes shell state from the host env, then starts the
** interactive loop. On early init failure, print the required "exit" line and
** return with failure. 'ac' and 'av' are unused by design (subject rules).
**
** Return: 0 on clean exit (loop never returns), 1 on early init failure.
*/
int	main(int ac, char **av, char **envp)
{
	t_bash	bash;

	(void)ac;
	(void)av;
	if (init_minishell(&bash, envp))
		return (exit_failure(&bash));
	minishell_loop(&bash);
	clear_history();
	free_all_and_exit(&bash, 0);
	return (0);
}
