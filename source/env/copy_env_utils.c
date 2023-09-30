/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:21:05 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/28 18:11:36 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_env_lst(t_env **env_lst)
{
	t_env	*current;
	t_env	*next;

	current = *env_lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	free_env_node(t_env *env_node)
{
	free(env_node->key);
	free(env_node->value);
	free(env_node);
}

void	ft_lstadd_back_two(t_env **lst, t_env *new)
{
	t_env	*temp;

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

t_env	*ft_lstnew_two(char *key, char *value)
{
	t_env	*list;

	list = (t_env *)malloc(sizeof (t_env));
	if (!list)
		return (NULL);
	list->key = key;
	list->value = value;
	list->next = NULL;
	return (list);
}

char	*ft_getenvcpy(t_data *data, char *key)
{
	t_env	*current;

	current = *data->env_cpy;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
