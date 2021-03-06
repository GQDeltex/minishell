/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebeiline <ebeiline@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 15:16:12 by ebeiline          #+#    #+#             */
/*   Updated: 2022/02/06 15:20:37 by ebeiline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_env_cwd(char ***envp, char *key)
{
	char	**args_export;
	char	*cwd_str;

	cwd_str = cwd();
	args_export = ft_calloc(3, sizeof(char *));
	args_export[0] = ft_strdup("export");
	args_export[1] = ft_strdup(key);
	args_export[1] = ft_strext(&args_export[1], "=", 1);
	args_export[1] = ft_strext(&args_export[1], cwd_str, ft_strlen(cwd_str));
	builtin_export(args_export, envp);
	envp = NULL;
	free(cwd_str);
	ft_arrclear(args_export, free);
}

int	builtin_cd(char **args, char ***envp)
{
	int	argslen;
	int	returnval;

	argslen = 0;
	returnval = -1;
	set_env_cwd(envp, "OLDPWD");
	while (args[argslen] != NULL)
		argslen++;
	if (argslen == 1)
		returnval = chdir(ft_in_envp(*envp, "HOME"));
	else if (argslen == 2)
		returnval = chdir(args[1]);
	else
		return (error(7));
	if (returnval != 0)
		return (error(5));
	set_env_cwd(envp, "PWD");
	return (0);
}
