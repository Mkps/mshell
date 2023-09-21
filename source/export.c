/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:46:26 by uaupetit          #+#    #+#             */
/*   Updated: 2023/09/21 17:48:38 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void env_to_export(t_data *data)
{
    t_env		*current = *data->env_cpy;
    t_export	*new_export = NULL;
    
    while (current != NULL)
    {
        new_export = ft_lstnew_export(current->key, current->value, 0);
        if (!new_export)
        {
            perror("Malloc failed");
            exit(EXIT_FAILURE);
        }
        ft_lstadd_back_export(data->env_export, new_export);
        current = current->next;
    }
}

t_cmd	*find_export_command(t_data *data)
{
	t_cmd	*cmd;

	cmd = *data->cmd_list;
    while (cmd)
    {
        if (cmd->args && cmd->args[0] && ft_strncmp(cmd->args[0], "export", ft_strlen("export")) == 0)
            return cmd;
        cmd = cmd->next;
    }
    return (NULL);
}

void    ft_export(t_data *data)
{
    t_cmd *cmd_lst;
    
    cmd_lst = NULL;
    if (data->env_export == NULL)
        env_to_export(data);
    cmd_lst = find_export_command(data);
    if (cmd_lst->args[1] == NULL)
    {
        sort_export_list(data);
        print_export(data);
        return;
    }
    else
    {
        execute_export(data, cmd_lst);
        env_update(data);
    }
}
/*
void dprint_export(t_data *data)
{
    t_export *current = data->env_export;

    while (current != NULL)
    {   
        if (current->value[0] == '\0' && current->flag == 0)
            printf("1 : %s %s=\"%s\"\n", current->export, current->key, current->value);
        else if (current->value[0] == '\0' && current->flag > 0)
            printf("2 : %s %s flag = %i\n", current->export, current->key, current->flag);
        else
            printf("3 : %s %s=%s\n", current->export, current->key, current->value);
        current = current->next;
    }
}*/

void print_export(t_data *data)
{
    t_export *current;
	
	current = *data->env_export;
    while (current != NULL)
    {   
        if (current->value[0] == '\0' && current->flag == 1)
            printf("1 : %s %s\n", current->export, current->key);
        else if (current->value[0] == '\0' && current->flag == 0)
            printf("2 : %s %s=\"\"\n", current->export, current->key);
        else
            printf("3 : %s %s=%s\n", current->export, current->key, current->value);
        current = current->next;
    }
}
//printf("1 : %s %s=\"\"\n", current->export, current->key);


    //    if (data->flag > 0)
      //  {
          //  printf("1 : %s %s %s\n", current->export, current->key, current->value);
        //}
 ///       else if (current->value[0] == '\0')
    //        printf("2 : %s %s=\"\"\n", current->export, current->key);
        //else

void sort_export_list(t_data *data)
{
    t_export *sorted;
    t_export *current;
    t_export *next;

	sorted = NULL;
	current = *data->env_export;
    while (current != NULL)
    {
        next = current->next;
        insert_sorted(&sorted, current);
        current = next;
    }
    *data->env_export = sorted;
}
