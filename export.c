/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebeiline <ebeiline@42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 15:26:40 by ebeiline          #+#    #+#             */
/*   Updated: 2022/02/24 13:36:14 by pstengl          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_helper(char ***envp, char *token, char *arg)
{
	char	**temp;
	int		i;

	i = 0;
	while ((*envp)[i] != NULL)
	{
		if (!ft_strncmp((*envp)[i], token, ft_strlen(token))
				&& (*envp)[i][ft_strlen(token)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(arg);
			return (0);
		}
		i++;
	}
	temp = ft_calloc(ft_arrlen(*envp) + 2, sizeof(char *));
	if (!temp)
		return (error(5));
	temp = ft_memcpy(temp, *envp, sizeof(char *) * ft_arrlen(*envp));
	temp[ft_arrlen(temp)] = ft_strdup(arg);
	free(*envp);
	*envp = temp;
	return (0);
}

char	**unset_helper(char **envp, char *arg)
{
	char	**temp;
	int		j;
	int		i;

	i = 0;
	j = 0;
	temp = ft_calloc(sizeof(char *), (ft_arrlen(envp) + 2));
	while (envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], arg, ft_strlen(arg))
			&& envp[i][ft_strlen(arg)] == '=')
			free(envp[i]);
		else
		{
			temp[j] = envp[i];
			j++;
		}
		i++;
	}
	free(envp);
	return (temp);
}

int	export_print_declare(char **envp)
{
	char	**parts;
	char	*var;

	while (*envp)
	{
		parts = ft_split(*envp, '=');
		ft_print("declare -x ");
		ft_print(parts[0]);
		ft_print("=\"");
		var = parts[1];
		while (*var)
		{
			if (*var == '"')
				ft_print("\\\"");
			else
				ft_printc(*var);
			var++;
		}
		ft_println("\"");
		ft_arrclear(parts, free);
		envp++;
	}
	return (0);
}

int	builtin_export(char **arg, char ***envp)
{
	int		j;
	int		returncode;
	char	**tokens;

	j = 0;
	returncode = 0;
	if (ft_arrlen(arg) <= 1)
		return (export_print_declare(*envp));
	while (arg[++j])
	{
		if (!(ft_strchr(arg[j], '=')))
		{
			if (!(ft_checkname(arg[j])))
				error (1);
			continue ;
		}
		tokens = ft_split(arg[j], '=');
		if (ft_checkname(tokens[0]))
			returncode += export_helper(envp, tokens[0], arg[j]);
		else
			error (1);
		ft_arrclear(tokens, free);
	}
	return ((returncode > 0));
}

char	**builtin_unset(char **arg, char **envp)
{
	int		j;

	j = 0;
	while (arg[++j])
		envp = unset_helper(envp, arg[j]);
	return (envp);
}
