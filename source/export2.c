/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 16:19:43 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/23 01:07:17 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	env_update(t_data *data)
{
	int		num_env;
	int		i;
	t_env	*current_env;
	size_t	key_len;
	size_t	value_len;
	size_t	entry_len;

	num_env = 0;
	i = 0;
	current_env = NULL;
	if (data->envv)
	{
		i = 0;
		while (data->envv[i])
		{
			free(data->envv[i]);
			i++;
		}
		free(data->envv);
	}
	num_env = ft_lstsize_env(data->env_cpy);
	data->envv = (char **)malloc((num_env + 1) * sizeof(char *));
	if (!data->envv)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	current_env = *data->env_cpy;
	i = 0;
	while (current_env)
	{
		key_len = ft_strlen(current_env->key);
		value_len = ft_strlen(current_env->value);
		entry_len = key_len + value_len + 2;
		data->envv[i] = (char *)malloc(entry_len);
		if (!data->envv[i])
		{
			perror("Malloc failed");
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(data->envv[i], current_env->key, entry_len);
		ft_strlcat(data->envv[i], "=", entry_len);
		ft_strlcat(data->envv[i], current_env->value, entry_len);
		current_env = current_env->next;
		i++;
	}
	data->envv[i] = NULL;
}

void	set_in_env(t_data *data, char *variable)
{
	char	**variable_split;
	char	*key;
	char	*value;
	t_env	*new_env;
	int		i;

	variable_split = NULL;
	key = NULL;
	value = NULL;
	new_env = NULL;
	i = 0;
	if (data->flag > 0)
		return ;
	variable_split = ft_split2(variable, '=');
	key = ft_strdup(variable_split[0]);
	value = ft_strdup(variable_split[1]);
	if (key_is_valid(key) == 1 || value[0] == '\0')
	{
		free(value);
		free(key);
		ft_free_tab(variable_split);
		return ;
	}
	new_env = ft_lstnew_env(key, value);
	if (!new_env)
	{
		perror("Malloc failed");
		free(value);
		free(key);
		ft_free_tab(variable_split);
		exit(EXIT_FAILURE);
	}
	if (env_key_exists(*data->env_cpy, key) == 1)
	{
		printf("\n key existe deja \n");
		if (value[0] != '\0')
			remove_env(data, key);
		else
		{
			free(value);
			free(key);
			ft_free_tab(variable_split);
			return ;
		}
	}
	ft_lstadd_back_env(data->env_cpy, new_env);
	free(key);
	free(value);
	ft_free_tab(variable_split);
}

void	set_in_export(t_data *data, char *variable)
{
	char		**variable_split;
	char		*key;
	char		*value;
	t_export	*new_export;
	int			i;
	int			flag;

	variable_split = NULL;
	key = NULL;
	value = NULL;
	new_export = NULL;
	i = 0;
	flag = 0;
	variable_split = ft_split2(variable, '=');
	key = ft_strdup(variable_split[0]);
	value = ft_strdup(variable_split[1]);
	if (key_is_valid(key) == 1)
	{
		if (key[0] == '\0')
			printf("export `': not a valid identifier\n");
		else
			printf("export: `%s=%s': not a valid identifier\n", key, value);
		free(key);
		free(value);
		ft_free_tab(variable_split);
		return ;
	}
	printf("123\n");
	if (ft_strrchr(variable, '=') == NULL)
		flag++;
	if (value[0] != '\0')
		value = add_quotes(value);
	printf("234\n");
	if (export_key_exists(*data->env_export, key) == 1)
	{
		if (value[0] != '\0')
			remove_export(data, key);
		else
		{
			free(value);
			free(key);
			ft_free_tab(variable_split);
			return ;
		}
	}
	new_export = ft_lstnew_export(key, value, flag);
	if (!new_export)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	ft_lstadd_back_export(data->env_export, new_export);
	free(key);
	if (value)
		free(value);
	ft_free_tab(variable_split);
}

void	execute_export(t_data *data, t_cmd *cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	while (cmd->args[i])
	{
		if (ft_strlen(cmd->args[i]) == 1 && cmd->args[i][0] == '=')
		{
			printf("export: `=': not a valid identifier\n");
			i++;
		}
		else
		{
			set_in_export(data, cmd->args[i]);
			set_in_env(data, cmd->args[i]);
			i++;
		}
	}
}

void	free_export(t_export *node)
{
	t_export *next;

	while (node)
	{
		next = node->next;
		free(node->value);
		free(node->key);
		free(node);
		node = next;
	}
}