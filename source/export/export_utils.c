/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <alex.loubiere@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 15:50:33 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/25 14:18:07 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_lstadd_back_env(t_env **lst, t_env *new)
{
	t_env	*temp;

	temp = NULL;
	if (!lst || !new)
		return ;
	new->next = NULL;
	if (*lst)
	{
		temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	else
		*lst = new;
}

t_env	*ft_lstnew_env(char *key, char *value)
{
	t_env	*new_env;

	new_env = (t_env *)malloc(sizeof(t_export));
	if (!new_env)
		return (NULL);
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	return (new_env);
}

void	free_env_list(t_env **env)
{
	t_env	*current;
	t_env	*next;

	next = NULL;
	current = *env;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

int	env_key_exists(t_env *env, char *key_to_check)
{
	while (env != NULL)
	{
		if (ft_strncmp(env->key, key_to_check, ft_strlen(key_to_check)) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

int	ft_lstsize_env(t_env **lst)
{
	size_t	size;
	t_env	*current;

	size = 0;
	current = *lst;
	while (current)
	{
		current = current->next;
		size++;
	}
	return (size);
}

int	print_envp(t_data *data)
{
	int	i;

	i = 0;
	while (data->envv[i])
	{
		printf("%s\n", data->envv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
